#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// int kill(pid_t pid, int sig);

int main(int argc, char *argv[]){
    
    if(argc != 2) {
        fprintf(stderr, "error: invalid argument <./a.out> <pid>\n");
        return 1;
    }

    
    int pid = atoi(argv[1]);
    if(kill(pid, SIGINT) == -1){
        perror("kill()");
        exit(1);
    }

    return 0;
}