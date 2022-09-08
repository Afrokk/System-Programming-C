#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	int fd;
	char appendIn[250]; 
	
	//Opening file via command line argument
	fd = open(argv[1], O_RDWR | O_APPEND); 

	//Getting filesize via lseek.
	off_t fileSize = lseek(fd, 0, SEEK_END); 

	//Buffer
	char buffer[fileSize+1];

	//Setting pointer back to beginning of the file. 
	lseek(fd, 0, SEEK_SET);

	//Reading and displaying the file contents. 
	read(fd, buffer, fileSize);
	write(1, buffer, fileSize);
	printf("Enter Data to be Appended in the file: \n");

	//Appendi input
	fgets(appendIn, 250, stdin);
	write(fd, appendIn, fileSize);
	printf("Data appended:\n");
	fileSize = lseek(fd, 0, SEEK_END); 

	//Again, setting seek to the beginning of the file. 
	lseek(fd, 0, SEEK_SET);

	//Printing the whole file agian, after appending.
	read(fd, buffer, fileSize);
	write(1, buffer, fileSize);
	close(fd);
    return 0;
}

