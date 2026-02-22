#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  void* lib_so;
  char name[255];
  int (*func)(int, int);
} lib_func;

typedef struct {
  unsigned size;
  unsigned capacity;
} info_lib_func;

int library_processing(const char* lib_name, lib_func* list_func, int i);
void menu(lib_func* list_func, const info_lib_func* info_list_func);

#endif