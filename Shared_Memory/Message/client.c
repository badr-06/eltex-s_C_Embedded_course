#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <curses.h>

#include "library.h"

int shm_fd;
pid_t pid;
sem_t *sem_server, *sem_client, *mutex1, *mutex2, *sem_server2;
int fp; // потом надо убрать

WINDOW *chat_win;
WINDOW *child_chat_win;
WINDOW *users_win;
WINDOW *input_win;

void view_infClients(struct infoClients *infCl, int size) {
  werase(users_win);
  box(users_win, 0, 0);
  mvwprintw(users_win, 0, 2, " Users ");
  for (int i = 0; i < size; i++) {
    mvwprintw(users_win, i + 2, 2, "%s", infCl[i].name);
  }
  wrefresh(users_win);
}

void *read_queue(void *arg) {
  struct message local_msg = {0};
  char *ptr = mmap(0, sizeof(struct message) + sizeof(struct infoClients) * 10 + sizeof(int), \
                  PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  struct message* shared_msg = (struct message*)ptr;
  struct infoClients *infCl = (struct infoClients*)(ptr + sizeof(struct message));
  int *shared_sizeInfcl = (int*)(ptr + sizeof(struct message) + sizeof(struct infoClients) * 10);
  pid_t pid = *((pid_t *)arg);
  while (1) {

    sem_wait(sem_client);
    sem_wait(mutex2);
    local_msg = (*shared_msg);
    char str[300];
    int size = *shared_sizeInfcl;
    sprintf(str, "size = %d, name = %s\n", size, infCl[0].name);
    write(fp, str, strlen(str));

    sprintf(str, "size = %d, mtext = %s\n", size, local_msg.mtext);
    write(fp, str, strlen(str));
    sem_post(mutex2);

    if (local_msg.categoriy_message == NEW_CLIENT || local_msg.categoriy_message == CLIENT_LEAVE) {

      view_infClients(infCl, size);
    } else if (local_msg.categoriy_message == MESSAGE_FROM_CLIENT || (local_msg.categoriy_message == LOADING_MESSAGE && local_msg.to_client == pid)) {

      wprintw(child_chat_win, "%s\n", local_msg.mtext);
      touchwin(child_chat_win);
      wrefresh(child_chat_win);
      wrefresh(chat_win);
    } else if (local_msg.categoriy_message == MESSAGE_FROM_SERVER) {
      wprintw(child_chat_win, "%s\n", local_msg.mtext);
      touchwin(child_chat_win);
      wrefresh(child_chat_win);
      wrefresh(chat_win);
    }

    sem_post(sem_server2);
  }

  return NULL;
}

void new_client(struct message *msg, char *name) {
  msg->categoriy_message = NEW_CLIENT;
  msg->from_message = pid;
  strncpy(msg->mtext, name, sizeof(msg->mtext) - 1);
  msg->mtext[sizeof(msg->mtext) - 1] = '\0';  

}

void *write_queue(void *arg) {
  struct message *shared_msg = (struct message*)mmap(0, sizeof(struct message), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, SIZE / 2);
  
  struct message new_msg = {0};
  char input[256];
  char *name = (char *)arg;

  new_client(&new_msg, name);

  sem_wait(mutex1);
  *shared_msg = new_msg;
  sem_post(mutex1);

  sem_post(sem_server2);
  sem_post(sem_server);

  while (1) {

    struct message local_msg = {0};

    wmove(input_win, 1, 2);
    wclrtoeol(input_win);
    mvwprintw(input_win, 1, 2, "> ");
    wrefresh(input_win);

    echo();
    mvwgetnstr(input_win, 1, 4, input, 255);
    noecho();

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0) {
      strncpy(local_msg.mtext, input, 5);
      local_msg.categoriy_message = CLIENT_LEAVE;
      local_msg.from_message = pid;
      sem_wait(mutex1);
      *shared_msg = local_msg;
      sem_post(mutex1);
      sem_post(sem_server);
      break;
    }

    if (strlen(input) > 0) {
      strncpy(local_msg.mtext, input, 255);
      local_msg.mtext[255] = '\0';
      local_msg.categoriy_message = MESSAGE_FROM_CLIENT;
      local_msg.from_message = pid;
      sem_wait(mutex1);
      *shared_msg = local_msg;
      sem_post(mutex1);
      sem_post(sem_server);
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <username>\n", argv[0]);
    exit(1);
  }

  fp = open("./text.txt", O_CREAT | O_RDWR, 0666);
  pid = getpid();

  mutex1 = sem_open("/mutex1", 0, 0666, 1);
  mutex2 = sem_open("/mutex2", 0, 0666, 1);
  sem_server = sem_open("/sem_server", 0, 0666, 0);
  sem_client = sem_open("/sem_client", 0, 0666, 0);
  sem_server2 = sem_open("/sem_server2", 0, 0666, 0);

  char *name = "/MyShared";
  shm_fd = shm_open(name, O_RDWR, 0666);

  initscr();
  cbreak();
  noecho();
  curs_set(0);

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  int chat_width = max_x * 2 / 3;
  int users_width = max_x - chat_width - 1;

  chat_win = newwin(max_y - 3, chat_width, 0, 0);

  int height, width;
  getmaxyx(chat_win, height, width);
  child_chat_win = subwin(chat_win, height - 2, width - 3, 1, 2);
  scrollok(child_chat_win, TRUE);

  box(chat_win, 0, 0);
  mvwprintw(chat_win, 0, 2, " Chat Messages ");
  wrefresh(chat_win);

  users_win = newwin(max_y - 3, users_width, 0, chat_width + 1);
  box(users_win, 0, 0);
  mvwprintw(users_win, 0, 2, " Users Online ");
  wrefresh(users_win);

  input_win = newwin(3, max_x, max_y - 3, 0);
  box(input_win, 0, 0);
  mvwprintw(input_win, 1, 2, "> ");
  wrefresh(input_win);

  pthread_t thread_read, thread_write;

  pthread_create(&thread_read, NULL, read_queue, (void *)&pid);
  pthread_create(&thread_write, NULL, write_queue, (void *)argv[1]);

  pthread_join(thread_write, NULL);
  pthread_cancel(thread_read);
  pthread_join(thread_read, NULL);

  endwin();
  return 0;
}