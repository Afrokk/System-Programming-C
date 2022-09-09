#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

    //Initializing two file pointers.
    FILE* fp = fopen(argv[1], "r");
    FILE* fpOut = fopen(argv[2], "w");

    const int bufferSize = 3000;        //Global buffer.
    const int lineLength = 200;         //Line buffer.
    int bytesRead = 0, len, i = 0;

    char bufferArr[lineLength];
    int linePtr[bufferSize];

    //Error handling
    if (fp == NULL || fpOut == NULL) {
        printf("Error reading/writing file.\n");
        exit(1);
    }

    linePtr[i] = 0;                     //Initializing 0th index to zero.

    //Reading the input file line-by-line.
    while (fgets(bufferArr, sizeof(bufferArr), fp)) {
        len = strlen(bufferArr);        //Storing the length of the line read in len variable.
        bytesRead += len;               //Accounting for total no. of bytes read.

        /* Incrementing i for indexing in the linePtr array, as we're going to store 
           the nth byte in linePtr array, pointing to the start of each line in the
           input file. */

        i++;                           
        linePtr[i] = bytesRead;         //Storing the specific  no. of bytes (index) at which each new line starts.     
    }
    
    rewind(fp);                         //Seeking back to file start.
    --i;                                //Decrementing i to account for newline character.

    //Again, reading input line-by-line and writing to output file, in reverse.
    while (i >= 0) {
        fseek(fp, linePtr[i], 0);       //Seeking to nth byte where new line each line starts.
        fgets(bufferArr, sizeof(bufferArr), fp);  
        fprintf(fpOut, "%s", bufferArr); 
        i--;                            //Decrementing i to output lines in reverse.
    }

    fclose(fp);
    fclose(fpOut);
    exit(0);
}