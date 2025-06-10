/**
* This program takes a file input from user of bmp to ppm type. Reads the
* file and writes it back to a bmp or ppm with an appropriate color shift 
* specified by the user.
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
#include <getopt.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include "BmpProcessor.h"
#include "PixelProcessor.h"

//////////////////////
///MACROS
#define THREAD_COUNT 150

//////////////////////
///GLOBAL VARIABLES
char* bmpExt = ".bmp";
char* cheeseCheck;
char* blurCheck;

//////////////////////
///FORWARD DECLARATIONS
void startBMPThreadBlur(FILE* file_i, FILE* file_o, int threadCount);
void startBMPThreadSwissCheese(FILE* file_i, FILE* file_o, int threadCount);
int getHoleCount(int ImageWidth, int ImageHeight);
int getAvgRadius(int ImageWidth, int ImageHeight);

//////////////////////
///FUNCTIONS
void startBMPThreadBlur(FILE* file_i, FILE* file_o, int threadCount){
    
    //Create BMP Header struct, allocate memory.
    struct BMP_Header* header_BMP = malloc(sizeof(struct BMP_Header));
    readBMPHeader(file_i, header_BMP);
    
    //Pointer to DIB Header.
    struct DIB_Header* header_DIB = malloc(sizeof(struct DIB_Header));
    readDIBHeader(file_i, header_DIB);

    //Create Pixel struct, allocate Memory.
    struct Pixel** pixel = malloc(sizeof(struct Pixel*) * header_DIB->ImageHeight);
    readPixelsBMP(file_i, pixel, header_DIB->ImageWidth, header_DIB->ImageHeight);
    
    //CHECK if thread count works for image width.
    if (threadCount > header_DIB->ImageWidth){
        printf("ThreadCount is larger than Image Width.\n");
        printf("Exiting Program, please define THREAD_COUNT as a number smaller than width\n");
        exit(1);
    }
 
    //start thread creation
    pthread_t tids[threadCount];

    int threadWidth = header_DIB->ImageWidth / threadCount;
    
    //dynamically allocate thread info.
    struct blurInfo** infos = (struct blurInfo**)malloc(sizeof(struct blurInfo*) * threadCount);
  
    for (int i = 0; i < threadCount; i++) {
        infos[i] = malloc(sizeof(struct blurInfo));
        //checks if left side of image.
        if (i == 0){
            infos[i]->pArr = pixel;
            infos[i]->start = 0;
            infos[i]->end = threadWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;
        //checks if end.
        } else if (i == threadCount-1) {
            infos[i]->pArr = pixel;
            infos[i]->start = threadWidth;
            infos[i]->end = header_DIB->ImageWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;
        //in between.
        } else {
            infos[i]->pArr = pixel;
            infos[i]->start = threadWidth;
            threadWidth = threadWidth + (header_DIB->ImageWidth / threadCount);
            infos[i]->end = threadWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;

        }
    } 

    //send off threads and end them.
    for (int i = 0; i < threadCount; i++)
        pthread_create(&tids[i], NULL, boxThreadBlur, infos[i]);
    for (int i = 0; i < threadCount; i++)
        pthread_join(tids[i], NULL);   

    //write BMPheader to Output File
    writeBMPHeader(file_o, header_BMP);
    
    //write DIBHeader to Output File
    writeDIBHeader(file_o, header_DIB);
    
    //write Pixels to Output File
    writePixelsBMP(file_o, pixel, header_DIB->ImageWidth, header_DIB->ImageHeight);
    
    
    //free memory
    
    for (int i = 0; i < threadCount; i++) {
        infos[i]->pArr = NULL;
        free(infos[i]);
               
    }
    free(infos);
    
    PixelDestructor(&pixel,header_DIB->ImageWidth, header_DIB->ImageHeight);
    BMPHeaderDestructor(&header_BMP);
    DIBHeaderDestructor(&header_DIB);
}
/**
 * Description: BMP is Process for converting BMP file to BMP.
 * @param file_i passes file to read from.
 * @param file_o passes file to write to.
 * @param rShift user specified red color pixel shift.
 * @param gShift user specified green color pixel shift.
 * @param bShift user specified blue color pixel shift.
 */

