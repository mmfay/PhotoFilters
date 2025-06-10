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
#include <math.h>
#include <pthread.h>
#include "PixelProcessor.h"

//////////////////////
///FUNCTIONS
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
void colorShiftPixels(struct Pixel** pArr, int width, int height, int rShift, int gShift, int bShift){
    int i, j;
    for(i = 0; i < height; i++){
        for (j = 0; j < width; j++){
            //blue logic NO CLAMPING

                if (((pArr[i][j].blue + bShift) < 0) || ((pArr[i][j].blue + bShift) > 255)){
                    
                    if (((pArr[i][j].blue + bShift) < 0)){
                        pArr[i][j].blue = 0;
                    }
                    else {
                        pArr[i][j].blue = 255;
                    }
                } else {
                    pArr[i][j].blue = pArr[i][j].blue + bShift;
                } 
           
            //green logic NO CLAMPING
                if (((pArr[i][j].green + gShift) < 0) || ((pArr[i][j].green + gShift) > 255)){
                    
                    if (((pArr[i][j].green + gShift) < 0)){
                        pArr[i][j].green = 0;
                    }
                    else {
                        pArr[i][j].green = 255;
                    }
                } else {
                    pArr[i][j].green = pArr[i][j].green + gShift;
                } 

            //red logic NO CLAMPING
                if (((pArr[i][j].red + rShift) < 0) || ((pArr[i][j].red + rShift) > 255)){
                    
                    if (((pArr[i][j].red + rShift) < 0)){
                        pArr[i][j].red = 0;
                    }
                    else {
                        pArr[i][j].red = 255;
                    }
                } else {
                    pArr[i][j].red = pArr[i][j].red + rShift;
                }   

        }
    }
}
/**
 * Blur Filter for testing without threads.
 * @param pArr pixel data
 * @param width image width
 * @param height image height
 */
void boxBlur(struct Pixel** pArr, int width, int height) {
    int red = 0;
    int blue = 0;
    int green = 0;
    int sumR = 0;
    int sumB = 0;
    int sumG = 0;
    int count = 0;
    
    for(int i = 0; i < width; i++) {
        
        for (int j = 0; j < height; j++) {
             
                //(0,0)
                sumR = sumR + pArr[i][j].red;
                sumG = sumG + pArr[i][j].green;
                sumB = sumB + pArr[i][j].blue; 
                count++;
                //as long as we arent to width limit.
                if(i+1 < width){
                    if (j+1 < height) {
                        sumR = sumR + pArr[i+1][j+1].red;
                        sumG = sumG + pArr[i+1][j+1].green;
                        sumB = sumB + pArr[i+1][j+1].blue; 
                        count++;
                    } 
                    if (j-1 >= 0) {
                        sumR = sumR + pArr[i+1][j-1].red;
                        sumG = sumG + pArr[i+1][j-1].green;
                        sumB = sumB + pArr[i+1][j-1].blue; 
                        count++;                        
                    }
                    
                    sumR = sumR + pArr[i+1][j].red;
                    sumG = sumG + pArr[i+1][j].green;
                    sumB = sumB + pArr[i+1][j].blue; 
                    count++;
                    
                }
                //as long as we arent reaching beyond width start.
                if (i-1 >= 0) {
                    if (j+1 < height) {
                        sumR = sumR + pArr[i-1][j+1].red;
                        sumG = sumG + pArr[i-1][j+1].green;
                        sumB = sumB + pArr[i-1][j+1].blue; 
                        count++;
                    }
                    if (j-1 >= 0) {
                        sumR = sumR + pArr[i-1][j-1].red;
                        sumG = sumG + pArr[i-1][j-1].green;
                        sumB = sumB + pArr[i-1][j-1].blue; 
                        count++;                        
                    }  
                    
                    sumR = sumR + pArr[i-1][j].red;
                    sumG = sumG + pArr[i-1][j].green;
                    sumB = sumB + pArr[i-1][j].blue; 
                    count++;
                    
                }
                if (j+1 < height) {
                    sumR = sumR + pArr[i][j+1].red;
                    sumG = sumG + pArr[i][j+1].green;
                    sumB = sumB + pArr[i][j+1].blue; 
                    count++;
                }
                if (j-1 >= 0){
                    sumR = sumR + pArr[i][j-1].red;
                    sumG = sumG + pArr[i][j-1].green;
                    sumB = sumB + pArr[i][j-1].blue; 
                    count++;
                }
  
                pArr[i][j].red = (sumR / count);
                pArr[i][j].green = (sumG / count);
                pArr[i][j].blue = (sumB / count);
                sumR = 0;
                sumG = 0;
                sumB = 0;
                count = 0;
                
            
        }
        
    }
    //edge case
    
    //edge case
}
/**
 * free memory for Pixel
 *
 * @param  header: a pointer to Pixel
 * @param  width: width of pixel
 * @param  height: height of pixel 
 */
