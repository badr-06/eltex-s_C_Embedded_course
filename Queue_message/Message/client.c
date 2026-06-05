#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <unistd.h>

#include "library.h"

int msgid[2];
pid_t pid;

WINDOW *chat_win;
WINDOW *child_chat_win;
WINDOW *users_win;
WINDOW *input_win;

void view_infClients(struct infoClients *infCl, int sizeInfcl) {
  werase(users_win);
  box(users_win, 0, 0);
  mvwprintw(users_win, 0, 2, " Users ");
  for (int i = 0; i < sizeInfcl; i++) {
    mvwprintw(users_win, i + 2, 2, "%s", infCl[i].name);
  }
  wrefresh(users_win);
}

void *read_queue(void *arg) {
  struct message local_msg = {0};
  struct infoClients infCl[SIZE_CLIENTS] = {0};
  int sizeInfcl = 0;

  pid_t pid = *((pid_t *)arg);
  while (1) {
    msgrcv(msgid[1], &local_msg, sizeof(local_msg) - sizeof(local_msg.mtype),
           pid, 0);

    if (local_msg.categoriy_message == NEW_CLIENT) {
      strncpy(infCl[sizeInfcl].name, local_msg.mtext, 19);
      infCl[sizeInfcl].Pid = local_msg.from_message;
      sizeInfcl++;

      view_infClients(infCl, sizeInfcl);
    } else if (local_msg.categoriy_message == MESSAGE_FROM_CLIENT ||
               local_msg.categoriy_message == MESSAGE_FROM_SERVER) {
      wprintw(child_chat_win, "%s\n", local_msg.mtext);
      touchwin(child_chat_win);
      wrefresh(child_chat_win);
      wrefresh(chat_win);
    } else if (local_msg.categoriy_message == CLIENT_LEAVE) {
      int idx = find_name_client(infCl, local_msg.from_message, sizeInfcl);

      if (idx + 1 < sizeInfcl) {
        for (int i = idx; i < sizeInfcl; i++) {
          infCl[i] = infCl[i + 1];
        }
      }
      sizeInfcl--;
      view_infClients(infCl, sizeInfcl);
    }
  }

  return NULL;
}

void new_client(struct message *msg, int msgid, char *name) {
  msg->mtype = NEW_CLIENT;
  msg->categoriy_message = NEW_CLIENT;
  msg->from_message = pid;
  strncpy(msg->mtext, name, sizeof(msg->mtext) - 1);
  msg->mtext[sizeof(msg->mtext) - 1] = '\0';

  if (msgsnd(msgid, msg, sizeof(*msg) - sizeof(msg->mtype), 0) == -1) {
    perror("msgsnd");
  }
}

void *write_queue(void *arg) {
  struct message local_msg = {0};
  char input[256];
  char *name = (char *)arg;

  local_msg.from_message = pid;
  new_client(&local_msg, msgid[0], name);

  while (1) {
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
      local_msg.mtype = CLIENT_LEAVE;
      local_msg.from_message = pid;
      msgsnd(msgid[0], &local_msg, sizeof(local_msg) - sizeof(local_msg.mtype),
             0);
      break;
    }

    if (strlen(input) > 0) {
      strncpy(local_msg.mtext, input, 255);
      local_msg.mtext[255] = '\0';
      local_msg.mtype = MESSAGE_FROM_CLIENT;
      local_msg.categoriy_message = MESSAGE_FROM_CLIENT;
      local_msg.from_message = pid;
      msgsnd(msgid[0], &local_msg, sizeof(local_msg) - sizeof(local_msg.mtype),
             0);
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: %s <username>\n", argv[0]);
    exit(1);
  }

  pid = getpid();
  key_t key;
  key = ftok(".", 1);
  if ((msgid[0] = msgget(key, 0666)) == -1) {
    perror("msgid[0] msgget()");
  }

  key = ftok(".", 2);
  if ((msgid[1] = msgget(key, 0666)) == -1) {
    perror("msgid[1] msgget()");
  }

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