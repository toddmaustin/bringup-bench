#include "libmin.h"
#include "helpers.h"

MFILE bmp =
{
  "original.bmp",
  __origbmp_sz,
  __origbmp,
  0
};
MFILE *mbmp = &bmp;

int
main(int argc, char *argv[])
{
    // help option
    char *help = argv[1];
    if (argc == 2 && ((strcmp(help,"-h") == 0) || (strcmp(help,"--help") == 0)))
    {
        fprintf(stderr,"Options available\n");
        fprintf(stderr,"-b     blur filter\n");
        fprintf(stderr,"-e     edges filter\n");
        fprintf(stderr,"-s     sepia filter\n");
        fprintf(stderr,"-g     greyscale filter\n");
        fprintf(stderr,"-r     reflected filter\n");
        return 1;
    }

    // Define allowable filters
    const char *filters = "begrs";

    // Get filter flag and check validity
    char filter = getopt(argc, argv, filters);
    if (filter == '?')
    {
        fprintf(stderr, "Invalid filter.\n");
        return 1;
    }
    // Ensure only one filter
    if (getopt(argc, argv, filters) != -1)
    {
        fprintf(stderr, "Only one filter allowed.\n");
        return 2;
    }

    // Ensure proper usage
    if (argc != optind + 2)
    {
        fprintf(stderr, "Usage: filter [flag] infile outfile\n");
        return 3;
    }

    // Remember filenames
    char *infile = argv[optind];
    char *outfile = argv[optind + 1];

    // Open input file
    libmin_mopen(mbmp, "r");

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    if (libmin_mread(&bf, sizeof(BITMAPFILEHEADER), mbmp) != sizeof(BITMAPFILEHEADER))
    {
      fprintf(stderr, "ERROR: cannot read BMP header.\n");
      exit(1);
    }

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    if (libmin_mread(&bi, sizeof(BITMAPINFOHEADER), mbmp) != 1)
    {
      fprintf(stderr, "ERROR: cannot read BMP info header.\n");
      exit(1);
    }

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 6;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    _native_RGBTRIPLE(*_native_image)[width] = (_native_RGBTRIPLE (*)[width])calloc(height, width * sizeof(_native_RGBTRIPLE));
    _native_RGBTRIPLE *_pnative_image = (_native_RGBTRIPLE *)_native_image;
    if (_native_image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(_native_RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        if (fread(_native_image[i], sizeof(_native_RGBTRIPLE), width, inptr) != (size_t)width)
        {
          fprintf(stderr, "ERROR: cannot read BMP scanline.\n");
          exit(1);
        }

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // copy native native data to protected structures
    RGBTRIPLE *image = (RGBTRIPLE *)calloc(height, width * sizeof(RGBTRIPLE));
    if (image == NULL)
    {
        fprintf(stderr, "Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j ++)
      {
        PIXEL(image,height,width,i,j).rgbtBlue = PIXEL(_pnative_image,height,width,i,j).rgbtBlue;
        PIXEL(image,height,width,i,j).rgbtGreen = PIXEL(_pnative_image,height,width,i,j).rgbtGreen;
        PIXEL(image,height,width,i,j).rgbtRed = PIXEL(_pnative_image,height,width,i,j).rgbtRed;
      }
    }

    // time the core algorithm
    {
      Stopwatch s("VIP-Bench image-filters:");

      // Filter image
      switch (filter)
      {
        // Blur
        case 'b':
            blur(height, width, (RGBTRIPLE *)image);
            break;

        // Edges
        case 'e':
            edges(height, width, (RGBTRIPLE *)image);
            break;

        // sepia
        case 's':
            sepia(height, width, (RGBTRIPLE *)image);
            break;

        // Grayscale
        case 'g':
            grayscale(height, width, (RGBTRIPLE *)image);
            break;

        // Reflect
        case 'r':
            reflect(height, width, (RGBTRIPLE *)image);
            break;
      }
    }

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // copy protected structures to native native
    for (int i = 0; i < height; i++)
    {
      for (int j = 0; j < width; j ++)
      {
        PIXEL(_pnative_image,height,width,i,j).rgbtBlue = VIP_DEC(PIXEL(image,height,width,i,j).rgbtBlue);
        PIXEL(_pnative_image,height,width,i,j).rgbtGreen = VIP_DEC(PIXEL(image,height,width,i,j).rgbtGreen);
        PIXEL(_pnative_image,height,width,i,j).rgbtRed = VIP_DEC(PIXEL(image,height,width,i,j).rgbtRed);
      }
    }

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(_native_image[i], sizeof(_native_RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    free(image);

    // Close infile
    fclose(inptr);

    // Close outfile
    fclose(outptr);

    return 0;
}
