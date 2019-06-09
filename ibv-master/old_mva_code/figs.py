from numpy import *
import pylab
import LGN
import LGNTools
import pickle
import FilterTools
import V1Tools

def fig1_collect():
  """ this function collects data for a plot of p_c vs t, for different values
  of trans, the resulting matrix is stored in results/fig1data.txt """
  window_size = 50
  r = 3
  lgn = LGN.LGN(window_size, 0.5, r)
  t = range(1,11)
  #fix
  trans = arange(0,5) * 0.02
  pc = zeros([size(t), size(trans)], float)
  display_count = 0
  for t_i in range(0,size(t)):
    for trans_i in range(0,size(trans)):
      # display progress
      print float(display_count)/(size(t) * size(trans)), t[t_i], trans[trans_i]
      display_count += 1 
      lgn.t = t[t_i]
      lgn.trans = trans[trans_i]
      pc[t_i,trans_i] = LGNTools.find_pc(lgn)
  # saving all the output
  outputfile = open ( 'results/fig1data.txt', 'wb' ) 
  fig1data = {"t":t, "trans":trans, "pc":pc, 'r':r, 'window_size':window_size}
  pickle.dump(fig1data, outputfile)
  outputfile.close()
  
def fig1_display(datafilename = 'results/fig1data.txt'):
  """ this function plots the data collected in fig1_collect """
  
  inputfile = open (datafilename, 'rb')
  fig1data = pickle.load(inputfile)
  inputfile.close()
  pylab.plot(fig1data['t'],fig1data['pc'])
  pylab.savefig('results/fig1.png',dpi=300)

def fig2_collect():
  trans = [0, 0.05, 0.2, 0.8]
  p = [0.55, 0.50, 0.45, 0.35]
  r = 1
  t = 1
  filters = []
  bases = []
  patches = []
  lgn = LGN.LGN(r=r,t=t, width=100)  
  for i in range(len(trans)):
    lgn.p = p[i]
    lgn.trans = trans[i]
    print "collecting patches..."
    data_patches = LGNTools.collect_patches(lgn, 5000, 10)
    patches.append(data_patches)
    """
    print "finding filters..."
    filter_data = LGNTools.find_filters(data_patches)
    filters.append(filter_data['ica_filters'])
    bases.append(filter_data['ica_bases'])
    """
  # save the data    
  outputfile = open ( 'results/fig2data.txt', 'wb' ) 
  fig2data = {"p":p, "r":r, "t":t, 'trans':trans, 'patches':patches}
    #,'ica_filters':filters, 'ica_bases':bases}
  pickle.dump(fig2data, outputfile)
  outputfile.close()

def fig2_display(datafilename = 'results/fig2data.txt'):
  """ this function creates a plot of various LGN correlations and
      the resulting disparity profiles """
  inputfile = open (datafilename, 'rb')
  data = pickle.load(inputfile)
  inputfile.close()
  lgn = LGN.LGN(r=data['r'],t=data['t'], make_wave=False)
  pylab.figure(2)
  pylab.clf()
  pylab.bone()
  p = data['p']
  trans = data['trans']
  for i in range(len(trans)):
    lgn.p = p[i]
    lgn.trans = trans[i]
    lgn.reset_wave()
    
    # the LGN wave plot
    pylab.subplot(len(trans),3,3*i+1)
    pylab.imshow(lgn.make_img_mat(), interpolation='nearest')
    pylab.title("p=" + str(p[i]) + ", trans=" + str(trans[i]) + ", corr=" + 
      str(lgn.correlation()) )
    
    # plotting the patches
    pylab.subplot(len(trans),3,3*i+2)
    pylab.imshow(FilterTools.show_patches_mat(data['patches'][i],
      6).transpose(), interpolation='nearest')

    """
    # plotting the filters
    pylab.subplot(len(trans),2, 2*i+1)
    pylab.imshow(FilterTools.show_patches_mat(data['ica_filters'][i], 8))
    """
    pylab.subplot(len(trans),3, 3*i+3)
    disp, weight = FilterTools.get_disparities(data['patches'][i][:,:])
    pylab.bar(disp[1:-1]-0.4, weight[1:-1]/5000)
    pylab.ylim(0,1)
    #pylab.imshow(FilterTools.get_disparity_mat(data['patches'][i][:,:100], True))
    
