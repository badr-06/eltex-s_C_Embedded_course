#ifndef LIBRARY_H
#define LIBRARY_H

#define SIZE_CLIENTS 10
#define SIZE 8192

 union semun {
 int val; /* Значение для SETVAL */
 struct semid_ds *buf; /* Буфер для IPC_STAT, IPC_SET */
 unsigned short * array; /* Массив для GETALL, SETALL */
 struct seminfo *__buf; /* Буфер для IPC_INFO (Зависит от Linux) */
 };

enum { MESSAGE_FROM_SERVER = 1, MESSAGE_FROM_CLIENT, NEW_CLIENT, CLIENT_LEAVE, LOADING_MESSAGE };

struct infoClients {
  char name[20];
  int Pid;
};

struct message {
  char mtext[256];
  long from_message;
  int categoriy_message;
  int to_client;
};

int find_name_client(struct infoClients* infCl, int pid, int size);

#endif