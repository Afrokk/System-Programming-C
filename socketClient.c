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

#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
    int SUMs[2];
    SUMs[0] = 19;
    SUMs[1] = 2;
    int Diff;
    int socketFD;
    
    struct sockaddr_in server = {AF_INET, 1124};
    server.sin_addr.s_addr = inet_addr(argv[1]); 

    if ((socketFD = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Failed to create client Socket.\n");
        exit(1);
    }
    
    if (connect(socketFD, (struct sockaddr *)&server, SIZE) == -1) {
        perror("Failed to connect Sockets.\n");
        exit(1);
    }
    
    printf("I am sending the two integers: %d, %d to the server.\n", SUMs[0], SUMs[1]);
    
    //Sending Integers
    send(socketFD, &SUMs[0], sizeof(int), 0);
    send(socketFD, &SUMs[1], sizeof(int), 0);


    //Receiving Difference
    recv(socketFD, &Diff, sizeof(int), 0);

    printf("The server told me %d is their difference.\n", Diff);

    printf("Thank you. Client terminated.\n");
    close(socketFD);
    return 0;
}
