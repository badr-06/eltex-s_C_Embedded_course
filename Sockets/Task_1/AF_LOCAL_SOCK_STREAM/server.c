#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/un.h>

int main(){

    int server_fd, client_fd;
    struct sockaddr_un address;
    socklen_t addrlen = sizeof(address);

    server_fd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if(server_fd == -1){
        perror("socket()");
        return -1;
    }

    address.sun_family = AF_LOCAL;
    strcpy(address.sun_path, "socket");

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == -1){
        perror("bind()");
        return -1;
    }

    if(listen(server_fd, 1) == -1){
        perror("listen");
        return -1;
    }

    client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen);

    char buffer[100];
    memset(buffer, 0, 100);
    read(client_fd, buffer, 99);

    printf("%s\n", buffer);
    memset(buffer, 0, strlen(buffer));
    strcpy(buffer, "Hi! from server");

    write(client_fd, buffer, strlen(buffer));

    close(client_fd);
    close(server_fd);
    unlink("socket");

    return 0;
}