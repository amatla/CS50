#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>

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

    //temporary buffer to store a 512 byte block of data.
    BYTE buffer[512];

    //string to hold 3 digit filename + jpg suffix + string termination character.
    char filename[8];

    //counts the number of jpegs we found for numbering the filename.
    int count = 0;

    //pointer to the output file
    FILE *outfile = NULL;

    //bool to see if we found the first jpeg.
    bool jpeg_found = false;

    //read 512 bytes sized blocks from input file untill we reach the end of it
    while (fread(buffer, sizeof(buffer), 1, infile) == 1)
    {
        //when we find a header that marks the beginning of a jpeg.
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //check if its the first time we find a jpeg
            if (jpeg_found == false)
            {
                jpeg_found = true;
            }
            //if we find the beginning of another jpeg close the current one
            else
            {
                fclose(outfile);
            }
            //create a filename, open a output file with that name and increase the jpeg counter.
            sprintf(filename, "%03i.jpg", count);
            outfile = fopen(filename, "w");
            count++;
        }
        //write jpeg data to output file.
        if (jpeg_found)
        {
            fwrite(buffer, sizeof(buffer), 1, outfile);
        }
    }
    //close files.
    fclose(infile);
    fclose(outfile);
}