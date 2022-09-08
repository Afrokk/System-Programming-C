#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

    FILE* fpIn; 
    FILE* fpOut;
    char readChar;

    fpOut = fopen(argv[--argc], "w");    //Initializing output File Ptr. 

    //Error handling for output file.
    if (fpOut == NULL) {
        puts("Error outputting the file.");
        exit(1);
    }

    for (int i = 1; i <= argc - 1; i++) {

        //Opening files in sequence as passed in by the args.
        fpIn = fopen(argv[i], "r");

        //Error handling for input files.
        if (fpIn == NULL) {
            puts("Error, file not found.");
            exit(2);
        }

        //Reading input files byte by byte and writing to output.
        while ((readChar = fgetc(fpIn)) != EOF) {
            fprintf(fpOut, "%c", readChar);
        }
        fclose(fpIn);
    }
    fclose(fpOut);
    exit(0);
}