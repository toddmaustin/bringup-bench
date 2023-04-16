#include "bmp.h"

// image accessor macros
#define PIXEL(IMG, H, W, i, j)    IMG[(i)*(W) + (j)]


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE *image);

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE *image);

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE *image);

// Detect edges
void edges(int height, int width, RGBTRIPLE *image);

// Blur image
void blur(int height, int width, RGBTRIPLE *image);

