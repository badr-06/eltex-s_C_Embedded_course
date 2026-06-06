#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <sys/sem.h>

#define SIZE 4096

int main(){

    int shm_fd;
    key_t key = ftok(".", 1);
    int semf;

    semf = semget(key, 2, 0666 | IPC_CREAT);
    semctl(semf, 0, SETVAL, 0);
    semctl(semf, 1, SETVAL, 0);

    shm_fd = shmget(key, SIZE, 0666 | IPC_CREAT);

    char *ptr = (char*)shmat(shm_fd, NULL, O_RDWR);
    sprintf(ptr, "%s", "Hi!");

    struct sembuf v_server = {0, 1, 0};
    struct sembuf p_client = {1, -1, 0};

    semop(semf, &v_server, 1);
    semop(semf, &p_client, 1);

    printf("%s\n", ptr);

    shmctl(shm_fd, IPC_RMID, NULL);
    semctl(semf, 0, IPC_RMID);

    return 0;
}