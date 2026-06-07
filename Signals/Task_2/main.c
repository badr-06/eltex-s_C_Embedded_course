#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    sigset_t sigset;
    
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);

    if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1){
        perror("sigprocmask()");
        exit(1);
    }

    printf("Команда SIGINT заблокирована\n");
    printf("Для проверки отправьте сигнал с помощью команды kill -SIGINT %d - PID процесса\n", getpid());

    while(1){}
}