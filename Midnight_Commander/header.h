#ifndef HED_H
#define HED_H

#include <dirent.h>
#include <malloc.h>

#include "lib.h"
#include "tui/tui.h"

typedef struct dirent dirent;

void run();
int scanningDirectory(dirent*** catalogList, const path_t* Path_catlog);
int sortList(dirent** catalogList, path_t* path_c, int size);
int update_infoCatalog(dirent** d_catalogList, infoCatalog** inf_catalogList,
                       path_t* path, int size);
int create_infoCatalog(infoCatalog*** inf_catalogList, path_t* Path_catalog,
                       auxiliaryStructure* auxStr, int idx);

#endif