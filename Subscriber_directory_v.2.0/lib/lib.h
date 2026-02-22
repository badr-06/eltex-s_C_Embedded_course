#ifndef LIB_H
#define LIB_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 100

typedef struct list_node {
  char* name;
  char* second_name;
  char* tel;

  struct list_node* next;
  struct list_node* pred;
} list_node;

typedef struct {
  list_node* node;
  int size;
} Abonent;

int insert_abonent(Abonent* array);
void insert(list_node** current_node, char* name, char* second_name, char* tel);
int print_all_abonents(Abonent* array);
int search_abonent(Abonent* array);
int search(list_node* current_node, const char* name);
int delete_abonent(Abonent* array);
list_node* delete_node(list_node* current_node, list_node** array_node);

void my_getline(char** str);

#endif  // LIB_H