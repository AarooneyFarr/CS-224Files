#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define TRUE 1
#define FALSE 0
#define BAD_NUMBER_ARGS 1

/**
 * Parses the command line.
 *
 * argc: the number of items on the command line (and length of the
 *       argv array) including the executable
 * argv: the array of arguments as strings (char* array)
 * bits: the integer value is set to TRUE if bits output indicated
 *       outherwise FALSE for hex output
 *
 * returns the input file pointer (FILE*)
 **/
FILE *parseCommandLine(int argc, char **argv, int *bits)
{
  if (argc > 2)
  {
    printf("Usage: %s [-b|-bits]\n", argv[0]);
    exit(BAD_NUMBER_ARGS);
  }

  if (argc == 2 &&
      (strcmp(argv[1], "-b") == 0 || strcmp(argv[1], "-bits") == 0))
  {
    *bits = TRUE;
  }
  else
  {
    *bits = FALSE;
  }

  return stdin;
}

/**
 * Writes data to stdout in hexadecimal.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/

void printDataAsHex(unsigned char *data, size_t size)
{

  //  printf(" ");

  for (int i = 0; i < size; i++)
  {
    int hexOne = data[i] / 16;
    int hexTwo = data[i] % 16;
    char hexList[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    int h1 = hexList[hexOne];
    int h2 = hexList[hexTwo];

    if ((i) % 2 == 0)
    {
      printf(" ");
    }

    printf("%c%c", h1, h2);
    //printf("  %02x  ", data[i]);
  }

  for (int i = 0; i < ((16 - size) * 2); i++)
  {
    printf(" ");
  }

  if ((16 - size) % 2 == 1)
  {
    for (int i = 0; i < ((16 - size - 1) / 2); i++)
    {
      printf(" ");
    }
  }
  else
  {
    for (int i = 0; i < ((16 - size) / 2); i++)
    {
      printf(" ");
    }
  }
}

/**
 * Writes data to stdout as characters.
 *
 * See myxxd.md for details.
 *
 * data: an array of no more than 16 characters
 * size: the size of the array
 **/
void printDataAsChars(unsigned char *data, size_t size)
{
  for (int i = 0; i < size; i++)
  {
    if (data[i] < 127 && data[i] > 31)
    {
      printf("%c", data[i]);
    }
    else
    {
      printf(".");
    }
  }
}

void readAndPrintInputAsHex(FILE *input)
{
  unsigned char data[16];
  int numBytesRead = fread(data, 1, 16, input);
  unsigned int offset = 0;
  while (numBytesRead != 0)
  {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsHex(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 16, input);
  }
}

void printDataAsBits(unsigned char *data, size_t size)
{

  //  printf(" ");

  for (int i = 0; i < size; i++)
  {
    char bit[8] = {'0', '0', '0', '0', '0', '0', '0', '0'};
    int tracker = data[i];

    for (int k = 7; k >= 0; k--)
    {

      bit[k] = (tracker % 2 == 1) ? '1' : '0';
      tracker /= 2;
    }

    if ((i) % 1 == 0)
    {
      printf(" ");
    }

    printf("%s", bit);
  }

  for (int i = 0; i < ((6 - size) * 9); i++)
  {
    printf(" ");
  }

  //printf("  ");
}

/**
 * Bits output for xxd.
 *
 * See myxxd.md for details.
 *
 * input: input stream
 **/
void readAndPrintInputAsBits(FILE *input)
{
  unsigned char data[6];
  int numBytesRead = fread(data, 1, 6, input);
  unsigned int offset = 0;
  while (numBytesRead != 0)
  {
    printf("%08x:", offset);
    offset += numBytesRead;
    printDataAsBits(data, numBytesRead);
    printf("  ");
    printDataAsChars(data, numBytesRead);
    printf("\n");
    numBytesRead = fread(data, 1, 6, input);
  }
}

int main(int argc, char **argv)
{
  int bits = FALSE;
  FILE *input = parseCommandLine(argc, argv, &bits);

  if (bits == FALSE)
  {
    readAndPrintInputAsHex(input);
  }
  else
  {
    readAndPrintInputAsBits(input);
  }
  return 0;
}
