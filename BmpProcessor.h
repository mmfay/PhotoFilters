/**
* This file holds the forward declarations & Structs for BMPFiles.
*
* Completion time: 11h (Total Project)
*
* @author Matthew Fay used Matthew Fay HW3 as Base File.
* @version 1.0
*/

#ifndef BMPPROCESSOR_H
#define BMPPROCESSOR_H

#include "PixelProcessor.h"

struct BMP_Header {
    char signature[2];          //2 bytes
    int fileSize;               //4 bytes
    short reserved1;            //2 bytes
    short reserved2;            //2 bytes
    int fileOffsetToPixArray;   //4 bytes
                                //14 bytes.
};

struct DIB_Header {
    int DIBHeaderSize;          //4 bytes
    int ImageWidth;             //4 bytes
    int ImageHeight;            //4 bytes
    short planes;               //2 bytes
    short bitsPerPixel;         //2 bytes
    int compression;            //4 bytes
    int imageSize;              //4 bytes
    int xPixPerMeter;           //4 bytes
    int yPixPerMeter;           //4 bytes
    int colorsInColorTable;     //4 bytes
    int importantColorCount;    //4 bytes
                                //40 bytes
 
};
/**
 * read BMP header of a file. Useful for converting files from PPM to BMP.
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination BMP header
 */
void readBMPHeader(FILE* file, struct BMP_Header* header);
/**
 * write BMP header of a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: The header made by makeBMPHeader function
 */
void writeBMPHeader(FILE* file, struct BMP_Header* header);
/**
 * read DIB header from a file. Useful for converting files from PPM to BMP.
 *
 * @param  file: A pointer to the file being read or written
 * @param  header: Pointer to the destination DIB header
 */
void readDIBHeader(FILE* file, struct DIB_Header* header);
 /**
 * Makes new DIB header based on width and height. Useful for converting files from PPM to BMP.
 *
 * @param  header: Pointer to the destination DIB header
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writeDIBHeader(FILE* file, struct DIB_Header* header);
/**
 * read Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void readPixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);
/**
 * write Pixels from BMP file based on width and height.
 *
 * @param  file: A pointer to the file being read or written
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 */
void writePixelsBMP(FILE* file, struct Pixel** pArr, int width, int height);
/**
 * free memory for BMP Header
 *
 * @param  header: a pointer to BMP Header 
 */
void BMPHeaderDestructor(struct BMP_Header** header);
/**
 * free memory for DIB Header
 *
 * @param  header: a pointer to DIB Header 
 */
void DIBHeaderDestructor(struct DIB_Header** header);

#endif /* BMPPROCESSOR_H */


