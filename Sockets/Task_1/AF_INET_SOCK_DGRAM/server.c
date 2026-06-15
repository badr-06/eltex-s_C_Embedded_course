#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

int main(){

    int server_fd;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(address);
    int port = 1234;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    char buffer[100];
    memset(buffer, 0, 100);
    recvfrom(server_fd, buffer, 99, 0, (struct sockaddr*)&client_addr, &addrlen);

    printf("%s\n", buffer);
    strcpy(buffer, "Hi! from server");

    sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addrlen);

    close(server_fd);

    return 0;
}