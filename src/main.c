#include<stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <dirent.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

#define MAX_IMAGES 3000
#define PI 3.14159265358979323846

struct img_data
{
  uint64_t phash;
  char name[512];
};

struct img_data images[MAX_IMAGES];
int img_count = 0;
double threshold = 0.34;

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



unsigned int hamming_dist(uint64_t hash1,uint64_t hash2)
{
    uint64_t x = hash1 ^ hash2;
    unsigned int dist =0;
    while(x){
    dist +=x &1;
    x>>=1;
    }
    return dist;
}


uint64_t phash(char path[])
{

  uint64_t hash = 0;
  int bit = 0;
  int width,height,channels;
  const int resize= 8;

  unsigned char *img=stbi_load(path,&width,&height,&channels,0);
  if(img==NULL)
  {
    printf("ERROR while loading");
    exit(1);
  }



 // printf("image loaded width of %dpx, a height of %dpx and %d channels\n",width,height,channels);
  //resizing image

   unsigned char *resized_img = malloc(resize*resize * channels); 
    if(!resized_img) {
    printf("unable to allocate memory for the rezie img\n");
    exit(1);
   }

  //resizing the image to 32*32
   stbir_resize_uint8(img, width, height, 0,
                       resized_img, resize,resize, 0,
                       channels);

  size_t gray_img_size=resize*resize;
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
//stbi_write_png("images/out_gray8x8.png", resize, resize, 1, gray_img, resize);
//printf("Resized grayscale image saved to images/out_gray8x8.png\n");



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


  //extracting low frequecy co-eff values form dct 2
  int dct_size = 8;
  double low_freq[dct_size][dct_size];
  for(int i=0;i<dct_size;i++){
    for(int j=0;j<dct_size;j++){
      low_freq[i][j]=dct_output[i][j];
    }
  }


 // Optional: print extracted low-frequency DCT block
//    printf("\nTop-left 8x8 Low-Frequency DCT Block:\n");
//    for (int i = 0; i < dct_size; i++) {
//        for (int j = 0; j < dct_size; j++) {
//            printf("%8.2f ", low_freq[i][j]);
//        }
//        printf("\n");
//  }
 

  //computing the average value from the 63 values of low frequecy dct
    double sum = 0.0;
    int count = 0;
    
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 0 && j == 0) continue;  // skip DC coefficient
            sum += low_freq[i][j];
            count++;
        }
    }
    
    double mean = sum / count;
   // printf("Mean of 63 DCT values (excluding DC): %.2f\n", mean);
    
    // Input: double dct[8][8], double mean
    // Output: 64-bit hash as a bitstring
    
    for (int i = 0; i < 8; i++)
       {
        for (int j = 0; j < 8; j++)
           {
            if (i == 0 && j == 0) continue;  // skip DC if you want
            if (low_freq[i][j] < mean) {
                hash |= (1ULL << bit);  // set bit to 1 if value < mean
            }
            // else leave bit as 0 (default)
            bit++;
         }
      }

    // Clean up
    stbi_image_free(img);
    free(resized_img);
    free(gray_img);
    return hash;
}


void query_mode(char* filepath,int images_count)
{
  uint64_t target_hash=phash(filepath);
            for (int i=0; i < img_count; i++) 
              {
                 unsigned int dist = hamming_dist(target_hash, images[i].phash);
                 double normalized = dist / 63.0; double percentage = (1.0 - normalized) * 100.0;
                    if (normalized < threshold) 
                       {
                          printf("Match: %s   (Hamming: %u, Normalized: %.3f, Percentage: %.2f%%)\n",
                          images[i].name,  dist, normalized, percentage);
                       }
              }

}


int load_dir(char* dir_path)
{
    //opening the dir
    DIR *dir;
    struct dirent *entry;
    dir=opendir(dir_path);

    printf("Generating image_hash...\n"); 
    if (dir==NULL){return 1;}
        while((entry=readdir(dir))!=NULL)
        {
          if(entry->d_type==DT_REG){
          //printf("%s\n",entry->d_name);
             char filepath[512];
              snprintf(filepath, sizeof(filepath), "%s/%s", dir_path, entry->d_name);
              uint64_t hash = phash(filepath);
              images[img_count].phash = hash;
              strncpy(images[img_count].name, entry->d_name, sizeof(images[img_count].name) - 1);
              images[img_count].name[sizeof(images[img_count].name) - 1] = '\0';
              img_count++;
              printf(" %d %s loaded....\n", img_count,entry->d_name);
            }
        }
   if(closedir(dir)==-1)
    {
         printf("error closing dir\n");  
         return 1;
     }
return 0;
}


int main(int argc,char *argv[]) {

    if(argc>2 && strcmp(argv[1],"-q")==0)
      {
         printf("Query mode!\n");
         char *query_path = argv[2];
         char *dir_path = argv[3];
         printf("[Query image: %s]\n", query_path);
         printf("Search directory: %s\n", dir_path);
        load_dir(dir_path);
         query_mode(query_path,img_count);
      }

    return 0;
}

