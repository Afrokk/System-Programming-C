#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <signal.h>

#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int socketFD, newFD, idx = 0;
    int n, Diff;
    char buf[2];

    struct sockaddr_in server = {AF_INET, 1124, INADDR_ANY};

    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create server Socket.\n");
        exit(1);
    }


    if (bind(socketFD, (struct sockaddr *)&server, SIZE) == -1) {
        perror("Failed to bind.\n");
        exit(1);
    }
    
    if (listen(socketFD, 6) == -1) {
        perror("Listen failed.\n");
        exit(1);
    }
    
    if ((newFD = accept(socketFD, NULL, NULL)) == -1) {
        perror("Failed to accept.\n");
    }

    printf("Client connected successfully.\n");
    sleep(1);

    if (fork() == 0) {

        //Receiving Integers
        for (int i = 0; i < 2; i++) {
            if (recv(newFD, &n, sizeof(int), 0) > 0) {
                buf[i] = n;
            }
        }
        
        printf("The two integers received are: %d %d.\n", buf[0], buf[1]);
        printf("I will find the difference between them and let the client know.\n");

        Diff = buf[0] - buf[1];

        //Sending Difference
        send(newFD, &Diff, sizeof(int), 0); 

        printf("Done. Server terminated.\n");
        close(newFD);
        close(socketFD);
    }
    
    return 0;
}