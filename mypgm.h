/* pgm file IO headerfile ------ mypgm.h */

/* Constant declaration */
#define MAX_IMAGESIZE   /*32768*/16384//8192//4096//1024
#define MAX_BRIGHTNESS  255 /* Maximum gray level */
#define GRAYLEVEL       256 /* No. of gray levels */
#define MAX_FILENAME    256 /* Filename length limit */
#define MAX_BUFFERSIZE  256

/* Declaracion de variables globales */
/* Arreglos para guardar las imagenes */
unsigned char image1[MAX_IMAGESIZE][MAX_IMAGESIZE],
  image2[MAX_IMAGESIZE][MAX_IMAGESIZE];
int x_size1, y_size1, /* width & height of image1*/
  x_size2, y_size2; /* width & height of image2 */

/* Prototype declaration of functions */
void load_image_data( ); /* image input */
void save_image_data( ); /* image output*/
void load_image_file(char *); /* image input */
void save_image_file(char *); /* image output*/


/* Main body of functions */

void load_image_data( )
/* Input of header & body information of pgm file */
/* for image1[ ][ ]，x_size1，y_size1 */
{
  char file_name[MAX_FILENAME];
  char buffer[MAX_BUFFERSIZE];
  FILE *fp; /* File pointer */
  int max_gray; /* Maximum gray level */
  int x, y; /* Loop variable */

  /* Abrir file de entrada */

  printf("-----------------------------------------------------\n\n");
  printf(" Solo archivos pgm son aceptados \n\n");
  printf("Nombre de la imagen de entrada ? (*.pgm) :");
  scanf("%s", file_name);
  fp = fopen(file_name, "rb");
  if (NULL == fp) {
    printf("   El Archivo no existe!\n\n");
    exit(1);
  }
  /* Comprobar que el archivo es del tipo P5 */
  fgets(buffer, MAX_BUFFERSIZE, fp);
  if (buffer[0] != 'P' || buffer[1] != '5') {
    printf("   El archivo leido no es del tipo P5!\n\n");
    exit(1);
  }
  /* input of x_size1, y_size1 */
  x_size1 = 0;
  y_size1 = 0;
  while (x_size1 == 0 || y_size1 == 0) {
    fgets(buffer, MAX_BUFFERSIZE, fp);
    if (buffer[0] != '#') {
      sscanf(buffer, "%d %d", &x_size1, &y_size1);
    }
  }
  /* input of max_gray */
  max_gray = 0;
  while (max_gray == 0) {
    fgets(buffer, MAX_BUFFERSIZE, fp);
    if (buffer[0] != '#') {
      sscanf(buffer, "%d", &max_gray);
    }
  }
  /* Display of parameters */
  printf("\n     Ancho de Imagen = %d, Alto de Imagen = %d\n", x_size1, y_size1);
  printf("    Nivel maximo de gris = %d\n\n",max_gray);
  if (x_size1 > MAX_IMAGESIZE || y_size1 > MAX_IMAGESIZE) {
    printf("   El tamaño de la imagen es muy grande %d x %d\n\n", 
	   MAX_IMAGESIZE, MAX_IMAGESIZE);
    printf("     Utilice una imagen mas pequeña!\n\n");
    exit(1);
  }
  if (max_gray != MAX_BRIGHTNESS) {
    printf("    Valor invalido de maximo nivel de gris!\n\n");
    exit(1);
  }
  /* Imagen de entrada */
  for (y = 0; y < y_size1; y++) {
    for (x = 0; x < x_size1; x++) {
      image1[y][x] = (unsigned char)fgetc(fp);
    }
  }
  printf("----- La Lectura de entrada de la Image Sin problemas -----\n\n");
  printf("-----------------------------------------------------\n\n");
  fclose(fp);
}

