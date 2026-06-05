#include <fcntl.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>

#include "library.h"

struct storeMessage {
  char** message;
  int* fromMessage;
  int size;
  int capacity;
};

struct storeMessage* storeMsg_ptr = NULL;

int sizeInfcl = 0;
int msgid[2];

void initStoreMessage(struct storeMessage* storeMsg) {
  storeMsg->capacity = 10;
  storeMsg->size = 0;
  storeMsg->message = malloc(storeMsg->capacity * sizeof(char*));
  if (storeMsg->message == NULL) {
    perror("malloc()");
    exit(1);
  }
  storeMsg->fromMessage = calloc(storeMsg->capacity, sizeof(int));
}

void deleteStoreMessage(struct storeMessage* storeMsg) {
  for (int i = 0; i < storeMsg->size; ++i) {
    free(storeMsg->message[i]);
  }
  free(storeMsg->message);
  free(storeMsg->fromMessage);
}

void signalHandler(int sig) {
  msgctl(msgid[0], IPC_RMID, NULL);
  msgctl(msgid[1], IPC_RMID, NULL);
  deleteStoreMessage(storeMsg_ptr);
  exit(sig);
}

void to_send_from_server(struct message* local_msg, struct infoClients* infCl) {
  for (int i = 0; i < sizeInfcl; ++i) {
    local_msg->mtype = infCl[i].Pid;
    msgsnd(msgid[1], local_msg, sizeof(*local_msg) - sizeof(local_msg->mtype),
           0);
  }
}

void read_queue(struct message* local_msg, struct infoClients* infCl,
                struct storeMessage* storeMsg) {
  msgrcv(msgid[0], local_msg, sizeof(*local_msg) - sizeof(local_msg->mtype), 0,
         0);

  if (local_msg->mtype == NEW_CLIENT) {
    if (sizeInfcl >= SIZE_CLIENTS) return;

    infCl[sizeInfcl].Pid = local_msg->from_message;
    memset(infCl[sizeInfcl].name, 0, sizeof(infCl[sizeInfcl].name));
    strncpy(infCl[sizeInfcl].name, local_msg->mtext, 19);
    sizeInfcl++;

  } else if (local_msg->mtype == MESSAGE_FROM_CLIENT) {
    if (storeMsg->size >= storeMsg->capacity - 1) {
      char** temp =
          realloc(storeMsg->message, storeMsg->capacity * 2 * sizeof(char*));
      int* temp_from =
          realloc(storeMsg->fromMessage, storeMsg->capacity * 2 * sizeof(int));
      if (temp == NULL || temp_from == NULL) {
        perror("realloc()");
        exit(1);
      }
      storeMsg->capacity *= 2;
      storeMsg->message = temp;
      storeMsg->fromMessage = temp_from;
    }
    int idx = find_name_client(infCl, local_msg->from_message, sizeInfcl);
    char str[300];
    sprintf(str, "%s: %s", infCl[idx].name, local_msg->mtext);
    storeMsg->message[storeMsg->size] = strdup(str);
    storeMsg->fromMessage[storeMsg->size] = local_msg->from_message;
    strncpy(local_msg->mtext, storeMsg->message[storeMsg->size],
            strlen(storeMsg->message[storeMsg->size]));
    storeMsg->size++;

  } else if (local_msg->mtype == CLIENT_LEAVE) {
    int idx = find_name_client(infCl, local_msg->from_message, sizeInfcl);
    sprintf(local_msg->mtext, "%s: %s %s", "server", infCl[idx].name,
            "left the chat");

    if (idx + 1 < sizeInfcl) {
      for (int i = idx; i < sizeInfcl; i++) {
        infCl[i] = infCl[i + 1];
      }
    }
    sizeInfcl--;
  }
}

void write_queue(struct message* local_msg, struct infoClients* infCl,
                 struct storeMessage* storeMsg) {
  if (local_msg->mtype == NEW_CLIENT) {
    local_msg->categoriy_message = NEW_CLIENT;

    for (int i = 0; i < sizeInfcl; ++i) {
      if (local_msg->from_message == infCl[i].Pid && sizeInfcl > 1) continue;
      local_msg->mtype = infCl[i].Pid;
      if (msgsnd(msgid[1], local_msg,
                 sizeof(*local_msg) - sizeof(local_msg->mtype), 0) == -1) {
        perror("msgsnd() 1");
      }
    }

    local_msg->mtype = local_msg->from_message;
    if (sizeInfcl > 1) {
      for (int i = 0; i < sizeInfcl; ++i) {
        strncpy(local_msg->mtext, infCl[i].name, 19);
        local_msg->from_message = infCl[i].Pid;
        if (msgsnd(msgid[1], local_msg,
                   sizeof(*local_msg) - sizeof(local_msg->mtype), 0) == -1) {
          perror("msgsnd() 2");
        }
      }
    }

    local_msg->categoriy_message = MESSAGE_FROM_CLIENT;
    if (storeMsg->size > 0) {
      for (int i = 0; i < storeMsg->size; ++i) {
        strncpy(local_msg->mtext, storeMsg->message[i],
                strlen(storeMsg->message[i]));
        local_msg->from_message = storeMsg->fromMessage[i];
        if (msgsnd(msgid[1], local_msg,
                   sizeof(*local_msg) - sizeof(local_msg->mtype), 0) == -1) {
          perror("msgsnd() 3");
        }
      }
    }
  } else if (local_msg->mtype == MESSAGE_FROM_CLIENT) {
    local_msg->categoriy_message = MESSAGE_FROM_CLIENT;
    for (int i = 0; i < sizeInfcl; ++i) {
      local_msg->mtype = infCl[i].Pid;
      msgsnd(msgid[1], local_msg, sizeof(*local_msg) - sizeof(local_msg->mtype),
             0);
    }
  } else if (local_msg->mtype == CLIENT_LEAVE) {
    local_msg->categoriy_message = MESSAGE_FROM_SERVER;
    to_send_from_server(local_msg, infCl);
    local_msg->categoriy_message = CLIENT_LEAVE;
    for (int i = 0; i < sizeInfcl; ++i) {
      local_msg->mtype = infCl[i].Pid;
      msgsnd(msgid[1], local_msg, sizeof(*local_msg) - sizeof(local_msg->mtype),
             0);
    }
  }
}

int main() {
  signal(SIGINT, signalHandler);

  key_t key;
  key = ftok(".", 1);
  msgid[0] = msgget(key, 0666 | IPC_CREAT);

  key = ftok(".", 2);
  msgid[1] = msgget(key, 0666 | IPC_CREAT);

  struct storeMessage storeMsg = {0};
  initStoreMessage(&storeMsg);
  storeMsg_ptr = &storeMsg;

  struct message msg = {0};
  struct infoClients infCl[SIZE_CLIENTS];

  while (1) {
    read_queue(&msg, infCl, &storeMsg);
    write_queue(&msg, infCl, &storeMsg);
  }

  if (msgctl(msgid[0], IPC_RMID, NULL) == -1 ||
      msgctl(msgid[1], IPC_RMID, NULL) == -1) {
    perror("Error msgctl()");
  }
  deleteStoreMessage(&storeMsg);

  return 0;
}