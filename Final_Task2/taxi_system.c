#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/msg.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_DRIVERS 100
#define BUFFER_SIZE 256

typedef struct {
  int pid;
  int is_busy;
  int task_timer;
} Driver;

struct message {
  long mtype;
  int time;
  pid_t pid;
};

Driver drivers[MAX_DRIVERS];
int driver_count = 0;
key_t key;
int msgid;

void cleanup() {
  for (int i = 0; i < driver_count; i++) {
    kill(drivers[i].pid, SIGTERM);
  }

  if (msgctl(msgid, IPC_RMID, NULL) == -1) printf("Error msgctl()");
}

void driver_process(pid_t main_pid) {
  int is_busy = 0;
  int task_timer = 0;
  pid_t my_pid = getpid();

  printf("Driver %d started\n", my_pid);

  while (1) {
    struct message msg;

    if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), my_pid, 0) == -1)
      continue;

    if (msg.time > 0) {
      task_timer = msg.time;
      is_busy = 1;
    }

    while (is_busy) {
      sleep(1);
      task_timer--;
      if (task_timer <= 0) {
        is_busy = 0;
        task_timer = 0;
        msg.mtype = main_pid;
        msg.pid = my_pid;
        msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0);
        printf("> Driver %d: task completed\n", getpid());
      }
    }
  }
}

int create_driver() {
  pid_t main_pid = getpid();
  pid_t pid = fork();

  if (pid < 0) {
    printf("Error: fork failed\n");
    return -1;
  }

  if (pid == 0) {
    driver_process(main_pid);
    exit(0);
  }

  drivers[driver_count].pid = pid;
  drivers[driver_count].is_busy = 0;
  drivers[driver_count].task_timer = 0;
  driver_count++;

  printf("Driver created with pid: %d\n", pid);
  return pid;
}

void send_task(int pid, int task_timer) {
  for (int i = 0; i < driver_count; i++) {
    if (drivers[i].pid == pid) {
      if (drivers[i].is_busy) {
        printf("Error: Busy %d\n", drivers[i].task_timer);
        return;
      }

      drivers[i].is_busy = 1;
      drivers[i].task_timer = task_timer;

      struct message msg;
      msg.mtype = pid;
      msg.time = task_timer;

      printf("Task sent to driver %d for %d seconds\n", pid, task_timer);
      if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), 0) == -1) {
        perror("msgsnd()");
      }
      return;
    }
  }
  printf("Error: Driver not found\n");
}

void get_status(int pid) {
  for (int i = 0; i < driver_count; i++) {
    if (drivers[i].pid == pid) {
      if (drivers[i].is_busy)
        printf("Busy %d\n", drivers[i].task_timer);
      else
        printf("Available\n");
      return;
    }
  }
  printf("Error: Driver not found\n");
}

void update_status_driver(int pid) {
  for (int i = 0; i < driver_count; i++) {
    if (drivers[i].pid == pid) {
      drivers[i].is_busy = 0;
      drivers[i].task_timer = 0;
      break;
    }
  }
}

void get_drivers() {
  if (driver_count == 0) {
    printf("No drivers found\n");
    return;
  }

  printf("Drivers:\n");
  for (int i = 0; i < driver_count; i++) {
    if (drivers[i].is_busy)
      printf("  PID: %d, Status: Busy %d\n", drivers[i].pid,
             drivers[i].task_timer);
    else
      printf("  PID: %d, Status: Available\n", drivers[i].pid);
  }
}

int main() {
  printf("Taxi Management CLI\n");
  printf(
      "Commands: create_driver, send_task <pid> <timer>, get_status <pid>, "
      "get_drivers, exit\n-----------------------------------------------\n\n");

  char line[BUFFER_SIZE];
  key = ftok(".", 1);
  if (key == -1) printf("Error ftok()");

  msgid = msgget(key, 0666 | IPC_CREAT);
  if (msgid == -1) printf("Error msgget()");

  int my_pid = getpid();
  while (1) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    struct timeval timeout = {1, 0};

    int max_fd = STDIN_FILENO > msgid ? STDIN_FILENO : msgid;

    int ret = select(max_fd + 1, &fds, NULL, NULL, &timeout);

    if (ret > 0 && FD_ISSET(STDIN_FILENO, &fds)) {
      if (read(STDIN_FILENO, line, BUFFER_SIZE) == -1) break;
      line[strcspn(line, "\n")] = 0;

      printf("> ");
      int pid, timer;

      if (strcmp(line, "create_driver") == 0) {
        create_driver();
      } else if (strncmp(line, "send_task ", 10) == 0) {
        if (sscanf(line + 10, "%d %d", &pid, &timer) == 2)
          send_task(pid, timer);
        else
          printf("Usage: send_task <pid> <task_timer>\n");
      } else if (strncmp(line, "get_status ", 11) == 0) {
        if (sscanf(line + 11, "%d", &pid) == 1)
          get_status(pid);
        else
          printf("Usage: get_status <pid>\n");
      } else if (strcmp(line, "get_drivers") == 0) {
        get_drivers();
      } else if (strcmp(line, "exit") == 0) {
        printf("GOODBYE\n");
        cleanup();
        break;
      } else {
        printf("Error: Incorrect input data\n");
      }
    }

    struct message msg;

    if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(msg.mtype), my_pid,
               IPC_NOWAIT) == -1) {
      continue;
    }
    int pid = msg.pid;
    update_status_driver(pid);
  }

  return 0;
}