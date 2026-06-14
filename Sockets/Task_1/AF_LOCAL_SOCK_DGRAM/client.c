#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(){

    int client_fd;
    struct sockaddr_un address, address2;
    socklen_t addrlen = sizeof(address);

    client_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    address.sun_family = AF_LOCAL;
    address2.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "socket");
    strcpy(address2.sun_path, "socket2");

    bind(client_fd, (struct sockaddr*)&address2, addrlen);

    char buffer[100];
    memset(buffer, 0, 100);
    strcpy(buffer, "Hello! from client");

    if(sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, addrlen) == -1){
        perror("sendto()");
    }

    memset(buffer, 0, strlen(buffer));
    int size = recvfrom(client_fd, buffer, 99, 0, NULL, NULL);
    if(size == -1){
        perror("recvfrom()");
    }
    buffer[size] = '\0';
    printf("%s\n", buffer);

    close(client_fd);
    unlink("socket2");
    return 0;
}