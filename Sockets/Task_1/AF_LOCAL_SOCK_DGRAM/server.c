#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>

int main(){

    int server_fd;
    struct sockaddr_un address, address2;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_LOCAL, SOCK_DGRAM, 0);

    address.sun_family = AF_LOCAL;
    address2.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "socket");
    strcpy(address2.sun_path, "socket2");

    bind(server_fd, (struct sockaddr*)&address, addrlen);

    char buffer[100];
    memset(buffer, 0, 100);
    int size = recvfrom(server_fd, buffer, 99, 0, (struct sockaddr*)&address2, &addrlen);

    printf("%s\n", buffer);
    strcpy(buffer, "Hi! from server");

    if(sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&address2, addrlen) == -1){
        perror("send()");
    }

    close(server_fd);
    unlink("socket");

    return 0;
}