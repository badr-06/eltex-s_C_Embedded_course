#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LENGTH_INPUT_STRING 1024
#define MAX_NUMBER_ARGUMENTS 64

int main() {
  char line[MAX_LENGTH_INPUT_STRING];
  char *args[MAX_NUMBER_ARGUMENTS];
  pid_t child_pid;
  // int status;

  while (1) {
    // char current_cwd[MAX_LENGTH_INPUT_STRING];
    // if(getcwd(current_cwd, MAX_LENGTH_INPUT_STRING) == NULL){
    //     perror("getcwd() error");
    //     break;
    // }

    printf("$ ");
    fflush(stdout);

    fgets(line, MAX_LENGTH_INPUT_STRING, stdin);
    line[strcspn(line, "\n")] = '\0';

    char *token;
    char *tmp = line;
    int i = 0;

    while ((token = __strtok_r(tmp, " ", &tmp))) {
      args[i] = token;
      ++i;
    }
    args[i] = NULL;

    if (!strcmp(args[0], "exit")) {
      break;
    }

    child_pid = fork();

    if (child_pid == -1) {
      perror("fork() error");
      continue;
    } else if (child_pid == 0) {
      execvp(args[0], args);

      fprintf(stderr, "%s: нет такой команды\n", args[0]);
      exit(1);
    } else {
      if (wait(NULL) == -1) {
        perror("wait() error");
        exit(1);
      }
    }
  }

  return 0;
}