void startBMPThreadSwissCheese(FILE* file_i, FILE* file_o, int threadCount){
    
    //Create BMP Header struct, allocate memory.
    struct BMP_Header* header_BMP = malloc(sizeof(struct BMP_Header));
    readBMPHeader(file_i, header_BMP);
    
    //Pointer to DIB Header.
    struct DIB_Header* header_DIB = malloc(sizeof(struct DIB_Header));
    readDIBHeader(file_i, header_DIB);

    //Create Pixel struct, allocate Memory.
    struct Pixel** pixel = malloc(sizeof(struct Pixel*) * header_DIB->ImageHeight);
    readPixelsBMP(file_i, pixel, header_DIB->ImageWidth, header_DIB->ImageHeight);
    
    //Apply Color Shift. Testing.
    //colorShiftPixels(pixel, header_DIB->ImageWidth, header_DIB->ImageHeight, 10, 10, 0);
    
    //gather and create hole info.
    int holeNum = getHoleCount(header_DIB->ImageWidth, header_DIB->ImageHeight);
    int avgRad = getAvgRadius(header_DIB->ImageWidth, header_DIB->ImageHeight);
    struct Holes** holes = malloc(sizeof(struct Holes*) * holeNum);
    holeCreator(holes, holeNum, avgRad, header_DIB->ImageWidth, header_DIB->ImageHeight);

    //check if thread count is greater than image width.
    if (threadCount > header_DIB->ImageWidth){
        printf("ThreadCount is larger than Image Width.\n");
        printf("Exiting Program, please define THREAD_COUNT as a number smaller than width\n");
        exit(1);
    }
 

    pthread_t tids[threadCount];

    int threadWidth = header_DIB->ImageWidth / threadCount;
    
    //dynamically allocate info for threads.
    struct SwissInfo** infos = (struct SwissInfo**)malloc(sizeof(struct SwissInfo*) * threadCount);
    for (int i = 0; i < threadCount; i++) {
        infos[i] = malloc(sizeof(struct SwissInfo));
        if (i == 0){
            infos[i]->pArr = pixel;
            infos[i]->start = 0;
            infos[i]->end = threadWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;
            infos[i]->holeCount = holeNum;
            infos[i]->hArr = holes;

        } else if (i == threadCount-1) {
            infos[i]->pArr = pixel;
            infos[i]->start = threadWidth;
            infos[i]->end = header_DIB->ImageWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;
            infos[i]->holeCount = holeNum;
            infos[i]->hArr = holes;

        } else {
            infos[i]->pArr = pixel;
            infos[i]->start = threadWidth;
            threadWidth = threadWidth + (header_DIB->ImageWidth / threadCount);
            infos[i]->end = threadWidth;
            infos[i]->imageHeight = header_DIB->ImageHeight;
            infos[i]->imageWidth = header_DIB->ImageWidth;
            infos[i]->holeCount = holeNum;
            infos[i]->hArr = holes;

        }
    }
    
    //start threads and end them.
    for (int i = 0; i < threadCount; i++)
        pthread_create(&tids[i], NULL, threadSwissCheese, infos[i]);
    for (int i = 0; i < threadCount; i++)
        pthread_join(tids[i], NULL); 
    
    //Apply Holes.
    //swissCheese(pixel, holes, holeNum, header_DIB->ImageWidth, header_DIB->ImageHeight);
    //write BMPheader to Output File
    writeBMPHeader(file_o, header_BMP);
    
    //write DIBHeader to Output File
    writeDIBHeader(file_o, header_DIB);
    
    //write Pixels to Output File
    writePixelsBMP(file_o, pixel, header_DIB->ImageWidth, header_DIB->ImageHeight);
    
    //free memory
        for (int i = 0; i < threadCount; i++) {
        infos[i]->pArr = NULL;
        free(infos[i]);
               
    }
    for (int i = 0; i < holeNum; i++) {
        
        free(holes[i]);
               
    }
    free(holes);
    PixelDestructor(&pixel,header_DIB->ImageWidth, header_DIB->ImageHeight);
    BMPHeaderDestructor(&header_BMP);
    DIBHeaderDestructor(&header_DIB);   
}
//helper for hole info.
int getHoleCount(int ImageWidth, int ImageHeight){
    int holeCount;
    if (ImageWidth < ImageHeight) {
        holeCount = ImageWidth / 10;
    }
    else {
        holeCount = ImageHeight / 10;
    }
    return holeCount;
}
//helper for hole infoo.
int getAvgRadius(int ImageWidth, int ImageHeight){
    int avgRad;
    if (ImageWidth < ImageHeight) {
        avgRad = ImageWidth / 10;
    }
    else {
        avgRad = ImageHeight / 10;
    }
    return avgRad;
}
int main(int argc, char* argv[]) {   
    //gcc FayFilters.c BmpProcessor.c PixelProcessor.c -lm -pthread -o reader
    //./reader -i input.bmp -o output.bmp -f c

    char* input;
    char* output;
    char* type;
    int opt;
    while((opt = getopt(argc, argv, ":i:o:f:")) != -1){
        switch(opt){
            case 'i':
                input = optarg;
                break;  
            case 'o':
                output = optarg;
                break;
            case 'f':
                type = optarg;
                break;
            case ':':
                break;
            case '?':
                printf("unknown option: %c\n", optopt);
                break;
            default:
                printf("OK\n");
                break;
        } 
    }

    if (access(input, F_OK) == 0) {
        printf("File OK\n");
    } else {
        printf("Please specify a valid input file path ./reader -i test2.bmp (Then Options -o -f)\n"
                "Program Closing.\n");  
        exit(1);
    }  
    
    cheeseCheck = strstr(type,"c");
    blurCheck = strstr(type,"b");
    
    if (cheeseCheck) {
        FILE* file_input = fopen(input,"rb");
        FILE* file_output = fopen(output,"wb");
        startBMPThreadSwissCheese(file_input, file_output, THREAD_COUNT);
        fclose(file_input);
        fclose(file_output);
    }
    if (blurCheck) {
        FILE* file_input = fopen(input,"rb");
        FILE* file_output = fopen(output,"wb");
        startBMPThreadBlur(file_input, file_output, THREAD_COUNT);
        fclose(file_input);
        fclose(file_output);       
    }
    return 0;
}
