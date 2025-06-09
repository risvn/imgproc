#include<stdio.h>
#include<stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

int main(void){
  int width,height,channels;
  unsigned char *img=stbi_load("images/img1.png",&width,&height,&channels,0);
  if(img==NULL) {
    printf("ERROR while loading");
    exit(1);
  }
  printf("image loaded width of %dpx, a height of %dpx and %d channels\n",width,height,channels);
  //manipulating pixel data
  size_t img_size=width * height * channels;
  // ternary -> condition ? value_if_true : value_if_false;
  int gray_channels=4?2:1;
  size_t gray_img_size=width * height * gray_channels;

  unsigned char *gray_img=malloc(gray_img_size);
  if(!gray_img) {
    printf("unable to allocat memory for the gray IMAGE. \n");
    exit(1);
  }
  for(unsigned char *pi = img,*pg=gray_img;pi!=img+img_size;pi+=channels,pg+=gray_channels)
    {
      //gry_img:1byte RGBA_img:4byts  1 byt->4 bytes avg
      *pg=(uint8_t)((*pi+*(pi+1)+*(pi+2))/3.0);

      if (channels == 4 && gray_channels == 2) {
        *(pg+1)=*(pi+3);
        }
  }



  //takes the raw pixel buffer and makes in to the .png 
  //stride_in_bytes=width * channels (no.of bytes between each row )
  stbi_write_png("images/output.png", width, height, gray_channels, gray_img, width * gray_channels);
  stbi_image_free(img);

}



