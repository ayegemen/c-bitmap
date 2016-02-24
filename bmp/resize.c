/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int fac = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    
    if (fac < 1 || fac > 100)
    {
        printf("Number between 1 and 100\n");
        return 4;
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }
    

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // initialize outfiles Bitmapfileheader and bitmapinfoheader
    BITMAPFILEHEADER obf = bf;
    //fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    
    BITMAPINFOHEADER obi = bi;
    //fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    
    // change the bitmapinfoheader
    obi.biSize = 40;
    obi.biWidth = bi.biWidth * fac;
    obi.biHeight = bi.biHeight * fac;
    obi.biPlanes = 1;
    obi.biBitCount = 24;
    obi.biCompression = 0;
    //obi.biSizeImage = (obi.biWidth * abs(obi.biHeight) * 3);
    obi.biXPelsPerMeter = 2834;
    obi.biYPelsPerMeter = 2834;
    obi.biClrUsed = 0;
    obi.biClrImportant = 0;
    
    // change the bitmapfileheader
    obf.bfType = 0x4d42;
    obf.bfReserved1 = 0;
    obf.bfReserved2 = 0;
    bf.bfOffBits = 54;
    //obf.bfSize = obi.biSizeImage + obf.bfOffBits;

    // determine input padding for scanlines
    int padding =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    // determine output padding for scanlines
    int opadding =  (4 - (obi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    
    obi.biSizeImage = (obi.biWidth * sizeof(RGBTRIPLE) + opadding) * abs(obi.biHeight);
    obf.bfSize = obi.biSizeImage + obf.bfOffBits;
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&obf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&obi, sizeof(BITMAPINFOHEADER), 1, outptr);
    
    // iterate over infile's scanlines
    for (int i = 1, biHeight = abs(bi.biHeight); i <= biHeight; i++)
    {
        //int count = 0;
        // iterate over pixels in scanline
        for (int j = 1; j <= obi.biWidth; j++)
        {
            
            // temporary storage    
            RGBTRIPLE triple;

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
            
            //fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            
            for (int z = 0; z < fac; z++)
            {
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
            }
            
            if (j % bi.biWidth == 0 && j != obi.biWidth)
            //if (j == bi.biWidth)
            {
                //count += 1;
                
                for (int k = 0; k < opadding; k++)
                {
                    fputc(0x00, outptr);
                }
                fseek(inptr, -bi.biWidth*3, SEEK_CUR);
            }
            if (j % obi.biWidth == 0)
            {
                for (int k = 0; k < opadding; k++)
                {
                    fputc(0x00, outptr);
                }
                fseek(inptr, padding, SEEK_CUR);
            }
        }
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
