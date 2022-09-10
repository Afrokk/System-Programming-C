#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int pID, pID2, numRead;
    int fd[2];

    //Creating the pipe.
    if ((pipe(fd)) < 0) {
        perror("Failed to create pipe.\n");
    }

    //Creating first child.
    if ((pID = fork()) < 0) {
        perror("Failed to create child process #1.\n");
    }
    else if (pID == 0) {

        /* Randomizing the seed to get a true pseudo random number generator. This ensures
        that the seed for the random number generator is different everytime the program is 
        executed. */

        srand(time(NULL));

        //Closing the read end of the pipe for child #1, as we don't need it.
        close(fd[0]);

        //Generating 10 random numbers and writing them onto the pipe.
        for (int i = 0; i < 5; i++) {
            int numGen = (rand() % (99 - 0 + 1)) + 0;
            write(fd[1], &numGen, sizeof(int));
        }
        //Closing the write end of the pipe for child #1. 
        close(fd[1]);
    }
    else {
        //Creating second child.
        if ((pID2 = fork()) < 0) {
            perror("Failed to create child process #2.\n");
        }
        else if (pID2 == 0) {

            /*Child #2 sleeps for 1 second. This makes sure that a new seed value is
            generated for the random number generator by the time child #2 starts
            executing. */
            sleep(1);

            //Again, randomizing the srand seed with time() function.
            srand(time(NULL));

            //Closing the read end of the pipe for child #2, as we don't need it.
            close(fd[0]);

            //Generating 10 random numbers and writing them onto the pipe.
            for (int i = 0; i < 5; i++) {
                int numGen = (rand() % (99 - 0 + 1)) + 0;
                write(fd[1], &numGen, sizeof(int));
            }

            //Closing the write end of the pipe for child #2. 
            close(fd[1]);
        }
        else {
            //Closing the write end of the pipe for the main parent process. 
            close(fd[1]);

            //Formatting
            printf("Reading the Randomly Generated Numbers from Pipe:\n");

            //Reading from the pipe and printing the 10 random numbers.
            for (int i = 1; i < 11; i++) {
                read(fd[0], &numRead, sizeof(int));
                printf("Number #%d = %d\n", i, numRead);
            }
            //Closing the read end of the pipe for the main parent process.
            close(fd[0]);
        }
        return 0;
    }
}