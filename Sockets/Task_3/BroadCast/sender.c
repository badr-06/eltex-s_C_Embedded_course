#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>
#include <arpa/inet.h>

int main(){

    int socket_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    int port = 7777;

    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(socket_fd == -1){
        perror("socket()");
        exit(1);
    }

    setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    inet_pton(AF_INET, "255.255.255.255", &address.sin_addr.s_addr);

    
    char buffer[256];
    memset(buffer, 0, 256);
    strcpy(buffer, "Hello! from server");
    for(int i = 0; i < 10; ++i){
        sendto(socket_fd, buffer, 255, 0, (struct sockaddr*)&address, addrlen);
    }

    close(socket_fd);
    return 0;
}