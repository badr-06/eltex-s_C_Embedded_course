#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>

int tcp_fd, udp_fd;

void signalHandler(int sig) {

    close(tcp_fd);
    close(udp_fd);
    exit(sig);
}

int main(){
    signal(SIGINT, signalHandler);

    
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    int port = 1234;

    tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(tcp_fd == -1){
        perror("socket()");
        exit(1);
    }

    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(tcp_fd == -1){
        perror("socket()");
        exit(1);
    }
    
    if(setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))== -1){
        perror("setsockopt()");
    }

    if(setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))== -1){
        perror("setsockopt()");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(tcp_fd, (struct sockaddr*)&address, addrlen) == -1){
        perror("bind()");
        exit(1);
    }

    if(bind(udp_fd, (struct sockaddr*)&address, addrlen) == -1){
        perror("bind()");
        exit(1);
    }

    if(listen(tcp_fd, 2) == -1){
        perror("listen()");
        exit(1);
    }

    while (1)
    {
        
        fd_set readfds;

        FD_ZERO(&readfds);
        FD_SET(tcp_fd, &readfds);
        FD_SET(udp_fd, &readfds);

        int max_fd = tcp_fd > udp_fd ? tcp_fd : udp_fd;

        int ready = select(max_fd + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(tcp_fd, &readfds)){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);
            
            int client_fd = accept(tcp_fd, (struct sockaddr*)&client_addr, &client_addrlen);
            time_t now = time(NULL);
            char *buffer = ctime(&now);
            
            send(client_fd, buffer, strlen(buffer), 0);

            close(client_fd);
        }

        if(FD_ISSET(udp_fd, &readfds)){
            struct sockaddr_in client_addr;
            socklen_t client_addrlen = sizeof(client_addr);

            char buffer[256];
            memset(buffer, 0, 256);
            recvfrom(udp_fd, buffer, 256, 0, (struct sockaddr*)&client_addr, &addrlen);

            memset(buffer, 0, strlen(buffer));
            time_t now = time(NULL);
            sprintf(buffer, "%s", ctime(&now));

            sendto(udp_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addrlen);

        }
    }
    
    close(tcp_fd);
    close(udp_fd);

    return 0;
}