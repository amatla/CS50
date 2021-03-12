#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //if the image has an even width we iterate over half of it
    //if the image has an odd width we leave the middle column unchanged
    int half = (width % 2 == 0) ? (width / 2) : (width - 1) / 2;

    RGBTRIPLE tmp;

    //iterate over half of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            //swap pixels from one side to the other
            tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of the orignal image to read un-altered values from.
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // variables to count the number of neighbours of a pixel
    // and to keep track of the average for Blue, Green and Red.
    float neighbours, avgB, avgG, avgR;
    neighbours = avgB = avgG = avgR = 0;

    //iterate over every pixel of the image (i and j for loops)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //iterate over every neighbouring pixel in a 1 pixel radius (k and j for loops).
            for (int k = i - 1; k <= i + 1; k++)
            {
                for (int l = j - 1; l <= j + 1; l++)
                {
                    // if the neighbour exist (if its not outside the edges of the image)
                    // add its Blue, Red and Green values to the correspondent variable
                    // and increase the neighbour count.
                    if (!((k < 0) || (l < 0) || (k >= height) || (l >= width)))
                    {
                        avgB += copy[k][l].rgbtBlue;
                        avgG += copy[k][l].rgbtGreen;
                        avgR += copy[k][l].rgbtRed;
                        neighbours++;
                    }
                }
            }
            //calculate the averages and set the pixel value accordingly.
            image[i][j].rgbtBlue = round(avgB / neighbours);
            image[i][j].rgbtGreen = round(avgG / neighbours);
            image[i][j].rgbtRed = round(avgR / neighbours);

            //reset neighbour count and averages to 0 for the next pixel.
            neighbours = avgB = avgG = avgR = 0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // create a copy of the orignal image to read un-altered values from.
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // create two 3x3 matrixes for the Gx and Gy kernels
    // and initialize them with the proper values.
    float gxKernel[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };
    float gyKernel[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    // variables to hold Blue, Green and Red values for both kernels.
    float gx[3] = {0, 0, 0};
    float gy[3] = {0, 0, 0};
    float tmp = 0;

    // iterate over every pixel of the image (i and j for loops).
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //iterate over every neighbouring pixel in a 1 pixel radius (k and j for loops).
            for (int k = -1; k <= 1 ; k++)
            {
                for (int l = -1; l <= 1; l++)
                {
                    // if the neighbour exist (if its not outside the edges of the image)
                    // multiply its Blue, Gren and Red values for the corresponding value in the Gx and Gy kernels
                    // and add the value to the gx and gy variables .
                    // (we shift the range for the gx and gy kernels from -1/1 to 0/2 by adding 1 to k and l).
                    if (!((i + k < 0) || (j + l < 0) || (i + k >= height) || (j + l >= width)))
                    {
                        gx[0] += (copy[i + k][j + l].rgbtBlue * gxKernel[k + 1][l + 1]);
                        gx[1] += (copy[i + k][j + l].rgbtGreen * gxKernel[k + 1][l + 1]);
                        gx[2] += (copy[i + k][j + l].rgbtRed * gxKernel[k + 1][l + 1]);

                        gy[0] += (copy[i + k][j + l].rgbtBlue * gyKernel[k + 1][l + 1]);
                        gy[1] += (copy[i + k][j + l].rgbtGreen * gyKernel[k + 1][l + 1]);
                        gy[2] += (copy[i + k][j + l].rgbtRed * gyKernel[k + 1][l + 1]);
                    }
                }
            }
            // Change the value of the pixel Blue, Green and Red channel
            // using Sobel operator. Make sure the final value is between 0 and 255.
            tmp = round(sqrt(pow(gx[0], 2) + pow(gy[0], 2)));
            image[i][j].rgbtBlue = (tmp >= 255) ? 255 : tmp;

            tmp = round(sqrt(pow(gx[1], 2) + pow(gy[1], 2)));
            image[i][j].rgbtGreen = (tmp >= 255) ? 255 : tmp;

            tmp = round(sqrt(pow(gx[2], 2) + pow(gy[2], 2)));
            image[i][j].rgbtRed = (tmp >= 255) ? 255 : tmp;

            // reset gx and gm to 0 for the next pixel.
            for (int m = 0; m < 3; m++)
            {
                gx[m] = 0;
                gy[m] = 0;
            }
        }
    }
    return;
}
