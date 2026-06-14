#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

int server_fd;

void signalHandler(int sig) {
 
  close(server_fd);
  exit(sig);
}


void* serve_clients(void *arg)
{
    int client_fd = *((int*)arg);
    free(arg);

    time_t now = time(NULL);
    char buffer[256];
    sprintf(buffer, "%s", ctime(&now));

    send(client_fd, buffer, strlen(buffer), 0);
    close(client_fd);
    
    return NULL;
}

int main(){
    signal(SIGINT, signalHandler);

    int server_fd;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    int opt = 1;
    int port = 1234;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, addrlen);

    listen(server_fd, 10);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);   
        
        int tmp = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);

        int *client_fd = calloc(1, sizeof(int));
        *client_fd = tmp;

        pthread_t thread;
        if(pthread_create(&thread, NULL, serve_clients, (void*)client_fd) != 0){
            perror("pthread_create");
            close(*client_fd);
            free(client_fd);
            continue;
        }
        pthread_detach(thread);
    }
    
    close(server_fd);

    return 0;
}