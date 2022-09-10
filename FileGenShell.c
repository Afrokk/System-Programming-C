#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int pID, fd, pipeFDs[2], bytesWritten;
    char charRead;

    //Deleting result.txt, if it already exists, for overwriting cleanly.
    remove("result.txt");

    //Opening file descriptor FD, to create the result.txt.
    fd = open("result.txt", O_CREAT | O_WRONLY, S_IRWXU);

    //Creating the pipe.
    if ((pipe(pipeFDs)) < 0) {
        perror("Failed to create pipe.\n");
    }

    //Creating first child.
    if ((pID = fork()) < 0) {
        perror("Failed to create child process #1.\n");
    }
    else if (pID == 0) {

        //Closing read end of the pipe for the child, as we don't need it.
        close(pipeFDs[0]);

        /* Closing stdout and duplicating it to write end of the pipe. So,
           when execlp() executes, it's output is redirected and written
           to the pipe. 
        */
        close(1);

        //Error handling.
        if ((dup(pipeFDs[1])) < 0) {
            perror("Error writing to pipe.\n");
        }

        //Executing the shell command from the args.
        execlp(argv[1], argv[1], (char *)NULL);

        //Closing write end of the pipe for the child as we are done writing.
        close(pipeFDs[1]);
    }
    else {
        //Closing write end of the pipe for the parent, as we don't need it.
        close(pipeFDs[1]);

        //Reading from the pipe.
        while (read(pipeFDs[0], &charRead, sizeof(char)) != 0) {
                bytesWritten += write(fd, &charRead, sizeof(char));
            }

        //Closing read end of the pipe for the child as we are done reading.
        close(pipeFDs[0]);

        //Closing FD as we are done writing to the file.
        close(fd);

        printf("The result of %s is written onto the result.txt with total %d bytes.\n", argv[1], bytesWritten);
        return 0;
    }
}