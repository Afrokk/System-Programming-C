#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void handlerFunc(int signal); //Function Prototype.

int main(int argc, char *argv[]) {
    //Installing handler for SIGINT and SIGQUIT signals.
    signal(SIGINT, handlerFunc);
    signal(SIGQUIT, handlerFunc);

    //Passing the given signals to the program so it invokes the handler function.
    kill(getpid(), SIGINT);
    kill(getpid(), SIGQUIT);
}

/* Handler function catches the signals from the kill() function in Main, and ouputs the 
signal number. */

void handlerFunc(int signal) {
    if (signal == SIGINT) {
        printf("SIGINT caught.\n");
    }
    else {
        printf("SIGQUIT caught.\n");
    }
    printf("Signal Number: %d\n\n", signal);
}