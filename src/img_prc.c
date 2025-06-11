#include<stdio.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

int main(void){
  int width,height,channels;
  unsigned char *img=stbi_load("images/img1.png",&width,&height,&channels,0);
  if(img==NULL) {
    printf("ERROR while loading");
    exit(1);
  }
  printf("image loaded width of %dpx, a height of %dpx and %d channels\n",width,height,channels);
  //resizing image
   const int resized_w = 32, resized_h = 32;
   unsigned char *resized_img = malloc(resized_w * resized_h * channels); 

    if(!resized_img) {
    printf("unable to allocate memory for the rezie img\n");
    exit(1);
   }

  //resizing the image to 32*32
   stbir_resize_uint8(img, width, height, 0,
                       resized_img, resized_w, resized_h, 0,
                       channels);

  int gray_channels=4?2:1;      // ternary -> condition ? value_if_true : value_if_false;
  size_t gray_img_size=resized_w*resized_h*gray_channels;
  unsigned char *gray_img=malloc(gray_img_size);
  if(!gray_img) {
    printf("unable to allocat memory for the gray IMAGE. \n");
    exit(1);
  }

 for (int i = 0; i < resized_w * resized_h; ++i) {
        int r = resized_img[i * channels + 0];
        int g = resized_img[i * channels + 1];
        int b = resized_img[i * channels + 2];
        gray_img[i] = (uint8_t)((r + g + b) / 3);
    }

  stbi_write_png("images/out_gray32x32.png", resized_w, resized_h, 1, gray_img, resized_w);

    printf(" Resized and grayscale image saved as images/gray32x32.png\n");

    stbi_image_free(img);
    free(resized_img);
    free(gray_img);

    return 0;
}

