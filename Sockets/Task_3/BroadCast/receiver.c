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

    if(setsockopt(socket_fd, SOL_SOCKET, SO_BROADCAST, &opt, sizeof(opt))== -1){
        perror("setsockopt()");
    }
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))== -1){
        perror("setsockopt()");
    }

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    inet_pton(AF_INET, "255.255.255.255", &address.sin_addr.s_addr);

    if(bind(socket_fd, (struct sockaddr*)&address, addrlen) == -1){
        perror("bind()");
        exit(1);
    }

    char buffer[256];
    memset(buffer, 0, 256);
    for(int i = 0; i < 10; ++i){
        
        int size = recvfrom(socket_fd, buffer, 255, 0, NULL, NULL);
        buffer[size] = '\0';
        printf("%s\n", buffer);
    }
    close(socket_fd);
    return 0;
}