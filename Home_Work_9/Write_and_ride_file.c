#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int write_file(const char *name_file){

    if(name_file == NULL){
        fprintf(stderr, "Передан указатель на NULL в функции \"%s\"", __func__);
        return 1;
    }

    int f = open(name_file, O_WRONLY | O_CREAT, 0644);

    if(f == -1){
        fprintf(stderr, "Ошибка при открытии файла \"%s\"", name_file);
        return 1;
    }

    const char *str = "String from file";
    write(f, str, 16);

    close(f);
}

int ride_file(const char *name_file){

    if(name_file == NULL){
        fprintf(stderr, "Передан указатель на NULL в функции \"%s\"", __func__);
        return 1;
    }

    int f = open(name_file, O_RDONLY);

    if(f == -1){
        fprintf(stderr, "Ошибка при открытии файла \"%s\"", name_file);
        return 1;
    }

    lseek(f, -1, SEEK_END);
    for(int i = 0; i < 16; ++i)
    {
        char ch;
        read(f, &ch, 1);
        printf("%c", ch);
        lseek(f, -2, SEEK_CUR);
    }
    

    close(f);
}


int main(){

    const char *name_file = "output.txt";
    
    if(write_file(name_file)) exit(1);
    if(ride_file(name_file)) exit(1);

    return 0;
}