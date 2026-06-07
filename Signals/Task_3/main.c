#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    sigset_t sigset;
    int signo = 0;

    sigemptyset(&sigset);
    sigaddset(&sigset, SIGUSR1);

    if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1){
        perror("sigprocmask()");
        exit(1);
    }

    printf("ожидает сигнал SIGUSR1\n");
    printf("Для отправки сигнала kill -SIGUSR1 %d - PID процесса\n", getpid());

    while (1)
    {
        if(sigwait(&sigset, &signo) != 0){
            perror("sigwait()");
            continue;
        }

        printf("получиил сигнал %d\n", signo);
    }
    
}