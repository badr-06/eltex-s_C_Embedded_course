#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
// #include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main(){

    char *myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);
    
    int fd = open(myfifo, O_WRONLY);

    write(fd, "Hi!", 4);

    close(fd);

    return 0;
}