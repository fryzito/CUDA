#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include "mypgm.h"

#define WIN_SIZE 3
#define N 512//16384



__global__ void kernel(unsigned char i[][N],unsigned char Img[][N],double mini,double maxi)
{
  printf("Primer Kernel%uc\n",i[0][0]);
  int dx=blockIdx.x*blockDim.x+threadIdx.x;
  int dy=blockIdx.y*blockDim.y+threadIdx.y;
  //printf("dx=(%d,%d,%d) dy=(%d,%d,%d)\n",blockIdx.x,blockDim.x,threadIdx.x,blockIdx.y,blockDim.y,threadIdx.y);
  double p = 0.0;  
  //  printf("%d %d\n",dx,dy);
  if(dx != 0 && dy != 0) {
    p += -1 * i[dx-1][dy-1];
    p += 0  * i[dx-1][dy];
    p += 1  * i[dx-1][dy+1];
    p += -2 * i[dx][dy-1];
    p += 0  * i[dx][dy];
    p += 2  * i[dx][dy+1];
    p += -1 * i[dx+1][dy-1];
    p += 0  * i[dx+1][dy];
    p += 1  * i[dx+1][dy+1];
    
    p = (double)MAX_BRIGHTNESS * (double)(p - mini) / (double)(maxi - mini);
    //  printf("%lf\n",p);
    //printf("%lf\n",p);
    Img[dy][dx] = (unsigned char)p;
  }
  
  
}



void move(unsigned char *a){

  for(int i=0;i<MAX_IMAGESIZE;i++){
    for(int j=0;j<MAX_IMAGESIZE;j++){
      image2[i][j]=0;
    }
  }
  
  for(int i=0;i<512;i++){
    for(int j=0;j<512;j++){
      image2[i][j]=a[i*N + j];
    }
  }
}

void getMaxMin(double &min,double &max){
  int weight[3][3] = {{ -1,  0,  1 },
                      { -2,  0,  2 },
                      { -1,  0,  1 }};
  double pixel_value;
  int x, y, i, j;  /* Loop variable */
  /* Maximum values calculation after filtering*/
  printf("Se esta procediendo a hallar la matriz\n\n");
  min = DBL_MAX;
  max = -DBL_MAX;
  for (y = 1; y < y_size1 - 1; y++) {
    for (x = 1; x < x_size1 - 1; x++) {
      pixel_value = 0.0;
      for (j = -1; j <= 1; j++) {
        for (i = -1; i <= 1; i++) {
          pixel_value += weight[j + 1][i + 1] * image1[y + j][x + i];
        }
      }
      if (pixel_value < min) min = pixel_value;
      if (pixel_value > max) max = pixel_value;
    }
  }
  if ((int)(max - min) == 0) {
    printf("No existe el archivo!!!\n\n");
    exit(1);
  }
}

int main(void){
  
  load_image_data( ); 

  ////////////////////////////////////////////////////
  // image1[][];
  // Reservar memoria en GPU
  unsigned char (*pA)[N],(*psobel)[N];
  int (*w)[3];
  cudaMalloc((void**)&pA,(N*N)*sizeof(unsigned char));
  cudaMalloc((void**)&psobel,(N*N)*sizeof(unsigned char));
  cudaMalloc((void**)&w,(N*N)*sizeof(int));

  // Mover a device
  cudaMemcpy(pA, image1, (N*N)*sizeof(unsigned char),cudaMemcpyHostToDevice);


  const dim3 dimGrid(4,4);
  const dim3 dimBlock(16,16);
  double min=0,max=0;
  getMaxMin(min,max);
  printf("main %uc\n",image1[0][0]);
  kernel<<<dimGrid,dimBlock>>>(pA,psobel,min,max);
  printf("salio kernel\n");

  cudaDeviceSynchronize();
  unsigned char* image3 =(unsigned char*)malloc(N*N*sizeof(unsigned char));
  cudaMemcpy(image3, psobel, (N*N)*sizeof(unsigned char),cudaMemcpyDeviceToHost);
  cudaDeviceSynchronize();

  move(image3);
  // printf("0000000000000000000000000000000000000========================");
  //  for(int i=0;i<64;i++){
  //    for(int j=0;j<64;j++){
  //      printf("xx%d ",(int)image3[i*N + j]);
  //    }
  //  }
  
  //free resources
  cudaFree(pA); 
  cudaFree(psobel);
  
  //////////////////////////////////////////////////////
  
 
  x_size2 = x_size1;
  y_size2 = y_size1;
  save_image_data( ); 
  
  return 0;
}
