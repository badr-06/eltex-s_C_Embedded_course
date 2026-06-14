#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int main(){

    int client_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int port = 1234;

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    connect(client_fd, (struct sockaddr*)&address, addrlen);

    char buffer[256];
    memset(buffer, 0, strlen(buffer));
    int size = recv(client_fd, buffer, 99, 0);
    buffer[size] = '\0';
    printf("%s", buffer);
    close(client_fd);

    return 0;
}