def fig3_display():
  """ displays a theoretical plot for how the pc line varies with t and trans """
  r = 3
  trans = arange(0,21)*0.05
  t = range(1,11)
  pc = zeros([len(t),len(trans)],'float')
  for t_i in range(len(t)):
    for trans_i in range(len(trans)):
      pc[t_i,trans_i] = t[t_i] / (pi * r**2 * (1 + trans[trans_i]) / 2)
  pylab.plot(t,pc)
  
def noise_images():
  """ this figure shows thresholded noise images at a number of thresholds """
  import magiceye
  pylab.figure()
  cnt = 0;
  for l in [2, 8, 64]:  
    for f in [1, 1.6, 2.2, 4]:
      cnt += 1
      pylab.subplot(3,4,cnt)
      pylab.imshow(magiceye.threshold_noise(f,levels=l))
      pylab.axis('off')
      pylab.title('f = ' + str(f) + ', lvls = ' + str(l))

def noise_collect():
  import magiceye
  levels = [2, 8, 64]
  f_exp = [1, 1.6, 2.2, 4]
  filters = []
  bases = []
  patches = []
  for l in levels:
    for f in f_exp:
      print "collecting patches..."
      data_patches = magiceye.collect_noise_patches(20000, 16, downsample=2, f_exp = f, levels = l)
      patches.append(data_patches)

      print "collected f=", f, " and levels=", l
      print "finding filters..."
      filter_data = LGNTools.find_filters(data_patches, 100)
      filters.append(filter_data['ica_filters'])
      bases.append(filter_data['ica_bases'])

    # save the data    
    outputfile = open ( 'results/noise_fig.dat', 'wb' ) 
    noise_fig_data = {"f_exp":f_exp, "levels":levels, 'patches':patches,'ica_filters':filters, 'ica_bases':bases}
    pickle.dump(noise_fig_data, outputfile)
    outputfile.close()

def noise_display(datafilename = 'results/noise_fig.dat'):
  """ this function creates a plot showing various noise images and bases """
  
  inputfile = open (datafilename, 'rb')
  data = pickle.load(inputfile)
  inputfile.close()
  
  f_exp = data['f_exp']
  levels = data['levels']  
  
  # plotting the filters
  pylab.figure()
  pylab.bone()
  i = -1
  for l in range(len(levels)):
    for f in range(len(f_exp)):
      i += 1
      # plotting the filters
      pylab.subplot(len(levels),len(f_exp), i+1)
      pylab.imshow(FilterTools.show_patches_mat(data['ica_filters'][i], 9, num_layers=1))
      pylab.axis('off')      

def fractal_dims():
  """ this function prints the fractal dimension with confidence intervals for a set of threshold noise f values """
  f_exp = [1, 1.6, 2.2, 4]
  
  import magiceye
  num_images = 20
  dims = zeros(num_images)
  for f in f_exp:
    for img_cnt in range(num_images):
      dims[img_cnt] = magiceye.boxcount_dim(magiceye.threshold_noise(f=f, image_width=256), start_size=1)
    print dims
    print "confidence interval (with n=",num_images,") for exp=", f, "is", mean(dims), "+-", 1.96*std(dims)

def multiscale_analysis(natural=False):
  """ produces an analysis of multiscale filter properties """
  nat_img_directory = '/Users/mva/bismuth/active/vcasa/data/natural_images'

  print "collecting patches..."
  import magiceye
  if natural:
    print "... from natural images"
    data_patches = FilterTools.collect_natural_patches(nat_img_directory,20000, 16, downsample=2)
  else:
    data_patches = zeros((256, 20000))
    data_patches[:,0:8000] = magiceye.collect_noise_patches(8000, 16, downsample=2, f_exp = 1.6, levels = 2)
    data_patches[:, 8000:] = magiceye.collect_noise_patches(12000, 16, downsample=2, f_exp = 4, levels = 2)
    
  print "finding filters..."
  filter_data = LGNTools.find_filters(data_patches, 100)
  icafilters = filter_data['ica_filters']
  icabases = filter_data['ica_bases']  
  FilterTools.show_patches_mat(icafilters, 100, num_layers=1)  
  pylab.figure()
  FilterTools.show_patches_mat(icabases, 100, num_layers=1)    
  
  