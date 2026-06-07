#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

// struct sigaction {
//                void     (*sa_handler)(int);
//                void     (*sa_sigaction)(int, siginfo_t *, void *);
//                sigset_t   sa_mask;
//                int        sa_flags;
//                void     (*sa_restorer)(void);
//            };

void sig_handler(int sig)
{
    printf("Получил сигнал %d\n", sig);
}

int main(){
    
    struct sigaction act;
    act.sa_handler = sig_handler;
    
    if(sigaction(SIGUSR1, &act, NULL) == -1){
        perror("sigaction()");
        exit(1);
    }

    printf("Отправьте сигнал с помощью команды kill -SIGUSR1 %d - PID процесса\n", getpid());

    while (1){}
    
    return 0;
}