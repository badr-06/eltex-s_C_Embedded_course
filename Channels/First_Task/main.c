#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(){

    int fd[2];
    pid_t childPid;


    if(pipe(fd) == -1){
        perror("pipe()");
        exit(1);
    }

    childPid = fork();

    switch (childPid)
    {
    case -1:
        perror("fork()");
        exit(1);
        break;
    case 0:
        printf("Дочерний процесс запустился\n");
        close(fd[1]);
        char str[4];
        read(fd[0], str, 4);
        printf("%s\n", str);
        exit(1);
    default:
        printf("Родительский процесс запустился\n");
        close(fd[0]);
        write(fd[1], "Hi!", 4);
        wait(NULL);
        break;
    }

    return 0;
}