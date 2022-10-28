#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define BAD_NUMBER_ARGS 1
#define FSEEK_ERROR 2
#define FREAD_ERROR 3
#define MALLOC_ERROR 4
#define FWRITE_ERROR 5

/**
 * Parses the command line.
 *
 * argc:      the number of items on the command line (and length of the
 *            argv array) including the executable
 * argv:      the array of arguments as strings (char* array)
 * grayscale: the integer value is set to TRUE if grayscale output indicated
 *            outherwise FALSE for threshold output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *isGrayscale)
{
  if (argc > 2)
  {
    printf("Usage: %s [-g]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 && strcmp(argv[1], "-g") == 0)
  {
    *isGrayscale = TRUE;
  }
  else
  {
    *isGrayscale = FALSE;
  }

  return stdin;
}

unsigned getFileSizeInBytes(FILE *stream)
{
  unsigned fileSizeInBytes = 0;

  rewind(stream);
  if (fseek(stream, 0L, SEEK_END) != 0)
  {
    exit(FSEEK_ERROR);
  }
  fileSizeInBytes = ftell(stream);

  return fileSizeInBytes;
}

void getBmpFileAsBytes(unsigned char *ptr, unsigned fileSizeInBytes, FILE *stream)
{
  rewind(stream);
  if (fread(ptr, fileSizeInBytes, 1, stream) != 1)
  {
#ifdef DEBUG
    printf("feof() = %x\n", feof(stream));
    printf("ferror() = %x\n", ferror(stream));
#endif
    exit(FREAD_ERROR);
  }
}

unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red)
{
  /*printf("TODO: unsigned char getAverageIntensity(unsigned char blue, unsigned char green, unsigned char red)\n");*/

  unsigned int average = ((unsigned int)blue + (unsigned int)green + (unsigned int)red) / 3;

  return (unsigned char)average;
}

void applyGrayscaleToPixel(unsigned char *pixel)
{
  /*printf("TODO: void applyGrayscaleToPixel(unsigned char* pixel)\n");*/

  unsigned char avg = getAverageIntensity(*(pixel), *(pixel + 1), *(pixel + 2));

  *(pixel) = avg;
  *(pixel + 1) = avg;
  *(pixel + 2) = avg;
}

void applyThresholdToPixel(unsigned char *pixel)
{

#ifdef DEBUG
  /*printf("%x %x %x => %d\n", *(pixel), *(pixel +1), *(pixel +2), getAverageIntensity(*(pixel), *(pixel + 1), *(pixel + 2)));*/

#endif

  /*printf("TODO: void applyThresholdToPixel(unsigned char* pixel)\n");*/

  if (getAverageIntensity(*(pixel), *(pixel + 1), *(pixel + 2)) < 128)
  {
    *(pixel) = 0x00;
    *(pixel + 1) = 0x00;
    *(pixel + 2) = 0x00;
  }
  else
  {
    *(pixel) = 0xFF;
    *(pixel + 1) = 0xFF;
    *(pixel + 2) = 0xFF;
  }
}

void applyFilterToPixel(unsigned char *pixel, int isGrayscale)
{

  /*printf("TODO: void applyFilterToPixel(unsigned char* pixel, int isGrayscale)\n");*/

  if (isGrayscale)
  {
    applyGrayscaleToPixel(pixel);
  }
  else
  {
    applyThresholdToPixel(pixel);
  }
}

void applyFilterToRow(unsigned char *row, int width, int isGrayscale)
{

  /*printf("TODO: void applyFilterToRow(unsigned char* row, int width, int isGrayscale)\n");*/

  /*#ifdef DEBUG
  printf("\n");
  for(int i = 0; i < 60; i++){
    printf("%02x ", *(row + i));
  }
  printf("Before %p \n", row);
#endif
*/

  for (int i = 0; i < width; i++)
  {
    applyFilterToPixel((row + (i * 3)), isGrayscale);
  }

  /*#ifdef DEBUG
  printf("\n");
  for(int i = 0; i < 60; i++){
    printf("%02x ", *(row +i));
  }
  printf("After  %p \n", row);
#endif*/
}

