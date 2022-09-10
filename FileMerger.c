//Afrasiyab Khan
//Student ID 110034991 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>

//Defining global buffer size
#define BUFFER 1000

int main (int argc, char* argv[]) {
    int numFiles;
    //File descriptor for Source Files.
    int fd[numFiles];
    //File descriptor for Output File.
    int fdOutput;
    char fileName[100];

    //Taking number of source files from the user:
    write(1, "Enter the Number of Source Files to be Read: ", sizeof("Enter the Number of Source Files to be Read: "));
    scanf("%d", &numFiles);
    
    //Initializing the source file FDs and checking if they exist
    for (int i = 0; i < numFiles; i++) {
        write(1, "Enter name of the File#", sizeof("Enter name of the File"));
        printf("%d: ", i+1);
        scanf("%s", fileName);
        fd[i] = open(fileName, O_EXCL, S_IRWXU);
        //Error handling
        if(access(fileName, F_OK) == -1) {
            perror("Error reading Source File");
            exit(1);
        }
    }

    //Taking Output File Name:
    write(1, "Enter name of the Output File: ", sizeof("Enter name of the Output File: "));
    scanf("%s", fileName);

    //Creating output file:
    fdOutput = open(fileName, O_CREAT | O_EXCL | O_RDWR, S_IRWXU);

    //Error handling, in case output file already exists:
    if (fdOutput == -1) {
        perror("Error producing output");
        exit(1);
    }

    //Writing from the source files to output file:
    for (int i = 0; i < numFiles; i++) {
        char buffer[BUFFER];
        int bytesRead = read(fd[i], buffer, sizeof(buffer));
        write(fdOutput, buffer, bytesRead);
        //Closing source FDs.
        close(fd[i]);
    }
    //Closing output file.
    close(fdOutput);
    //Completion Prompt:
    write(1, "All source files merged to output. Program Terminating.\n", sizeof("All source files merged to output. Program Terminating.\n"));
    //Terminating program.
    return 0;
}