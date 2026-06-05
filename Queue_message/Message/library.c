#include "library.h"

int find_name_client(struct infoClients* infCl, int pid, int size) {
  for (int i = 0; i < size; ++i) {
    if (pid == infCl[i].Pid) return i;
  }

  return -1;
}