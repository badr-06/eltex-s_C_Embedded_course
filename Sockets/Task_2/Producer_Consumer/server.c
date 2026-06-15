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
#define MAX_QUEUE 20

int server_fd;
pthread_t thread[MAX_CLIENTS];
int queue[MAX_QUEUE] = {0};
int front = 0, back = 0, count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void signalHandler(int sig) {
 
    for(int i = 0; i < MAX_CLIENTS; ++i){
        pthread_cancel(thread[i]);   
    }

  close(server_fd);
  exit(sig);
}

int enqueue(int client_fd)
{

    pthread_mutex_lock(&mutex);
    if(count >= MAX_QUEUE){
        pthread_mutex_unlock(&mutex);
        printf("Отказ клиенту, все потоки заняты\n");
        return 1;
    }

    queue[back] = client_fd;
    back = (back + 1) % MAX_QUEUE;
    count++;
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    return 0;
}

int dequeue()
{
    int client_fd = queue[front];
    front = (front + 1) % MAX_QUEUE;
    count--;

    return client_fd;
}


void* serve_clients(void *)
{
    while(1){
    pthread_mutex_lock(&mutex);
    while (count == 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    
    int client_fd = dequeue();
    pthread_mutex_unlock(&mutex);
    
    time_t now = time(NULL);
    char buffer[256];
    sprintf(buffer, "%s", ctime(&now));

    if(send(client_fd, buffer, strlen(buffer), 0) == -1){
        perror("send()");
    }
    close(client_fd);
    }
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
    if(server_fd == -1){
        perror("socket()");
        exit(1);
    }

    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))== -1){
        perror("setsockopt()");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr*)&address, addrlen) == -1){
        perror("bind()");
        exit(1);
    }

    if(listen(server_fd, 10) == -1){
        perror("listen()");
        exit(1);
    }

    for(int i = 0; i < 10; ++i){
        if(pthread_create(&thread[i], NULL, serve_clients, NULL) != 0){
            fprintf(stderr, "pthread[%d]", i);
            continue;
        }
        pthread_detach(thread[i]);
    }

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_addrlen = sizeof(client_addr);   
        
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addrlen);
        if(client_fd == -1){
            perror("accept()");
            continue;
        }        

        if(enqueue(client_fd) != 0){
            close(client_fd);
        }

    }
    
    close(server_fd);

    return 0;
}