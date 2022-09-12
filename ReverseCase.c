#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

void SIGUSER_Handler(int sig);      //Function Prototype.

int main(int argc, char *argv[]) {
    //Process IDs for the two processes.
    pid_t pID, pID2;

    //Message String.
    char stringBuffer[512] = "NULL";

    //Declaring two pipes.
    int sendPipe[2];
    int returnPipe[2];

    //Signal handler for process communication.
    signal(SIGUSR1, SIGUSER_Handler);

    //Creating two pipes, one for sending and one for receiving.
    if (pipe(sendPipe) == -1) {
            perror("Failed to create sendPipe");
            exit(1);
        }

    if (pipe(returnPipe) == -1) {
        perror("Failed to create returnPipe");
        exit(1);
    }

    /* Spawning two processes. One will send string, other will reverse-case it 
    letter-by-letter and send it back to the first process. */
    if ((pID = fork()) < 0) {
        perror("Failed to create process#1");
        exit(1);
    }
    else if (pID == 0) {
        //Process #1:
        //Paused until parent process signals.
        pause();
        //Reading String entered by the user.
        if (read(sendPipe[0], stringBuffer, 512) == -1) {
            perror("PROCESS#1: Failed to read from sendPipe");
            exit(1);
        }
        //Writing string to the sending pipe, to be read by Process#2.
        //Closing read-end of sendPipe as we don't need it for Process#1.
        close(sendPipe[0]);
        printf("PROCESS#1: Sending String \"%s\" to Process#2\n", stringBuffer);

        //Writing:
        if (write(sendPipe[1], stringBuffer, strlen(stringBuffer)) == -1) {
            perror("Failed to write to sendPipe");
            exit(1);
        }
        close(sendPipe[1]);
        //Reading string from the receiving pipe.
        //Closing write-end of returnPipe as we don't need it for Process#1.
        close(returnPipe[1]);
        if (read(returnPipe[0], stringBuffer, 512) == -1) {
            perror("PROCESS#1: Failed to read from returnPipe");
            exit(1);
        }
        //Displaying the case-reversed result.
        printf("PROCESS#1: The Case-Reversed String Received from Process#2 is: %s\n", stringBuffer);
        close(returnPipe[0]);
        printf("PROCESS#1: Terminating..\n", stringBuffer);
        exit(0);
    }
    else {
        if ((pID2 = fork()) < 0) {
        perror("Failed to create process#2");
        exit(1);
        }
        else if (pID2 == 0) {
            //Process #2:
            //Paused until signalled by main.
            pause();
            //Sleeping for 2 seconds for coordination.
            sleep(2);
            close(sendPipe[1]);
            //Reading user's string sent by Process#1.
            //Main and Process#1 both use sendPipe[0], but Process#1 is paused to avoid race conditions. 
            if (read(sendPipe[0], stringBuffer, 512) == -1) {
                perror("PROCESS#2: Failed to read from sendPipe");
                exit(1);
            }
            close(sendPipe[0]);
            printf("PROCESS#2: Received String \"%s\" from Process#1, Reversing Case...\n", stringBuffer);
            //Reversing the string cases letter-by-letter:
            for (int i = 0; stringBuffer[i] != '\0'; i++) {
                if (stringBuffer[i] >= 'A' && stringBuffer[i] <= 'Z') {
                    stringBuffer[i] += 32;
                }
                else if (stringBuffer[i] >= 'a' && stringBuffer[i] <= 'z') {
                    stringBuffer[i] -= 32;
                }
            }
            //Writing result to returnPipe, to be read by Process#1.
            close(returnPipe[0]);
            if (write(returnPipe[1], stringBuffer, strlen(stringBuffer)) == -1) {
                perror("PROCESS#2: Failed to write to returnPipe");
                exit(1);
            }
            close(returnPipe[1]);
            printf("PROCESS#2: String with Reversed Cases sent to Process#1. Terminating..\n\n");
            exit(0);
        }
        else {
            //Parent process:
            //Closing unused pipes.
            close(sendPipe[0]);
            close(returnPipe[0]);
            close(returnPipe[1]);
            printf("Enter a String Message: ");
            //Getting string input:
            fgets(stringBuffer, sizeof(stringBuffer), stdin);
            //Appending a \0 to the end of the String to signal the end of the message string.
            stringBuffer[strlen(stringBuffer)-1]='\0';
            //Writing to be read by Process#1.
            if (write(sendPipe[1], stringBuffer, strlen(stringBuffer)) == -1) {
                perror("Failed to write to sendPipe");
                exit(1);
            }
            close(sendPipe[1]);
            //Signalling both child processes to wake up from pause();
            kill(pID, SIGUSR1);
            kill(pID2, SIGUSR1);
            //Waiting for both child processes to terminate.
            waitpid(pID, NULL, 0);
            waitpid(pID2, NULL, 0);
            //end
            return 0;
        }
    }
}  

//Custom Signal Handler Function to invoke SIGCONT from pause() state
void SIGUSER_Handler(int sig) {
    //do nothing
}
