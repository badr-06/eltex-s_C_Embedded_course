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

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    address.sin_port = htons(port);

    char buffer[100];
    memset(buffer, 0, 100);
    strcpy(buffer, "Hi! from client");
    sendto(client_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&address, addrlen);

    int size = recvfrom(client_fd, buffer, 99, 0, NULL, NULL);
    buffer[size] = '\0';
    printf("%s\n", buffer);

    close(client_fd);

    return 0;
}