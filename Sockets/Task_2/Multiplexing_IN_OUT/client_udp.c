#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

int main(){

    int client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int port = 1234;

    client_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(client_fd == -1){
        perror("socket()");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);

    char buffer[256];
    memset(buffer, 0, 256);
    strcpy(buffer, "hi");
    if(sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, addrlen) == -1){
        perror("sendto()");
        exit(1);
    }
    
    int size = recvfrom(client_fd, buffer, 255, 0, NULL, NULL);
    buffer[size] = '\0';
    printf("%s", buffer);

    close(client_fd);

    return 0;
}