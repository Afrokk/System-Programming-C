#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

//Defining buffer limit for inter-process comms
#define LIMIT 50

int main (int argc, char *argv[]) {
    int fd, fdRead;
    char ch;
    char buffer[LIMIT];
   
    //Error handling
    while ((fd = open("../FIFO", O_WRONLY)) == -1) {
        fprintf(stderr, "trying to connect\n");
        sleep(1);
    }

    //Error handling
    while((fdRead = open("../FIFO1", O_RDONLY)) == -1) {
        fprintf(stderr, "Read Error.\n");
        exit(1);
    }

    printf("Connected: type in data to be sent\n");


    /*Reading from the FIFO. Using scanf with %*c argument to read 
      from the stream but not further store it in the args*/ 

    while (scanf("%c%*c", &ch) != -1) {
        write(fd, &ch, 1);
        //Reading from FIFO1, for server's acknowledgement
        read(fdRead, buffer, LIMIT);

        //Printing char-by-char
        for (int i = 0; i < strlen(buffer); i++) {
            printf("%c", buffer[i]);
        }
    }
    //closing FIFOs
    close(fd);
    close(fdRead);
    return 0;
}