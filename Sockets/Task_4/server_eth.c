#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(){

    int server_fd;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(address);
    int port = 1234;
    int opt = 1;

    server_fd = socket(AF_INET, SOCK_DGRAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("192.168.31.189");
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));

    char buffer[1024];
    memset(buffer, 0, 1024);
    int size = recvfrom(server_fd, buffer, 1023, 0, (struct sockaddr*)&client_addr, &addrlen);
    buffer[size] = '\0';

    printf("%s\n", buffer);
    
    buffer[0] = 'S';
    printf("%s\n", buffer);
    if(sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addrlen) == -1){
        perror("sendto()");
        exit(1);
    }

    close(server_fd);

    return 0;
}