#include <math.h>

#include "../config.h"
#include "mathlib.h"
#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE *image)
{
    // update to avg of blue green and red
    VIP_ENCFLOAT avg = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            avg = (((VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtBlue + PIXEL(image,height,width,i,j).rgbtGreen + PIXEL(image,height,width,i,j).rgbtRed) / 3);
            VIP_ENCINT roundi = myround(avg);
            PIXEL(image,height,width,i,j).rgbtBlue = roundi;
            PIXEL(image,height,width,i,j).rgbtGreen = roundi;
            PIXEL(image,height,width,i,j).rgbtRed = roundi;

        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE *image)
{
    // swaping 2 vals [end to start]
    for (int i = 0; i < height; i++)
    {
        //only till width / 2 because we dont want it to swap again
        for (int j = 0; j < (int)width / 2 ; j ++)
        {
            VIP_ENCINT tmpblue = PIXEL(image,height,width,i,j).rgbtBlue;
            PIXEL(image,height,width,i,j).rgbtBlue = PIXEL(image,heigth,width,i,(width - j - 1)).rgbtBlue;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtBlue = tmpblue;

            VIP_ENCINT tmpgreen = PIXEL(image,height,width,i,j).rgbtGreen;
            PIXEL(image,height,width,i,j).rgbtGreen = PIXEL(image,height,width,i,(width - j - 1)).rgbtGreen;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtGreen = tmpgreen;

            VIP_ENCINT tmpred = PIXEL(image,height,width,i,j).rgbtRed;
            PIXEL(image,height,width,i,j).rgbtRed = PIXEL(image,height,width,i,(width - j - 1)).rgbtRed;
            PIXEL(image,height,width,i,(width - j - 1)).rgbtRed = tmpred;

        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE *image)
{
    //sepia red blue green is all 0 for now
    VIP_ENCFLOAT sepiaRed = 0;
    VIP_ENCFLOAT sepiaBlue = 0;
    VIP_ENCFLOAT sepiaGreen = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            // applying algo to all
            sepiaRed = ((VIP_ENCFLOAT)0.393 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtRed) + ((VIP_ENCFLOAT)0.769 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtGreen) + ((VIP_ENCFLOAT)0.189 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtBlue);
            sepiaGreen = ((VIP_ENCFLOAT)0.349 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtRed) + ((VIP_ENCFLOAT)0.686 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtGreen) + ((VIP_ENCFLOAT)0.168 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtBlue);
            sepiaBlue = ((VIP_ENCFLOAT)0.272 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtRed) + ((VIP_ENCFLOAT)0.534 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtGreen) + ((VIP_ENCFLOAT)0.131 * (VIP_ENCFLOAT)PIXEL(image,height,width,i,j).rgbtBlue);

            VIP_ENCINT sred = myround(sepiaRed);
            VIP_ENCINT sgreen = myround(sepiaGreen);
            VIP_ENCINT sblue = myround(sepiaBlue);

            // if more than 255 which is max of rgb cap it to 255
#ifdef VIP_DO_MODE
            sred = VIP_CMOV(sred > 255, (VIP_ENCINT)255, sred);
            sgreen = VIP_CMOV(sgreen > 255, (VIP_ENCINT)255, sgreen);
            sblue = VIP_CMOV(sblue > 255, (VIP_ENCINT)255, sblue);
#else /* !VIP_DO_MODE */
            if (sred > 255)
            {
                sred = 255;
            }
            if (sgreen > 255)
            {
                sgreen = 255;
            }
            if (sblue > 255)
            {
                sblue = 255;
            }
#endif /* !VIP_DO_MODE */
            PIXEL(image,height,width,i,j).rgbtBlue = sblue;
            PIXEL(image,height,width,i,j).rgbtGreen = sgreen;
            PIXEL(image,height,width,i,j).rgbtRed = sred;
        }
    }
    return;

}

// Blur image
void blur(int height, int width, RGBTRIPLE *image)
{
    // RGBTRIPLE tmp[height][width];
    RGBTRIPLE *tmp = (RGBTRIPLE *)malloc(height * width * sizeof(RGBTRIPLE));

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            int count = 0;

            int xaxis[] = {row - 1, row, row + 1};
            int yaxis[] = {col - 1, col, col + 1};

            VIP_ENCFLOAT totalR = 0, totalG = 0, totalB = 0;

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = xaxis[r];
                    int curCol = yaxis[c];

                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width)
                    {
                        RGBTRIPLE pixel = PIXEL(image,height,width,curRow,curCol);

                        totalR = totalR + pixel.rgbtRed;
                        totalG = totalG + pixel.rgbtGreen;
                        totalB = totalB + pixel.rgbtBlue;
                        count ++;

                    }
                }
            }

            PIXEL(tmp,height,width,row,col).rgbtRed = myround(totalR / count);
            PIXEL(tmp,height,width,row,col).rgbtGreen = myround(totalG / count);
            PIXEL(tmp,height,width,row,col).rgbtBlue = myround(totalB / count);

        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            PIXEL(image,height,width,i,j) = PIXEL(tmp,height,width,i,j);
        }
    }

    free(tmp);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE *image)
{
    // RGBTRIPLE tmp[height][width];
    RGBTRIPLE *tmp = (RGBTRIPLE *)malloc(height * width * sizeof(RGBTRIPLE));

    //gx matrix
    VIP_ENCINT Gx[3][3] =
    {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    //gy matrix
    VIP_ENCINT Gy[3][3] =
    {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int row = 0; row < height; row ++)
    {
        for (int col = 0; col < width; col ++)
        {
            //x axis
            int xaxis[] = {row - 1, row, row + 1};
            // y axis
            int yaxis[] = {col - 1, col, col + 1};

            // float vals for gx rgb
            VIP_ENCFLOAT Gx_R = 0, Gx_G = 0, Gx_B = 0;

            // float vals for gy rgb
            VIP_ENCFLOAT Gy_R = 0, Gy_G = 0, Gy_B = 0;

            for (int r = 0; r < 3; r++)
            {
                for (int c = 0; c < 3; c++)
                {
                    int curRow = xaxis[r];
                    int curCol = yaxis[c];

                    if (curRow >= 0 && curRow < height && curCol >= 0 && curCol < width)
                    {
                        RGBTRIPLE pixel = PIXEL(image,height,width,curRow,curCol);

                        // matrix for gx_rgb * the gx vals
                        Gx_R += Gx[r][c] * pixel.rgbtRed;
                        Gx_G += Gx[r][c] * pixel.rgbtGreen;
                        Gx_B += Gx[r][c] * pixel.rgbtBlue;

                        // matrix for gy_rgb * the gy vals
                        Gy_R += Gy[r][c] * pixel.rgbtRed;
                        Gy_G += Gy[r][c] * pixel.rgbtGreen;
                        Gy_B += Gy[r][c] * pixel.rgbtBlue;
                    }
                }
            }

            //sqrt of the vals of gx and gy rgb then roud it
            VIP_ENCINT final_red = myround(mysqrt((Gx_R * Gx_R) + (Gy_R * Gy_R)));
            VIP_ENCINT final_green = myround(mysqrt((Gx_G * Gx_G) + (Gy_G * Gy_G)));
            VIP_ENCINT final_blue = myround(mysqrt((Gx_B * Gx_B) + (Gy_B * Gy_B)));

            // if the value more than 255 then cap it to 255
#ifdef VIP_DO_MODE
            final_red = VIP_CMOV(final_red > 255, (VIP_ENCINT)255, final_red);
            final_green = VIP_CMOV(final_green > 255, (VIP_ENCINT)255, final_green);
            final_blue = VIP_CMOV(final_blue > 255, (VIP_ENCINT)255, final_blue);
#else /* !VIP_DO_MODE */
            if (final_red > 255)
            {
                final_red = 255;
            }
            if (final_green > 255)
            {
                final_green = 255;
            }
            if (final_blue > 255)
            {
                final_blue = 255;
            }
#endif /* !VIP_DO_MODE */

            //update vals in the tmp
            PIXEL(tmp,height,width,row,col).rgbtRed = final_red;
            PIXEL(tmp,height,width,row,col).rgbtGreen = final_green;
            PIXEL(tmp,height,width,row,col).rgbtBlue = final_blue;
        }
    }

    // updating the vals into the new image output
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j ++)
        {
            PIXEL(image,heigth,width,i,j) = PIXEL(tmp,height,width,i,j);
        }
    }

    free(tmp);
    return;
}

