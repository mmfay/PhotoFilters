# PhotoFilters

## To Compile

Run the following command to compile

gcc FayFilters.c BmpProcessor.c PixelProcessor.c -lm -pthread -o reader

## To Run

Run the following command to run

./reader -i photos/test3.bmp -o test3out.bmp -f c

-i is the input file, 
-o is the name of the output file. Make sure to have .bmp at the end. 
-f is the filter type, choices are either c for swiss cheese or b for blur.

Thread Count Defined under macro THREAD_COUNT (line 25 of Fay Filters) per instructions.
Program will exit if THREAD_COUNT is greater than Image Width.



