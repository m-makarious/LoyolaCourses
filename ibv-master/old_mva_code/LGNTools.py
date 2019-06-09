from numpy import *

import LGN
import mdp
import pickle

def collect_patches(lgn=LGN.LGN(), num_patches = 2000, patch_width = 8, downsample=1):
  """ collects image patches from the LGN model for analysis """

  max_tries = num_patches * 20
  
  img_first_patch = 0 # the first patch number accepted from an image
  img_first_try = 0 # the first attempt to take a patch from the image
  patch_cnt = 0
  try_cnt = 0
  ds = downsample
  w = patch_width * ds
  d = w * w * lgn.num_layers
  d_final = patch_width * patch_width * lgn.num_layers
  avg_filt = ones([ds, ds],'float') / ds**2

  layer_patch = zeros([lgn.num_layers,w,w],float)
  patch = zeros([d,1],float)
  
  img_patches = zeros([d_final,num_patches],float)
  
  # collect the patches
  while patch_cnt < num_patches and try_cnt < max_tries:
    try_cnt += 1  # number of total patches attempted

    if (try_cnt - img_first_try) > 20 * 100 or \
      (patch_cnt - img_first_patch) > 100:
      # change the image sampled from
      lgn.reset_wave()
      img_first_patch = patch_cnt
      img_first_try = try_cnt
      print float(patch_cnt)/num_patches 
    
    px = random.randint(0,lgn.width - w)
    py = random.randint(0,lgn.width - w)

    # create an activity matrix of 0's and 1's (instead of True and False)
    active01 = zeros([lgn.num_layers,lgn.width,lgn.width],int)
    active01[where(lgn.active)] = 1
        
    layer_patch = active01[:,px:px+w,py:py+w].copy()
    frac_active = mean(layer_patch)
    # normalize for the number of possibly active sites
    # frac_active /= lgn.p
    
    if frac_active > 0.2 and frac_active < 0.8:
      # create the patch vector
      # downsample the patch
      down_patch = zeros([lgn.num_layers,patch_width,patch_width],'float')
      for l in range(lgn.num_layers):
        for x in range(patch_width):
          for y in range(patch_width):
            down_patch[l,x,y] = sum(avg_filt * layer_patch[l,ds*x:ds*x+ds,ds*y:ds*y+ds])      
      patch = reshape(down_patch, d_final)     
      patch = patch - mean(patch)         
      img_patches[:,patch_cnt] = patch.copy()
      patch_cnt += 1
  return img_patches

def find_filters(img_patches, perc_var = 0.9, filter_info={}):

  if 'pca' not in filter_info:
    # whitening the input
    filter_info['whiteNode'] = mdp.nodes.WhiteningNode(output_dim = perc_var, svd=True)
    filter_info['whiteNode'].train(img_patches.transpose())
    filter_info['whiteOut'] = filter_info['whiteNode'].execute(img_patches.transpose())
    filter_info['pca_filters'] = filter_info['whiteNode'].get_eigenvectors()
  filter_info['icaNode'] = mdp.nodes.FastICANode(approach = 'symm', stabilization=True, 
    g = 'tanh', fine_g = 'tanh', limit=1e-4, verbose = True, whitened=True, max_it = 1000)    
  filter_info['icaNode'].train(filter_info['whiteOut'])
  filter_info['icaNode'].stop_training()
  #filter_info['icaNode'].stop_training()
  
  # producing the ica bases and filters
  W = filter_info['whiteNode'].get_projmatrix(0)
  R = filter_info['whiteNode'].get_recmatrix(0)
  A = filter_info['icaNode'].get_projmatrix(1)  
  # the math with data in column vectors
  # x_white = W * x_orig            x_orig ~ R * x_white,    W * R = I,  R*W ~ I
  # x_white = A * s                 inv(A) = A.trans in a whitened space
  # x_orig ~ R * A * s  -->   columns of (R * A) are a basis set
  # s = A.trans * W * x --> rows of (A.trans * W) are filters --> cols of (W.trans * A)
  filter_info['ica_bases'] = dot(R,A)
  filter_info['ica_filters'] = dot(W.transpose(),A)
  # broken? below is (A * R.trans).trans + avg_mat = R * A.trans + avg_mat
  # ica_filters = whiteNode.inverse(A).transpose()
  
  return filter_info
  
  
def find_pc(lgn):
  """ this function finds the percolation threshold for a set of parameters
      defined in lgn  - needs to be fixed """
      
  # preserve lgn's p value
  original_p = lgn.p
  
  # the algorithm works by shrinking the range around the area with
  # a maximimum difference in the number of active cells
  initial_step = 0.1 # search radius is 5 * step size
  final_step = 0.002
  step_shrink = 2
  pc_guess = 0.5
  
  step_size = initial_step
  while step_size > final_step:
    p_vals = arange(pc_guess - 5 * step_size, pc_guess + 5 * step_size, step_size)
    frac_active = zeros(shape(p_vals),float)
    for i in range(0,size(p_vals)):
      lgn.p = p_vals[i]
      lgn.reset_wave()
      frac_active[i] = lgn.fraction_active()
      #fix
      if isnan(frac_active[i]):
        frac_active[i] = 0.2
        
        
    print pc_guess, step_size
    print frac_active    
    
    # now find the greatest difference between frac_active's
    diff = frac_active[1:] - frac_active[0:-1]
    max_index = argmax(diff)
    pc_guess = p_vals[max_index] + step_size / 2
    
    # trying the logistic regression
    from scipy import stats
    y = - log ( 0.8 / (frac_active + 0.0001 - 0.2) - 1.0 )
    regress_stats = stats.linregress(p_vals, y)
    slope = regress_stats[0]
    intercept = regress_stats[1]
    print "inflection?", - intercept / slope
    
    step_size /= step_shrink
      
  # reinstating the preserved p value
  lgn.p = original_p
  
  return pc_guess