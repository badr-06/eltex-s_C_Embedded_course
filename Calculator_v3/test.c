#include <stdio.h>
// #include <errno.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <dirent.h>
#include <string.h>

typedef struct 
{
    char name[255];
    int size;
    int (*func)(int, int);
}lib_func;


int library_processing(const char *lib_name){
    if(lib_name == NULL) fprintf(stderr, "Ошибка: указатель на NULL в функции \"%s\"", __func__);

    

}

int main(void) {
    char *name = {"subs.so"};
    char *str = strstr(name, ".so");
    int len = str - name;
    char res[255]; 
    strncpy(res, name, len);
    printf("%s\n", res);

    return 0;
}
