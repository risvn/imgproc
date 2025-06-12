#include<stdio.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"




//applying dct transform
#include <math.h>

#define PI 3.14159265358979323846
// DCT function generalized for NxN
void dct_2d(int N, double input[N][N], double output[N][N]) {
    for (int u = 0; u < N; u++) {
        for (int v = 0; v < N; v++) {
            double sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    sum += input[x][y] *
                        cos(((2 * x + 1) * u * PI) / (2.0 * N)) *
                        cos(((2 * y + 1) * v * PI) / (2.0 * N));
                }
            }
            double cu = (u == 0) ? 1.0 / sqrt(2.0) : 1.0;
            double cv = (v == 0) ? 1.0 / sqrt(2.0) : 1.0;
            output[u][v] = 0.25 * cu * cv * sum;
        }
    }
}



int main(void){
  int width,height,channels;
  unsigned char *img=stbi_load("images/img1.png",&width,&height,&channels,0);
  if(img==NULL) {
    printf("ERROR while loading");
    exit(1);
  }
  printf("image loaded width of %dpx, a height of %dpx and %d channels\n",width,height,channels);
  //resizing image
   const int resize= 8;
   unsigned char *resized_img = malloc(resize*resize * channels); 

    if(!resized_img) {
    printf("unable to allocate memory for the rezie img\n");
    exit(1);
   }

  //resizing the image to 32*32
   stbir_resize_uint8(img, width, height, 0,
                       resized_img, resize,resize, 0,
                       channels);

  int gray_channels=4?2:1;      // ternary -> condition ? value_if_true : value_if_false;
  size_t gray_img_size=resize*resize*gray_channels;
  unsigned char *gray_img=malloc(gray_img_size);
  if(!gray_img) {
    printf("unable to allocat memory for the gray IMAGE. \n");
    exit(1);
  }

 for (int i = 0; i <resize * resize; ++i) {
        int r = resized_img[i * channels + 0];
        int g = resized_img[i * channels + 1];
        int b = resized_img[i * channels + 2];
        gray_img[i] = (uint8_t)((r + g + b) / 3);
    }



 // Write grayscale output image
stbi_write_png("images/out_gray32x32.png", resize, resize, 1, gray_img, resize);
printf("Resized grayscale image saved to images/out_gray32x32.png\n");



//converting gry-img in to the matrix
 double matrix[resize][resize];
  for (int i = 0; i < resize; i++) {
      for (int j = 0; j < resize; j++) {
          matrix[i][j] = (double)gray_img[i * resize + j];
      }
  }

    




    // Apply DCT
    double dct_output[resize][resize];
    dct_2d(resize, matrix, dct_output);




  
    // Print DCT Matrix
    printf("\nDCT Matrix (%dx%d):\n", resize, resize);
    for (int i = 0; i < resize; i++) {
        for (int j = 0; j < resize; j++) {
            printf("%8.2f ", dct_output[i][j]);
        }
        printf("\n");
    }

    // Clean up
    stbi_image_free(img);
    free(resized_img);
    free(gray_img);

    return 0;
}