void save_image_data( )
/* Output of image2[ ][ ], x_size2, y_size2 in pgm format*/
{
  char file_name[MAX_FILENAME];
  FILE *fp; /* File pointer */
  int x, y; /* Loop variable */
  
  /* Abriendo el archivo de salida */

  printf("-----------------------------------------------------\n\n");
  printf("Nombre del file de salida ? (*.pgm) : ");
  scanf("%s",file_name);
  fp = fopen(file_name, "wb");
  /* output of pgm file header information */
  fputs("P5\n", fp);
  fputs("# Creado por el procesamiento de Imagen\n", fp);
  fprintf(fp, "%d %d\n", x_size2, y_size2);
  fprintf(fp, "%d\n", MAX_BRIGHTNESS);
  /* Imagen de salida*/
  for (y = 0; y < y_size2; y++) {
    for (x = 0; x < x_size2; x++) {
      fputc(image2[y][x], fp);
    }
  }
  printf("\n----- La Imagen de salida salio sin problemas ---\n\n");
  printf("-----------------------------------------------------\n\n");
  fclose(fp);
}

void load_image_file(char *filename)
/* Input of header & body information of pgm file */
/* for image1[ ][ ]，x_size1，y_size1 */
{
  char buffer[MAX_BUFFERSIZE];
  FILE *fp; /* File pointer */
  int max_gray; /* Maximum gray level */
  int x, y; /* Loop variable */
  
  /* Input file open */
  fp = fopen(filename, "rb");
  if (NULL == fp) {
    printf("    El Archivo no existe!\n\n");
    exit(1);
  }
  /* Check of file-type ---P5 */
  fgets(buffer, MAX_BUFFERSIZE, fp);
  if (buffer[0] != 'P' || buffer[1] != '5') {
    printf("    El archivo de entrada  no es P5 !\n\n");
    exit(1);
  }
  /* input of x_size1, y_size1 */
  x_size1 = 0;
  y_size1 = 0;
  while (x_size1 == 0 || y_size1 == 0) {
    fgets(buffer, MAX_BUFFERSIZE, fp);
    if (buffer[0] != '#') {
      sscanf(buffer, "%d %d", &x_size1, &y_size1);
    }
  }
  /* input of max_gray */
  max_gray = 0;
  while (max_gray == 0) {
    fgets(buffer, MAX_BUFFERSIZE, fp);
    if (buffer[0] != '#') {
      sscanf(buffer, "%d", &max_gray);
    }
  }
  if (x_size1 > MAX_IMAGESIZE || y_size1 > MAX_IMAGESIZE) {
    printf("     Imagen de salida excedio el tamaño permitido %d x %d\n\n", 
	   MAX_IMAGESIZE, MAX_IMAGESIZE);
    printf("     Utilice una imagen mas pequeña!\n\n");
    exit(1);
  }
  if (max_gray != MAX_BRIGHTNESS) {
    printf("    Valor invalido del nivel maximo de gris!\n\n");
    exit(1);
  }
  /* Input of image data*/
  for (y = 0; y < y_size1; y++) {
    for (x = 0; x < x_size1; x++) {
      image1[y][x] = (unsigned char)fgetc(fp);
    }
  }
  fclose(fp);
}

void save_image_file(char *filename)
/* Output of image2[ ][ ], x_size2, y_size2 */ 
/* into pgm file with header & body information */
{
  FILE *fp; /* File pointer */
  int x, y; /* Loop variable */
  
  fp = fopen(filename, "wb");
  /* output of pgm file header information */
  fputs("P5\n", fp);
  fputs("# Creado por el procesamiento de Imagen\n", fp);
  fprintf(fp, "%d %d\n", x_size2, y_size2);
  fprintf(fp, "%d\n", MAX_BRIGHTNESS);
  /* Output of image data */
  for (y = 0; y < y_size2; y++) {
    for (x = 0; x < x_size2; x++) {
      fputc(image2[y][x], fp);
    }
  }
  fclose(fp);
}
