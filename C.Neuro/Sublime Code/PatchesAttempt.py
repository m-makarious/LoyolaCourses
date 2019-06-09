# Patches attempt from ChiPy talk
# Meant to collect patches and normalize the data

import PIL.Image

# Collect patches from perculations
def collect_patches(num_patches = 100000, patch_width = 8):
    max_tries = num_patches * 50
    image_width = 200
    
    img_first_patch = 0 # the first patch number accepted from an image
    img_first_try = 0 # the first attempt to take a patch from the image
    patch_count = 0 # number of collected patches
    try_count = 0 # number of attempted collected patches
    num_pixels = patch_width * patch_width
    
    patch_sample = np.zeros([patch_width,patch_width],float)
    patch = np.zeros([num_pixels,1],float)
    
    image_patches = np.zeros([num_pixels, num_patches],float)

  # Change the image sampled from
    #image_count = 1  
    image = PIL.Image.open('fig1.png')
    image = np.asarray(image, 'double').transpose() 
    
  # Normalizing the image
    image -= image.mean()
    image /= image.std()
      
  # Collect the patches
    while patch_count < num_patches and try_count < max_tries:
        try_count += 1  # Number of total patches attempted

    if (try_count - img_first_try) > max_tries/12 or \
      (patch_count - img_first_patch) > num_patches/12:

    # Change the image sampled from
        #image_count += 1
        image = PIL.Image.open('fig1.png')
        image = np.asarray(image, 'double').transpose()      

    # Normalizing the image
        image -= image.mean()
        image /= image.std()
        
        img_first_patch = patch_count
        img_first_try = try_count
    
    # Update on every switch of images
        #print (int(100 * float(patch_count)/num_patches),'% percent complete')
    
    px = np.random.randint(0, image_width - patch_width)
    py = np.random.randint(0, image_width - patch_width)
         
    patch_sample = image[px:px+patch_width, py:py+patch_width].copy()
    patch_std = patch_sample.std()
    
    if patch_std > 0.0: # > 0 to remove blank/uninteresting patches for speed
    # Create the patch vector     
        patch = np.reshape(patch_sample, num_pixels)     
        patch = patch - np.mean(patch)
        image_patches[:,patch_count] = patch.copy()
        patch_count += 1
    return image_patches
        
patches_mat = collect_patches(num_patches = 100000, patch_width = 8)
print('\nshape of the extracted image patch data:', patches_mat.shape)