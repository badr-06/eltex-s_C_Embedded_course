#ifndef LIB_H
#define LIB_H

#include <stddef.h>
#include <stdio.h>
#include <string.h>


#define MAX_LEN 100

struct abonent {
    char name[10];
    char second_name[10];
    char tel[10];
};

int insert_abonent(struct abonent* array);
void insert(struct abonent *array, const char *name, const char* second_name, const char* tel);
int print_all_abonents(struct abonent* array);
int search_abonent(struct abonent* array);
int search(struct abonent* array, const char* name);
int delete_abonent(struct abonent* array);
void delete_(struct abonent* array, const int del_i, const int index);
size_t find_free_index(struct abonent* array);
void clean_buffer();

#endif // LIB_H