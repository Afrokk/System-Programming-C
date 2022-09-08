#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

void handleCtrlC(int signal); // Handler Function Prototype.

int main(int argc, char *argv[]) {
    int pID, count = 1, status;

    // Setting up signals, ignoring Ctrl+Z & handling Ctrl+C.
    signal(SIGINT, handleCtrlC);
    signal(SIGTSTP, SIG_IGN);

    // Creating child:
    if ((pID = fork()) == 0) {
        printf("Child: Running donothing.c via execlp.\n");
        printf("\ndonothing.c will run in infinite loop as long as this child is running, 1 output = 1 second!\n");
        execlp("./donothing", "donothing", (char *)NULL);
    }
    // Testing as required after exec is executed:
    else {
        printf("Parent waiting for 3 seconds...\n");
        sleep(3);

        /* Sending Ctrl + Z (SIGTSTP) signal to the child process repeatedly in the
        do-while loop to check if it is still ignored or not.*/

        do {
            if ((kill(pID, SIGTSTP)) == 0) {
                printf("Ctrl + Z #%d: Parent %d Sent Ctrl + Z (SIGTSTP) signal to child %d.\n", count, getppid(), pID);
            }
            else {
                printf("Ctrl + Z #%d: Failed to send Ctrl + Z signal.\n", count);
            }
            ++count;
        } while (count <= 3);

        // As nothig happens, we conclude that the SIGTSTP signal is ignored by the child.
        sleep(1);
        printf("\nChild %d ignores Ctrl + Z as it inherits to ignore it from the parent.\n\n", pID);
        sleep(1);
        printf("\nNow, checking if Ctrl + C (SIGINT) is reset to default handler or not:\n");
        printf("Parent will send Ctrl + C (SIGINT) signal to the child now...\n");

        /* Sending Ctrl + C (SIGINT) signal to the child process to check if it is
        reset to the default handler or not. If it is reset, the child process
        should be terminated upon receiving the signal. */

        if ((kill(pID, SIGINT)) == 0) {
            printf("\nParent: Sent Ctrl + C (SIGINT) signal to child.\n");
        }
        else {
            printf("\nFailed to send CtrlC.\n");
        }
        printf("\nChecking if Child is still running or got terminated by SIGINT...\n");
        sleep(1);

        // Calling waitpid() on the child to wait for it to change state.
        waitpid(pID, &status, 0);

        /* Now, sending signal 0 via kill() to the child, to check if it is 
        running or has been terminated. If kill() returns -1, the child
        is terminated.*/
        printf("Child status is %d (Terminated).\n", kill(pID, 0));
    }
    return 0;
}

//Handler function for Ctrl + C.
void handleCtrlC(int signal) {
    printf("Ctrl + C is pressed.\n");
}

//CONCLUSION:
/* As the child process terminates upon receiving the SIGINT signal, we successfully
verify the claim in the slide. After exec() is called by a process, previously
ignored signals remain ignored and any installed handlers are reset back to the default 
handler (reason why child got terminated). Hence, the claims in the slide are 
absolutely correct. */