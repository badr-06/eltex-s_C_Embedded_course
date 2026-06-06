#include <stdio.h>
#include <fcntl.h>
// #include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>

#define SIZE 4096

int main(){

    int shm_fd;
    void *ptr;
    char *name = "/MyShared";
    sem_t *sem_server, *sem_client;

    sem_server = sem_open("/sem_server", O_CREAT, 0666, 0);
    sem_client = sem_open("/sem_client", O_CREAT, 0666, 0);

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    ptr = mmap(0, 10, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    sprintf(ptr, "%s", "Hi");

    sem_post(sem_server);

    sem_wait(sem_client);

    printf("%s\n", (char*)ptr);

    munmap(ptr, 10);
    close(shm_fd);
    shm_unlink(name);

    sem_close(sem_server);
    sem_close(sem_client);
    sem_unlink("/sem_server");
    sem_unlink("/sem_client");

    return 0;
}