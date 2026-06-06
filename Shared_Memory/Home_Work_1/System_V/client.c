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

    semf = semget(key, 2, 0666);

    shm_fd = shmget(key, SIZE, 0666);

    char *ptr = (char*)shmat(shm_fd, NULL, O_RDWR);

    struct sembuf v_client = {1, 1, 0};
    struct sembuf p_server = {0, -1, 0};

    semop(semf, &p_server, 1);
    printf("%s\n", ptr);

    sprintf(ptr, "%s", "Hello!");
    semop(semf, &v_client, 1);

    return 0;
}