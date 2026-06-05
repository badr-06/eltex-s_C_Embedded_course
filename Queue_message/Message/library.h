#ifndef LIBRARY_H
#define LIBRARY_H

#define SIZE_CLIENTS 10

enum { MESSAGE_FROM_SERVER = 1, MESSAGE_FROM_CLIENT, NEW_CLIENT, CLIENT_LEAVE };

struct infoClients {
  char name[20];
  int Pid;
};

struct message {
  long mtype;
  char mtext[256];
  long from_message;
  int categoriy_message;
};

int find_name_client(struct infoClients* infCl, int pid, int size);

#endif