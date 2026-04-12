#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  pid_t childPid1 = fork();

  if (childPid1 == -1) {
    perror("childPid1 = fork()");
    exit(1);
  } else if (childPid1 == 0) {
    printf("Process1\n");
    printf("pid = %d\n", getpid());
    printf("ppid = %d\n", getppid());

    pid_t childPid3 = fork();

    if (childPid3 == -1) {
      perror("childPid3 = fork()");
      exit(1);
    } else if (childPid3 == 0) {
      printf("Process3\n");
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());

      exit(0);
    } else {
      waitpid(childPid3, NULL, 0);
    }

    pid_t childPid4 = fork();

    if (childPid4 == -1) {
      perror("childPid4 = fork()");
      exit(1);
    } else if (childPid4 == 0) {
      printf("Process4\n");
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());

      exit(0);
    } else {
      waitpid(childPid4, NULL, 0);
    }

    exit(0);
  } else {
    pid_t childPid2 = fork();

    if (childPid2 == -1) {
      perror("childPid2 = fork()");
      exit(1);
    } else if (childPid2 == 0) {
      printf("Process2\n");
      printf("pid = %d\n", getpid());
      printf("ppid = %d\n", getppid());

      pid_t childPid5 = fork();

      if (childPid5 == -1) {
        perror("childPid5 = fork()");
        exit(1);
      } else if (childPid5 == 0) {
        printf("Process5\n");
        printf("pid = %d\n", getpid());
        printf("ppid = %d\n", getppid());

        exit(0);
      } else {
        waitpid(childPid5, NULL, 0);
      }
    }

    wait(NULL);
    wait(NULL);
  }

  return 0;
}