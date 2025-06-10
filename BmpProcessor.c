/**
* Functions are specified below. helps seperate them from main.c file. 
* Provides a level of abstraction, allows us to not show our code.
*
* Completion time: 11h (Total Project)
*
* @author Matthew Fay used Matthew Fay HW3 as Base File.
* @version 1.0
*/
//////////////////////
///INCLUDES
#include <stdlib.h>
#include <stdio.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

//////////////////////
///FUNCTIONS
/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file,struct BMP_Header* header){
    
    fread(&header->signature, sizeof(char)*2,1, file);
    fread(&header->fileSize,sizeof(int),1,file);
    fread(&header->reserved1,sizeof(short),1,file);
    fread(&header->reserved2,sizeof(short),1,file);
    fread(&header->fileOffsetToPixArray,sizeof(int),1,file);
    
}
/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header){
    
    fread(&header->DIBHeaderSize, sizeof(int), 1, file);
    fread(&header->ImageWidth, sizeof(int), 1, file);
    fread(&header->ImageHeight, sizeof(int), 1, file);
    fread(&header->planes, sizeof(short), 1, file);
    fread(&header->bitsPerPixel, sizeof(short), 1, file);
    fread(&header->compression, sizeof(int), 1, file);
    fread(&header->imageSize, sizeof(int), 1, file);
    fread(&header->xPixPerMeter, sizeof(int), 1, file);
    fread(&header->yPixPerMeter, sizeof(int), 1, file);
    fread(&header->colorsInColorTable, sizeof(int), 1, file);
    fread(&header->importantColorCount, sizeof(int), 1, file);

}
/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    
    int i;
    unsigned char b, g, r;
    int padding;
    
    if (width % 4 == 1){
        padding = 3;
    }
    if (width % 4 == 2){
        padding = 2;
    }
    if (width % 4 == 3){
        padding = 1;
    }
    if (width % 4 == 0){
        padding = 0;
    }    
    for (i = 0; i < height; i++){
        pArr[i] = malloc(sizeof(struct Pixel) * width);
    }

    
    int j,k;

    for ( j = 0; j < height; j++){
        for ( k = 0; k < width; k++){
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);

            pArr[j][k].blue = b;
            pArr[j][k].green = g;
            pArr[j][k].red = r;
            
        }
        fseek(file,sizeof(unsigned char) * padding, SEEK_CUR);
    }
}
/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header){
    
    fwrite(&header->signature, sizeof(char) * 2, 1, file);
    fwrite(&header->fileSize, sizeof(int), 1, file);
    fwrite(&header->reserved1, sizeof(short), 1, file);
    fwrite(&header->reserved2, sizeof(short), 1, file);
    fwrite(&header->fileOffsetToPixArray, sizeof(int), 1, file);
    
}
 /**
 * Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header){
    fwrite(&header->DIBHeaderSize, sizeof(int), 1, file);
    fwrite(&header->ImageWidth, sizeof(int), 1, file);
    fwrite(&header->ImageHeight, sizeof(int), 1, file);
    fwrite(&header->planes, sizeof(short), 1, file);
    fwrite(&header->bitsPerPixel, sizeof(short), 1, file);
    fwrite(&header->compression, sizeof(int), 1, file);
    fwrite(&header->imageSize, sizeof(int), 1, file);
    fwrite(&header->xPixPerMeter, sizeof(int), 1, file);
    fwrite(&header->yPixPerMeter, sizeof(int), 1, file);
    fwrite(&header->colorsInColorTable, sizeof(int), 1, file);
    fwrite(&header->importantColorCount, sizeof(int), 1, file);
}
/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height){
    
    int i, j, k;
    int padding;
    int padVal = 1;
    
    if (width % 4 == 1){
        padding = 3;
    }
    if (width % 4 == 2){
        padding = 2;
    }
    if (width % 4 == 3){
        padding = 1;
    }
    if (width % 4 == 0){
        padding = 0;
    }
    for(i = 0; i < height; i++){
        for(j = 0; j < width; j++){
            fwrite(&pArr[i][j].blue, sizeof(char), 1, file);
            fwrite(&pArr[i][j].green, sizeof(char), 1, file);
            fwrite(&pArr[i][j].red, sizeof(char), 1, file);
            
        }

        for(k = 0; k < padding; k++){
            fwrite(&padVal, sizeof(char), 1, file);
        }
    }
}
/**
 * free memory for BMP Header
 *
 * @param  header: a pointer to BMP Header 
 */
void BMPHeaderDestructor(struct BMP_Header** header){
    free(*header);
    *header = NULL;
}
/**
 * free memory for DIB Header
 *
 * @param  header: a pointer to DIB Header 
 */
void DIBHeaderDestructor(struct DIB_Header** header){
    free(*header);
    *header = NULL;
}