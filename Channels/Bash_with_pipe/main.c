#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_LENGTH_INPUT_STRING 1024
#define MAX_NUMBER_ARGUMENTS 64
#define MAX_NUMBER_COMMAND = 5;

void close_Chanalles(int number, int pipes[number][2])
{
  for(int i = 0; i < number; ++i)
  {
    close(pipes[i][0]);
    close(pipes[i][1]);
  }

}

int find_number_command(char *str)
{

  int size = 0;
  int number_command = 1;
  size = strcspn(str, "|");
  while (size != strlen(str))
    {
      number_command++;
      memmove(str, str + size + 2, strlen(str + size + 2) + 1);
      size = strcspn(str, "|"); 
    }
  return number_command;
}

int run_process(int number_commands, char *args[number_commands][MAX_NUMBER_ARGUMENTS])
{
  pid_t child_pid[number_commands];
  int pipes[number_commands - 1][2];
  for(int i = 0; i < number_commands - 1; ++i)
  {
    pipe(pipes[i]);
  }

  for(int i = 0; i < number_commands; ++i){

    child_pid[i] = fork();

    if (child_pid[i] == -1) {
      perror("fork() error");
      exit(1);
    } else if (child_pid[i] == 0) {
      if(i > 0) dup2(pipes[i-1][0], STDIN_FILENO);
      if(i < number_commands - 1) dup2(pipes[i][1], STDOUT_FILENO);

      close_Chanalles(number_commands-1, pipes);

      execvp(args[i][0], args[i]);

      fprintf(stderr, "%s: нет такой команды\n", args[i][0]);
      exit(1);
    } 

  }

  close_Chanalles(number_commands-1, pipes);

  for(int i = 0; i < number_commands; ++i){
    wait(NULL);
  }

}

int main() {
  char line[MAX_LENGTH_INPUT_STRING];
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
    
    char str[strlen(line) + 1];
    strncpy(str, line, strlen(line) + 1); 
  

    int number_commands = find_number_command(str);


    char *token;
    char *tmp = line;
    int i = 0;
    char *args[number_commands][MAX_NUMBER_ARGUMENTS];
    int index_comand = 0;

    while ((token = __strtok_r(tmp, " ", &tmp))) {
      if (!strcmp(token, "exit")) {
        exit(0);
      }

      if(!strcmp(token, "|")){
        args[index_comand][i] = NULL;
        ++index_comand;
        i = 0;
        continue;
      }

      args[index_comand][i] = token;
      ++i;
    }
    args[index_comand][i] = NULL;


    run_process(number_commands, args);
    
  }

  return 0;
}