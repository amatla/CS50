#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{ 
  if (argc != 2)
  {
    printf("Usage: ./recover image\n");
    return 1;
  }
  FILE *infile = fopen(argv[1], "r");

  if (infile == NULL)
  {
    printf("Could not open %s\n", argv[1]);
    return 1;
  }
}