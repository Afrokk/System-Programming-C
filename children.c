#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void signalHandler(int sig);

int main(int argc, char *argv[]) {
    int pipeFD[2];
    int FIFOfd;
    int status;
    int cp1, cp2, cp3;
    int SUM1, SUM2, SUM3;

    signal(SIGUSR1, signalHandler);

    pipe(pipeFD);

    unlink("../child2fifo"); 

            if (mkfifo("../child2fifo", 0777) != 0) {
                printf("Error creating FIFO in Chhild #2.\n");
                exit(1);
            }

    if ((cp1 = fork()) < 0) {
        perror("Failed to create child process #1.\n");
    }
    else if (cp1 == 0) {
        //Child Process #1 Uses PIPE to communicate S = 2 to the parent process.
        printf("This is child 1 %d, with parent ID %d\n", getpid(), getppid());
        printf("Sending SUM1 (S) to parent process via pipe()..\n\n");
        //Closing read end of pipe as child won't read.
        close(pipeFD[0]);
        int SUM1 = 2;
        //Writing to pipe
        write(pipeFD[1], &SUM1, sizeof(int));
        close(pipeFD[1]);
        kill(SIGCHLD, getppid());
    }
    else {
        if ((cp2 = fork()) < 0) {
            perror("Failed to create child process #2.\n");
        }
        else if (cp2 == 0) {
           //Child Process #2 Uses FIFOs to communicate M = 7 to the parent process.
            sleep(1);
            printf("This is child 2 %d, with parent ID %d\n", getpid(), getppid());
            printf("Sending SUM2 (M) to parent process via FIFOs..\n\n");

            int SUM2 = 7;

            if ((FIFOfd = open("../child2fifo", O_WRONLY)) < 0) {
                printf("Error writing to FIFO.\n");
                exit(1);
            }
            //Writing
            write(FIFOfd, &SUM2, sizeof(int));
            close(FIFOfd);
            kill(SIGCHLD, getppid());
        }
        else {
             if ((cp3 = fork()) < 0) {
                perror("Failed to create child process #3.\n");
            }
            else if (cp3 == 0) {
                //Child Process #3 Uses Signals to communicate L = 19 to the parent process.
                sleep(2);
                printf("This is child 3 %d, with parent ID %d\n", getpid(), getppid());
                printf("Sending SUM3 (L) to parent process via Signals.\n\n");

                int SUM3 = 19;

                pause();
                //Terminating Child#3, with EXIT status value SUM3 (L).
                exit(SUM3);
            }
            else {
                //Main Code.
                //Waiting for Child#1 to terminate.
                waitpid(cp1, &status, 0);
                //Closing write end of pipe as parent won't write.
                close(pipeFD[1]);
                read(pipeFD[0], &SUM1, sizeof(int));
                close(pipeFD[0]);
                sleep(1);

                //Opening FIFO.
                if ((FIFOfd = open("../child2fifo", O_RDONLY)) < 0) {
                    printf("Error reading from FIFO.\n");
                    exit(1);
                }

                //Waiting for Child#2 to terminate.
                waitpid(cp2, &status, 0);
                read(FIFOfd, &SUM2, sizeof(int));
                close(FIFOfd);
                
                sleep(1);
                
                //Sending signal to Child#3 to wake up from pause()
                kill(cp3, SIGUSR1);
                waitpid(cp3, &status, 0);

                //Capturing exit status of Child#3
                if (WIFEXITED(status)) {
                    SUM3 = WEXITSTATUS(status);
                }
                //Outputting the SUMs:
                printf("Number received from child 1 via pipe is, SUM1 = %d\n", SUM1);
                printf("Number received from child 2 via FIFOs is, SUM2 = %d\n", SUM2);
                printf("Number received from child 3 via Signals is, SUM3 = %d\n", SUM3);
            }
        }
    }
    return 0;
}

void signalHandler(int sig) {
    //do nothing
}
