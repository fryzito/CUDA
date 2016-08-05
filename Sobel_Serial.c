#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h> 
#include "mypgm.h"
#define sqr(x) (x)*(x)
#define min(a,b)	      \
  ({ __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b);  \
    _a < _b ? _a : _b; })

void sobel_filtering( )
{
  /* Definicion de la matriz de sobel Horizontal*/
  int weight_h[3][3] = {{ -1,  0,  1 },
			{ -2,  0,  2 },
			{ -1,  0,  1 }};

  int weight_v[3][3] = {{ -1,  -2,  -1 },
			{ 0,  0,  0 },
			{ 1,  2,  1 }};
  double Gx,Gy;
  double min, max;
  int x, y, i, j;  /* Loop variable */
 
  /* Calculando los valores maximos y minimos de la imagen*/
  printf("Hallando el filtro de la imagen de entrada\n\n");
  min = DBL_MAX;
  max = -DBL_MAX;
  for (y = 1; y < y_size1 - 1; y++) {
    for (x = 1; x < x_size1 - 1; x++) {
      Gy = 0.0;
      for (j = -1; j <= 1; j++) {
	for (i = -1; i <= 1; i++) {
	  Gy += weight_v[j + 1][i + 1] * image1[y + j][x + i];
	}
      }
      if (Gy < min) min = Gy;
      if (Gy > max) max = Gy;
    }
  }
  if ((int)(max - min) == 0) {
    printf("No existe nada!!!\n\n");
    exit(1);
  }

  /* Inicializacion de la matriz image2[y][x] */
  x_size2 = x_size1;
  y_size2 = y_size1;
  for (y = 0; y < y_size2; y++) {
    for (x = 0; x < x_size2; x++) {
      image2[y][x] = 0;
    }
  }
  /* Generacion de Image2 despues de la inicializacion */
  for (y = 1; y < y_size1 - 1; y++) {
    for (x = 1; x < x_size1 - 1; x++) {
      Gx = 0.0;
      Gy = 0.0;
      for (j = -1; j <= 1; j++) {
	for (i = -1; i <= 1; i++) {
	  Gx += weight_h[j + 1][i + 1] * image1[y + j][x + i];
	  Gy += weight_v[j + 1][i + 1] * image1[y + j][x + i];
	}
      }
      Gx = MAX_BRIGHTNESS * (Gx - min) / (max - min);
      Gy = MAX_BRIGHTNESS * (Gy - min) / (max - min);
      image2[y][x] = (unsigned char) abs(Gx) + abs(Gy);
    }
  }
}

main( )
{
  load_image_data( );   /* Leer entrada en image1*/
  sobel_filtering( );   /* Aplicando el filtro de Sobel a la imagen1*/
  save_image_data( );   /* Salida de image2 */
  return 0;
}
