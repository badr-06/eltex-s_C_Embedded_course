#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <stdbool.h>

#define MAX_CLIENTS 10

int server_fd;
bool idx_threads[MAX_CLIENTS] = {true, true, true, true, true, true, true, true, true, true};
pthread_t thread[MAX_CLIENTS];
int count = 0, fd_socket;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond[10] = {PTHREAD_COND_INITIALIZER};

void signalHandler(int sig) {
 
    for(int i = 0; i < MAX_CLIENTS; ++i){
        pthread_cancel(thread[i]);   
    }

  close(server_fd);
  exit(sig);
}


void* serve_clients(void *arg)
{
    int index = *(int*)arg;
    free((int*)arg);

    printf("4\n");
    while(1){
    pthread_mutex_lock(&mutex);
    while (count == 0)
    {
        pthread_cond_wait(&cond[index], &mutex);
    }
    printf("5\n");
    int client_fd = fd_socket;
    pthread_mutex_unlock(&mutex);
    
    time_t now = time(NULL);
    char buffer[256];
    sprintf(buffer, "%s", ctime(&now));

    send(client_fd, buffer, strlen(buffer), 0);
    close(client_fd);
    pthread_mutex_lock(&mutex);
    count--;
    idx_threads[index] = true;
    pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int main(){
    signal(SIGINT, signalHandler);

    
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

    for(int i = 0; i < 10; ++i){
        int *idx = calloc(1, sizeof(int));
        *idx = i;

        pthread_create(&thread[i], NULL, serve_clients, (void*)idx);
        pthread_detach(thread[i]);
    }

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);   
        
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);
        printf("1\n");
        int idx = -1;
        pthread_mutex_lock(&mutex);
        for(int i = 0; i < MAX_CLIENTS; ++i){
            if(idx_threads[i]) {
                idx = i;
                idx_threads[i] = false;
                break;
            }
        }
        if(idx == -1){
            pthread_mutex_unlock(&mutex);
            printf("Отказ клиенту, все потоки заняты\n");
            close(client_fd);
            continue;
        }   
        printf("2\n");
        count++;
        fd_socket = client_fd;
        pthread_mutex_unlock(&mutex);
        printf("idx = %d 3\n", idx);
        pthread_cond_signal(&cond[idx]);

    }
    
    close(server_fd);

    return 0;
}