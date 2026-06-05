#include <stdio.h>
#include <sys/msg.h>
#include <string.h>
#include <stdlib.h>

struct message {
    long mtype;
    char mtext[256];
};

void print_error(char* error){
    perror(error);
    exit(1);
}
int main(){


    key_t key;
    int msgid;
    struct message mesg;
    
    key = ftok("file.txt", 1);
    if(key == -1) print_error("Error ftok()");

    msgid = msgget(key, 0666);
    if(msgid == -1) print_error("Error msgget()");

    if(msgrcv(msgid, &mesg, sizeof(mesg.mtext), 1, 0) == -1){
        perror("Error msgrcv(): сообщение не получено");
    }

    printf("Сообщение от Server: %s\n", mesg.mtext);
    
    printf("Введите сообщение которое хотите отправить: ");
    fgets(mesg.mtext, 256, stdin);
    mesg.mtext[strcspn(mesg.mtext, "\n")] = '\0';

    if(msgsnd(msgid, &mesg, sizeof(mesg.mtext), 0) == -1){
        perror("Error msggnd(): сообщение не отправлено");
    }

    return 0;
}