void PixelDestructor(struct Pixel*** pArr, int width, int height){
    int i;
    for(i = 0; i < height; i++){
        free(pArr[0][i]);
    }
    free(*pArr);
    *pArr = NULL;
}
/**
 * Used to create Swiss Cheese Filter without threads.
 * 
 * @param pArr pixel struct
 * @param hArr hole strut
 * @param holeNum number of holes
 * @param width image width
 * @param height image height
 */
void swissCheese(struct Pixel** pArr, struct Holes** hArr, int holeNum, int width, int height) {
    int x = 30;
    int y = 30;
    int r = 10;
    int resultx = 0;
    int resulty = 0;
    int total = 0;
    double distance = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < holeNum; k++){
                resultx = ((hArr[k]->x - i)*(hArr[k]->x - i));
                resulty = ((hArr[k]->y - j)*(hArr[k]->y - j));
                total = resultx + resulty;
                distance = sqrt(total);
                if (distance < hArr[k]->radius) {
                    pArr[i][j].red = 0;
                    pArr[i][j].blue = 0;
                    pArr[i][j].green = 0;
                }
                resultx = 0;
                resulty = 0;
                total = 0;
                distance = 0;
            }
            /*
            resultx = ((x-i)*(x-i));
            resulty = ((y-j)*(y-j));
            total = resultx + resulty;
            distance = sqrt(total);
            if (distance < r) {
                pArr[i][j].red = 0;
                pArr[i][j].blue = 0;
                pArr[i][j].green = 0;
            }
            */
        }
    }
}
/**
 * Creates Information on holes for swiss cheese filter.
 * 
 * @param hArr hole array
 * @param holeNum number of holes
 * @param avgRad Average radius
 * @param width Image Width
 * @param height Image Height
 */
void holeCreator(struct Holes** hArr, int holeNum, int avgRad, int width, int height){
    int normal = holeNum / 2;
    int bigger = (holeNum - normal) / 2;
    int smaller = holeNum - normal - bigger;
    int bigR = avgRad * 1.25;
    int smallR = avgRad / 2;
    int holeInc = 0;
    srand(time(0));
        
        //normalHole loop
        for(int n = 0; n < normal; n++){
            hArr[holeInc] = malloc(sizeof(struct Holes));            
            hArr[holeInc]->radius = avgRad;
            hArr[holeInc]->x = rand() % width;
            hArr[holeInc]->y = rand() % height;            
            holeInc++;
            
        }
        //biggerHole loop
        for(int b = 0; b < bigger; b++){
            hArr[holeInc] = malloc(sizeof(struct Holes));
            hArr[holeInc]->radius = bigR;
            hArr[holeInc]->x = rand() % width;
            hArr[holeInc]->y = rand() % height;
            holeInc++;
        }
        //smallerHole loop
        for(int s = 0; s < smaller; s++) {
            hArr[holeInc] = malloc(sizeof(struct Holes));
            hArr[holeInc]->radius = smallR;
            hArr[holeInc]->x = rand() % width;
            hArr[holeInc]->y = rand() % height;
            holeInc++;
        }
  
}
/**
 * Blur filter used for threading.
 * @param data holds data required for thread.
 * @return 
 */
