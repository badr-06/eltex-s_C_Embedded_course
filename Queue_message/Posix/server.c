#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(){

    mqd_t mq;
    struct mq_attr attr;

    attr.mq_flags = 0;
    attr.mq_maxmsg = 5;
    attr.mq_msgsize = 1024;
    attr.mq_curmsgs = 0;

    mq = mq_open("/queue_messge", O_CREAT | O_RDWR, 0666, &attr);

    if(mq == -1){
        perror("Error mq_open()");
        exit(1);
    }

    char buffer[1024];
    printf("Введите сообщение которое хотите отправить: ");
    fgets(buffer, attr.mq_msgsize, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if(mq_send(mq, buffer, strlen(buffer) + 1, 1) == -1){
        perror("Error mq_send(), Сообщение не отправлено");
        exit(1);
    }

    if(mq_receive(mq, buffer, attr.mq_msgsize, NULL) == -1){
        perror("Error mq_receive(), Сообщение не прочитано");
        exit(1);
    }
    printf("%ld\n", strlen(buffer));
    printf("Сообщение от Client %s\n", buffer);
    
    mq_close(mq);

    return 0;
}