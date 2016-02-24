/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef uint8_t  BYTE;
typedef uint32_t DWORD;

char* infile = "card.raw";
char outfile[] = "000.jpg";
bool startjpg = false;
int counter = 0;

int main(int argc, char* argv[])
{
    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }
    
    
    FILE* outptr = NULL;
    BYTE m[512];
    char* s1 = "ffd8ffe0\0";
    char* s2 = "ffd8ffe1\0";
    char sbuff[9];
    
    // read 512 Bytes at a time store in m
    while (fread(&m, sizeof(BYTE)*512, 1, inptr) == 1)
    {
        // store first 4 bytes
        sprintf(sbuff, "%x%x%x%x", m[0],m[1],m[2],m[3]);
        
        if ((strcmp(s1, sbuff) == 0 || strcmp(s2, sbuff) == 0) && startjpg)
        {
            fclose(outptr);
            startjpg = false;
        }
        
        if ((strcmp(s1, sbuff) == 0 || strcmp(s2, sbuff) == 0) && !startjpg)
        {
            startjpg = true;
            sprintf(outfile, "%.3d.jpg", counter);
            
            // open output file
            outptr = fopen(outfile, "w");
            if (outptr == NULL)
            {
                fclose(inptr);
                fprintf(stderr, "Could not create %s.\n", outfile);
                return 3;
            }
            counter += 1;
            
            //printf("found start of jpg\n");
        }
        
        if (startjpg)
        {
            fwrite(&m, sizeof(BYTE)*512, 1, outptr);
        }
        
        
    }
    
    //printf("%d\n", counter);
    fclose(inptr);
    fclose(outptr);
    
    return 0;   
}