void* boxThreadBlur(void* data){
    struct blurInfo* info = (struct blurInfo*) data;
    
    int red = 0;
    int blue = 0;
    int green = 0;
    int sumR = 0;
    int sumB = 0;
    int sumG = 0;
    int count = 0;
    
    //for(int i = info->start; i < info->end; i++) {
    for(int i = 0; i < info->imageHeight; i++) {    
        //for (int j = 0; j < info->imageHeight; j++) {
        for(int j = info->start; j < info->end; j++) {   
                //(0,0)
                sumR = sumR + info->pArr[i][j].red;
                sumG = sumG + info->pArr[i][j].green;
                sumB = sumB + info->pArr[i][j].blue; 
                count++;
                //as long as we arent to width limit.
                if(i+1 < info->imageWidth){
                    if (j+1 < info->imageHeight) {
                        sumR = sumR + info->pArr[i+1][j+1].red;
                        sumG = sumG + info->pArr[i+1][j+1].green;
                        sumB = sumB + info->pArr[i+1][j+1].blue; 
                        count++;
                    } 
                    if (j-1 >= 0) {
                        sumR = sumR + info->pArr[i+1][j-1].red;
                        sumG = sumG + info->pArr[i+1][j-1].green;
                        sumB = sumB + info->pArr[i+1][j-1].blue; 
                        count++;                        
                    }
                    
                    sumR = sumR + info->pArr[i+1][j].red;
                    sumG = sumG + info->pArr[i+1][j].green;
                    sumB = sumB + info->pArr[i+1][j].blue; 
                    count++;
                    
                }
                //as long as we arent reaching beyond width start.
                if (i-1 >= 0) {
                    if (j+1 < info->imageHeight) {
                        sumR = sumR + info->pArr[i-1][j+1].red;
                        sumG = sumG + info->pArr[i-1][j+1].green;
                        sumB = sumB + info->pArr[i-1][j+1].blue; 
                        count++;
                    }
                    if (j-1 >= 0) {
                        sumR = sumR + info->pArr[i-1][j-1].red;
                        sumG = sumG + info->pArr[i-1][j-1].green;
                        sumB = sumB + info->pArr[i-1][j-1].blue; 
                        count++;                        
                    }  
                    
                    sumR = sumR + info->pArr[i-1][j].red;
                    sumG = sumG + info->pArr[i-1][j].green;
                    sumB = sumB + info->pArr[i-1][j].blue; 
                    count++;
                    
                }
                if (j+1 < info->imageHeight) {
                    sumR = sumR + info->pArr[i][j+1].red;
                    sumG = sumG + info->pArr[i][j+1].green;
                    sumB = sumB + info->pArr[i][j+1].blue; 
                    count++;
                }
                if (j-1 >= 0){
                    sumR = sumR + info->pArr[i][j-1].red;
                    sumG = sumG + info->pArr[i][j-1].green;
                    sumB = sumB + info->pArr[i][j-1].blue; 
                    count++;
                }
  
                info->pArr[i][j].red = (sumR / count);
                info->pArr[i][j].green = (sumG / count);
                info->pArr[i][j].blue = (sumB / count);
                sumR = 0;
                sumG = 0;
                sumB = 0;
                count = 0;
                
            
        }
        
    }
    pthread_exit(0);  
       
}
/**
 * Swiss Cheese filter used for threading.
 * @param data holds all  data for thread.
 * @return 
 */
void* threadSwissCheese(void* data){
    struct SwissInfo* info = (struct SwissInfo*) data;
    int tint = 65;
    int resultx = 0;
    int resulty = 0;
    int total = 0;
    double distance = 0;
    for (int i = 0; i < info->imageHeight; i++) {
        for (int j = info->start; j < info->end; j++) {
            //green logic NO CLAMPING
                if (((info->pArr[i][j].green + tint) < 0) || ((info->pArr[i][j].green + tint) > 255)){
                    
                    if (((info->pArr[i][j].green + tint) < 0)){
                        info->pArr[i][j].green = 0;
                    }
                    else {
                        info->pArr[i][j].green = 255;
                    }
                } else {
                    info->pArr[i][j].green = info->pArr[i][j].green + tint;
                } 

            //red logic NO CLAMPING
                if (((info->pArr[i][j].red + tint) < 0) || ((info->pArr[i][j].red + tint) > 255)){
                    
                    if (((info->pArr[i][j].red + tint) < 0)){
                        info->pArr[i][j].red = 0;
                    }
                    else {
                        info->pArr[i][j].red = 255;
                    }
                } else {
                    info->pArr[i][j].red = info->pArr[i][j].red + tint;
                } 
            for (int k = 0; k < info->holeCount; k++){
                resultx = ((info->hArr[k]->x - i)*(info->hArr[k]->x - i));
                resulty = ((info->hArr[k]->y - j)*(info->hArr[k]->y - j));
                total = resultx + resulty;
                distance = sqrt(total);
                if (distance < info->hArr[k]->radius) {
                    info->pArr[i][j].red = 0;
                    info->pArr[i][j].blue = 0;
                    info->pArr[i][j].green = 0;
                }
                resultx = 0;
                resulty = 0;
                total = 0;
                distance = 0;
            }
            /*
            resultx = ((x-i)*(x-i));
            resulty = ((y-j)*(y-j));
            total = resultx + resulty;
            distance = sqrt(total);
            if (distance < r) {
                pArr[i][j].red = 0;
                pArr[i][j].blue = 0;
                pArr[i][j].green = 0;
            }
            */
        }
    } 
    pthread_exit(0); 
}