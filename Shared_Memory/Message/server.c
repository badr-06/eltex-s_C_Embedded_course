#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <unistd.h>

#include "library.h"

struct storeMessage {
  int size;
  int capacity;
  char** message;
};

struct storeMessage* storeMsg_ptr = NULL;

int *shared_sizeInfcl;
int shm_fd;
int semf;
sem_t *sem_server, *sem_client, *mutex1, *mutex2, *sem_server2;
char *name = "/MyShared";

void initStoreMessage(struct storeMessage* storeMsg) {
  storeMsg->capacity = 10;
  storeMsg->size = 0;
  storeMsg->message = malloc(storeMsg->capacity * sizeof(char*));
  if (storeMsg->message == NULL) {
    perror("malloc()");
    exit(1);
  }
}

void deleteStoreMessage(struct storeMessage* storeMsg) {
  for (int i = 0; i < storeMsg->size; ++i) {
    free(storeMsg->message[i]);
  }
  free(storeMsg->message);
}

void signalHandler(int sig) {
  sem_close(sem_server);
  sem_close(sem_client);
  sem_close(mutex1);
  sem_close(mutex2);
  sem_unlink("/mutex1");
  sem_unlink("/mutex2");
  sem_unlink("/sem_server");
  sem_unlink("/sem_client");
  close(shm_fd);
  shm_unlink(name);
  deleteStoreMessage(storeMsg_ptr);
  exit(sig);
}

void read_queue(struct message* local_msg, struct message* shared_msg, struct infoClients* infCl,
                struct storeMessage* storeMsg) {
  
  sem_wait(sem_server);
  sem_wait(mutex1);
  *local_msg = *shared_msg;
  sem_post(mutex1);

  if (local_msg->categoriy_message == NEW_CLIENT) {
    sem_wait(mutex1);
    if (*shared_sizeInfcl >= SIZE_CLIENTS) return;
    
    infCl[*shared_sizeInfcl].Pid = local_msg->from_message;
    memset(infCl[*shared_sizeInfcl].name, 0, sizeof(infCl[*shared_sizeInfcl].name));
    strncpy(infCl[*shared_sizeInfcl].name, local_msg->mtext, 19);
    *shared_sizeInfcl += 1;
    sem_post(mutex1);

  } else if (local_msg->categoriy_message == MESSAGE_FROM_CLIENT) {
    if (storeMsg->size >= storeMsg->capacity - 1) {
      char** temp =
          realloc(storeMsg->message, storeMsg->capacity * 2 * sizeof(char*));
      if (temp == NULL) {
        perror("realloc()");
        exit(1);
      }
      storeMsg->capacity *= 2;
      storeMsg->message = temp;
    }
    
    int idx = find_name_client(infCl, local_msg->from_message, *shared_sizeInfcl);
    
    char str[300];
    sprintf(str, "%s: %s", infCl[idx].name, local_msg->mtext);
    storeMsg->message[storeMsg->size] = strdup(str);
    strncpy(local_msg->mtext, storeMsg->message[storeMsg->size], strlen(storeMsg->message[storeMsg->size]));
    storeMsg->size++;
  } else if (local_msg->categoriy_message == CLIENT_LEAVE) {

    sem_wait(mutex1);
    int idx = find_name_client(infCl, local_msg->from_message, *shared_sizeInfcl);
    sprintf(local_msg->mtext, "%s: %s %s", "server", infCl[idx].name,
            "left the chat");

    if (idx + 1 >= *shared_sizeInfcl) {
      *shared_sizeInfcl -= 1;
    } else {
      for (int i = idx; i < *shared_sizeInfcl; i++) {
        infCl[i] = infCl[i + 1];
      }
      *shared_sizeInfcl -= 1;
    }
    sem_post(mutex1);
  }
}

void write_queue(struct message* local_msg, struct message* shared_msg, struct infoClients* infCl,
                 struct storeMessage* storeMsg) {
    
  for(int i = 0; i < *shared_sizeInfcl; ++i){
    sem_wait(sem_server2);
  }
                  
   if (local_msg->categoriy_message == NEW_CLIENT) {
    
    sem_wait(mutex2);
    *shared_msg = *local_msg;
    sem_post(mutex2);

    for(int i = 0; i < *shared_sizeInfcl; ++i){
      sem_post(sem_client);
    }
    sleep(1);    
    if (storeMsg->size > 0) {
      
      local_msg->categoriy_message = LOADING_MESSAGE;
      local_msg->to_client = local_msg->from_message;
      for (int i = 0; i < storeMsg->size; ++i) {
        
        for(int i = 0; i < *shared_sizeInfcl; ++i){
          sem_wait(sem_server2);
        }
        
        sem_wait(mutex2);
        printf("%s\n", storeMsg->message[i]);
        strncpy(local_msg->mtext, storeMsg->message[i],
                strlen(local_msg->mtext) - 1);
        
        *shared_msg = *local_msg;
        sem_post(mutex2);

        for(int i = 0; i < *shared_sizeInfcl; ++i){
          sem_post(sem_client);
        }
        sleep(2);
      }
    }
  } else if (local_msg->categoriy_message == MESSAGE_FROM_CLIENT) {
    sem_wait(mutex2);
    *shared_msg = *local_msg;
    sem_post(mutex2);

    for(int i = 0; i < *shared_sizeInfcl; ++i){
      sem_post(sem_client);
    }
  } else if (local_msg->categoriy_message == CLIENT_LEAVE) {
    local_msg->categoriy_message = MESSAGE_FROM_SERVER;
    sem_wait(mutex2);
    *shared_msg = *local_msg;
    sem_post(mutex2);

    for(int i = 0; i < *shared_sizeInfcl; ++i){
      sem_post(sem_client);
    }

    for(int i = 0; i < *shared_sizeInfcl; ++i){
      sem_wait(sem_server2);
    }

    local_msg->categoriy_message = CLIENT_LEAVE;
    sem_wait(mutex2);
    *shared_msg = *local_msg;
    sem_post(mutex2);
    
    for(int i = 0; i < *shared_sizeInfcl; ++i){
      sem_post(sem_client);
    }
  }

}

int main() {
  signal(SIGINT, signalHandler);

  mutex1 = sem_open("/mutex1", O_CREAT, 0666, 1);
  mutex2 = sem_open("/mutex2", O_CREAT, 0666, 1);
  sem_client = sem_open("/sem_client", O_CREAT, 0666, 0);
  sem_server = sem_open("/sem_server", O_CREAT, 0666, 0);
  sem_server2 = sem_open("/sem_server2", O_CREAT, 0666, 0);

  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, SIZE);

  struct storeMessage storeMsg = {0};
  initStoreMessage(&storeMsg);
  storeMsg_ptr = &storeMsg;

  char *ptr = mmap(0, sizeof(struct message) + sizeof(struct infoClients) * 10 + sizeof(int), \
                  PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  struct message* shared_msg_write = (struct message*)ptr;
  struct infoClients *infCl = (struct infoClients*)(ptr + sizeof(struct message));
  shared_sizeInfcl = (int*)(ptr + sizeof(struct message) + sizeof(struct infoClients) * 10);
  *shared_sizeInfcl = 0;

  struct message *shared_msg_read = (struct message*)mmap(0, sizeof(struct message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, SIZE / 2);

  while (1) {
    struct message local_msg = {0};
    read_queue(&local_msg, shared_msg_read, infCl, &storeMsg);
    write_queue(&local_msg, shared_msg_write, infCl, &storeMsg);
  }

  deleteStoreMessage(&storeMsg);

  return 0;
}