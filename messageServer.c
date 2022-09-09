#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int fd, fdWrite;
    char ch;

	//Overwriting if FIFOs already exist.
    unlink("../FIFO"); 
    unlink("../FIFO1");

	//Using two FIFOs.
    if (mkfifo("../FIFO", 0777) != 0)  //For client-to-server comms
        exit(1);

    if (mkfifo("../FIFO1", 0777) != 0)  //For server-to-client comms
        exit(1);

    while (1) {
        fprintf(stderr, "Waiting for a client\n");
		//To receive communications from client:
        fd = open("../FIFO", O_RDONLY);
		//To communicate with client:
        fdWrite = open("../FIFO1", O_WRONLY);
        fprintf(stderr, "Got a client: ");

		//Reading client's data:
        while (read(fd, &ch, 1) == 1) {
            fprintf(stderr, "%c\n", ch);
			//Acknowledgement to the client, via FIFO1
            write(fdWrite, "I received your message\n", sizeof("I received your message\n"));
        }
    }
	close(fd);
	close(fdWrite);
	return 0;
}