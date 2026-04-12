#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t childPid;
  int status;

  switch (childPid = fork()) {
    case -1:
      perror("fork()");
      exit(1);

    case 0:
      // printf("Дочерний процесс\n");
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());
      exit(21);

    default:
      // printf("Родительский процесс\n");
      if (wait(&status) == -1) {
        perror("wait()");
        exit(1);
      }

      if (WIFEXITED(status)) {
        printf("status = %d\n", WEXITSTATUS(status));
      }
      break;
  }

  return 0;
}