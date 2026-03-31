#ifndef LIB_H
#define LIB_H

#define NLINES 50
#define NCOLS 90

typedef struct {
  unsigned size;
  int status;
  char edit_time[13];
  char name[256];

} infoCatalog;
/*
    status:
    0 = директория;
    1 = бинарный файл;
    2 = остальные регулярные файлы;
*/

typedef struct {
  int size[2];
  int current_index[2];
  int current_catalog;
  int run;
} auxiliaryStructure;

typedef struct {
  int size;
  int capacity;
  char *path;
} path_t;

int changePathCatalog(char *name, path_t *Pc);

#endif