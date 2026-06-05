#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
// #include <sys/types.h>
#include <string.h>
#include <fcntl.h>

int main(){

    char *myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    char str[124];
    
    int fd = open(myfifo, O_RDONLY);

    read(fd, str, 4);

    printf("%s\n", str);

    return 0;
}