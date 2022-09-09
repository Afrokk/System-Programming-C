#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

int main (int argc, char* argv[]) {   
	int fd;

	//Opening FIFO for read-only.
	while((fd = open("../SIGPIPEfifo", O_RDONLY)) < 0) {  
		printf("trying to connect\n");  
		sleep(1);
	}
  
    printf("Connected with Sender.\n");

	/*After connecting, the Receiver program closes the read end of the FIFO.
	This results in a broken pipe, and a SIGPIPE signal is invoked when the 
	Sender program attempts to write to the disconnected FIFO. */

	//Closing read-end of the FIFO.
    close(fd);
    printf("Closed read end of the FIFO, program terminating\n.");
    return 0;
}