void applyFilterToPixelArray(unsigned char *pixelArray, int width, int height, int isGrayscale)
{
  int padding = 0;
  /*printf("TODO: compute the required amount of padding from the image width");*/

  padding = (!((width * 3) % 4)) ? 0 : 4 - ((width * 3) % 4);
  int rowWidth = (width * 3) + padding;

  for (int i = 0; i < height; i++)
  {
    applyFilterToRow((pixelArray + (i * rowWidth)), width, isGrayscale);
  }

#ifdef DEBUG
  /*printf("\n");
  for(int j = 0; j < 20; j++){
    printf("%02x ", *(pixelArray + j + (i * rowWidth)));
  }
  printf(" rowW: %d   %p \n", (i * rowWidth), (pixelArray + (i * rowWidth)));
  */
  printf("padding: (%d * 3) = %d, %d mod 4 = %d should equal: %d\n", width, (width * 3), (width * 3), ((width * 3) % 4), padding);
#endif

  /*printf("TODO: void applyFilterToPixelArray(unsigned char* pixelArray, int width, int height, int isGrayscale)\n");*/
}

void parseHeaderAndApplyFilter(unsigned char *bmpFileAsBytes, int isGrayscale)
{
  int offsetFirstBytePixelArray = *(bmpFileAsBytes + 10);
  int width = *((int *)(bmpFileAsBytes + 18));
  int height = *((int *)(bmpFileAsBytes + 22));
  unsigned char *pixelArray = (bmpFileAsBytes + offsetFirstBytePixelArray);

  /*printf("%u\n", *( bmpFileAsBytes + 22));*/

  /*printf("TODO: set offsetFirstBytePixelArray\n");
  printf("TODO: set width\n");
  printf("TODO: set height\n");
  printf("TODO: set the pixelArray to the start of the pixel array\n");
  */

#ifdef DEBUG
  printf("offsetFirstBytePixelArray = %u\n", offsetFirstBytePixelArray);
  printf("width = %u\n", width);
  printf("height = %u\n", height);
  printf("pixelArray = %p\n", pixelArray);

  for (int i = 0; i < 20; i++)
  {
    printf("%04d ", *(bmpFileAsBytes + 14 + i));
  }

  printf("\n");

/*  for(int j = 0; j < 20; j++){
    for(int i = 0; i < 20; i++){
      printf("%x ", *(pixelArray + i + (20 * j)));
    }

    printf("  %p \n", (pixelArray + (j * 20)));

  }

  printf("\n \n");*/
#endif

  applyFilterToPixelArray(pixelArray, width, height, isGrayscale);
}

int main(int argc, char **argv)
{
  int grayscale = FALSE;
  unsigned fileSizeInBytes = 0;
  unsigned char *bmpFileAsBytes = NULL;
  FILE *stream = NULL;

  stream = parseCommandLine(argc, argv, &grayscale);
  fileSizeInBytes = getFileSizeInBytes(stream);

#ifdef DEBUG
  printf("fileSizeInBytes = %u\n", fileSizeInBytes);
#endif

  bmpFileAsBytes = (unsigned char *)malloc(fileSizeInBytes);
  if (bmpFileAsBytes == NULL)
  {
    exit(MALLOC_ERROR);
  }
  getBmpFileAsBytes(bmpFileAsBytes, fileSizeInBytes, stream);

  parseHeaderAndApplyFilter(bmpFileAsBytes, grayscale);

#ifndef DEBUG
  if (fwrite(bmpFileAsBytes, fileSizeInBytes, 1, stdout) != 1)
  {
    exit(FWRITE_ERROR);
  }
#endif

  free(bmpFileAsBytes);
  return 0;
}
