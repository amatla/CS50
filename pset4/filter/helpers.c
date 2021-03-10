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
    int half = (width % 2 == 0) ? (width/2) : (width-1) / 2;

    RGBTRIPLE tmp;

    //iterate over half of the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < half; j++)
        {
            //swap pixels from one side to the other 
            tmp = image[i][j];
            image[i][j] = image[i][width-1-j];
            image[i][width-1-j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }
    float neighbours, avgB, avgG, avgR;
    neighbours = avgB = avgG = avgR = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for(int k = i - 1; k <= i + 1; k++)
            {
                for(int l = j - 1; l <= j + 1; l++)
                {
                    //
                    if(!((k < 0) || (l < 0) || (k >= height) || (l >= width)))
                    {
                        avgB += copy[k][l].rgbtBlue;
                        avgG += copy[k][l].rgbtGreen;
                        avgR += copy[k][l].rgbtRed;
                        neighbours++;
                    }
                }
            }
            image[i][j].rgbtBlue = round(avgB/ neighbours);
            image[i][j].rgbtGreen = round(avgG / neighbours);
            image[i][j].rgbtRed = round(avgR / neighbours);
            neighbours = avgB = avgG = avgR = 0;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
