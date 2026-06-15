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

    inet_pton(AF_INET, "127.0.0.1", &address.sin_addr.s_addr);

    connect(client_fd, (struct sockaddr*)&address, addrlen);

    char buffer[100];
    memset(buffer, 0, 100);
    strcpy(buffer, "Hello! from client");

    send(client_fd, buffer, strlen(buffer), 0);

    memset(buffer, 0, strlen(buffer));
    recv(client_fd, buffer, 99, 0);

    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}