#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/un.h>

int main(){

    int client_fd;
    struct sockaddr_un address;
    socklen_t addrlen = sizeof(address);

    client_fd = socket(AF_LOCAL, SOCK_STREAM, 0);

    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "socket");

    connect(client_fd, (struct sockaddr*)&address, addrlen);

    char buffer[100];
    memset(buffer, 0, 100);
    strcpy(buffer, "Hello! from client");

    write(client_fd, buffer, strlen(buffer));

    memset(buffer, 0, strlen(buffer));
    int size = read(client_fd, buffer, 99);

    printf("%s\n", buffer);

    close(client_fd);
    return 0;
}