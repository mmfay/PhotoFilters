/**
* This file holds the forward declarations & Structs for Pixels.
*
* Completion time: 11h (Total Project)
*
* @author Matthew Fay used Matthew Fay HW3 as Base File.
* @version 1.0
*/
////////////
//INCLUDES
#ifndef PIXELPROCESSOR_H
#define PIXELPROCESSOR_H
#include <stdlib.h>
#include <stdio.h>
////////////
//STRUCTS
struct Pixel{
    unsigned char blue;
    unsigned char green;
    unsigned char red;
};

struct Holes{
    int x;
    int y;
    int radius;   
};

struct blurInfo{
    int start;
    int end;
    int imageWidth;
    int imageHeight;
    struct Pixel** pArr;

};

struct SwissInfo{
    int start;
    int end;
    int imageWidth;
    int imageHeight;
    struct Pixel** pArr;
    struct Holes** hArr;
    int holeCount;
};
/**
 * Shift color of Pixel array. The dimension of the array is width * height. The shift value of RGB is 
 * rShift, gShift，bShift. Useful for color shift.
 *
 * @param  pArr: Pixel array of the image that this header is for
 * @param  width: Width of the image that this header is for
 * @param  height: Height of the image that this header is for
 * @param  rShift: the shift value of color r shift
 * @param  gShift: the shift value of col
 * @param  bShift: the shift value of color b shift
 */
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift);
/**
 * free memory for Pixel
 *
 * @param  header: a pointer to Pixel
 * @param  width: width of pixel
 * @param  height: height of pixel 
 */
void PixelDestructor(struct Pixel*** pArr, int width, int height);
/**
 * Blur Filter for testing without threads.
 * @param pArr pixel data
 * @param width image width
 * @param height image height
 */
void boxBlur(struct Pixel** pArr, int width, int height);
/**
 * Blur filter used for threading.
 * @param data holds data required for thread.
 * @return 
 */
void* boxThreadBlur(void* data);
/**
 * Swiss Cheese filter used for threading.
 * @param data holds all  data for thread.
 * @return 
 */
void* threadSwissCheese(void* data);
/**
 * Used to create Swiss Cheese Filter without threads.
 * 
 * @param pArr pixel struct
 * @param hArr hole strut
 * @param holeNum number of holes
 * @param width image width
 * @param height image height
 */
void swissCheese(struct Pixel** pArr, struct Holes** hArr, int holeNum, int width, int height);
/**
 * Creates Information on holes for swiss cheese filter.
 * 
 * @param hArr hole array
 * @param holeNum number of holes
 * @param avgRad Average radius
 * @param width Image Width
 * @param height Image Height
 */
void holeCreator(struct Holes** hArr, int holeNum, int avgRad, int width, int height);
#endif /* PIXELPROCESSOR_H */
