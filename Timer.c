#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void handle_SIGALRM();    //Function Prototype

int main(int argc, char *argv[]) {
    int pID;
    signal(SIGALRM, handle_SIGALRM);
    int timerIn = atoi(argv[1]);

    if ((pID = fork()) == 0) {
        printf("<%d seconds pause>\n", timerIn);
        sleep(timerIn); 
        //Sending signal to parent process
        kill(getppid(), SIGALRM); 
    }
    else {
        printf("alarm application running\n"); 
        printf("waiting for alarm to go off\n");
        pause();
        return 0;
    }
}

//Handling SIGALRM
void handle_SIGALRM() {
    printf("Ding!\n");
    printf("Done!\n");
}