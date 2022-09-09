#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void SIGPIPEhandler(int signal);   //Handler Function prototype.

extern int errno;

//Sender Program using named pipe (FIFO).
int main(int argc, char* argv[]){  
	int fd, writeReturn;

    //Setting up handler.
    signal(SIGPIPE, SIGPIPEhandler);

    // overwrite FIFO, if it exists. 
	unlink("../SIGPIPEfifo"); 

	if(mkfifo("../SIGPIPEfifo", 0777) != 0) {
        printf("Error creating FIFO.\n");
		exit(1);
    }

    //Opening FIFO for write-only.
	if ((fd = open("../SIGPIPEfifo", O_WRONLY)) < 0) {
        printf("Error connecting.\n");
		exit(1);
    }

    printf("Connected with Receiver.\n");
    sleep(1);
    
    //Attempting to write to FIFO.
    writeReturn = write(fd, "Test.\n", strlen("Test.\n"));

    //Printing error after catching the SIGPIPE:
    fprintf(stderr, "Write statement returned %d with error number %d, with description \"%s\".\n", writeReturn, errno, strerror(errno));
    printf("The write statement invoked a SIGPIPE signal which was caught by the handler above.\n");
    sleep(1);

    //Closing FIFO.
    close(fd);

    //End of program
    printf("\nMain Terminating.\n");
    return 0;
}

//Handler function
void SIGPIPEhandler(int signal) {
    printf("** SIGPIPE caught while writing to FIFO. **\n");
}
