#include "header.h"

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

int scanningDirectory(dirent*** catalogList, const path_t* Path_catlog) {
  // printf("Path_catlog->path = %s\n", Path_catlog->path);
  int n = scandir(Path_catlog->path, catalogList, NULL, NULL);
  if (n == -1) {
    perror("scandir");
    exit(1);
  }
  // printf("n = %d\n", n);
  return n;
}

int sortList(struct dirent** catalogList, path_t* path_c, int size) {
  if (catalogList == NULL) return -1;

  struct stat stats_j_minus_1, stats_j;

  for (int i = 0; i < size; ++i) {
    for (int j = 1; j < size - i; ++j) {
      int size_d_name_j = strlen(catalogList[j]->d_name);
      int size_d_name_j_minus_1 = strlen(catalogList[j - 1]->d_name);

      char name_j_minus_1[size_d_name_j_minus_1 + path_c->size + 1];
      char name_j[size_d_name_j + path_c->size + 1];

      strncpy(name_j_minus_1, path_c->path, path_c->size);
      strncpy(name_j_minus_1 + path_c->size, catalogList[j - 1]->d_name,
              size_d_name_j_minus_1);
      name_j_minus_1[size_d_name_j_minus_1 + path_c->size] = '\0';

      strncpy(name_j, path_c->path, path_c->size);
      strncpy(name_j + path_c->size, catalogList[j]->d_name, size_d_name_j);
      name_j[size_d_name_j + path_c->size] = '\0';

      int result1 = stat(name_j_minus_1, &stats_j_minus_1);
      int result2 = stat(name_j, &stats_j);

      if (result1 != 0 || result2 != 0) continue;

      int s = strcmp(catalogList[j - 1]->d_name, catalogList[j]->d_name);

      if ((S_ISDIR(stats_j.st_mode) && !S_ISDIR(stats_j_minus_1.st_mode)) ||
          (s > 0 &&
           ((S_ISDIR(stats_j.st_mode) && S_ISDIR(stats_j_minus_1.st_mode)) ||
            (!S_ISDIR(stats_j.st_mode) &&
             !S_ISDIR(stats_j_minus_1.st_mode))))) {
        dirent* temp = catalogList[j];
        catalogList[j] = catalogList[j - 1];
        catalogList[j - 1] = temp;
      }
    }
  }

  return 0;
}

void format_mc_time(time_t t, char* buf, size_t bufsize) {
  struct tm tm = *localtime(&t);
  time_t now = time(NULL);
  struct tm* tm_now = localtime(&now);

  if (tm.tm_year == tm_now->tm_year) {
    // Текущий год: показываем месяц, день, час:минута
    strftime(buf, bufsize, "%b %d %H:%M", &tm);
  } else {
    // Другой год: показываем месяц, день, год
    strftime(buf, bufsize, "%b %d  %Y", &tm);
  }
}

int update_infoCatalog(dirent** d_catalogList, infoCatalog** inf_catalogList,
                       path_t* path_c, int size) {
  struct stat stats;
  for (int i = 1; i < size; ++i) {
    int size_d_name = strlen(d_catalogList[i]->d_name);
    char name[size_d_name + path_c->size + 1];
    strncpy(name, path_c->path, path_c->size);
    strncpy(name + path_c->size, d_catalogList[i]->d_name, size_d_name);
    name[size_d_name + path_c->size] = '\0';

    int result = stat(name, &stats);

    if (result == 0 && S_ISDIR(stats.st_mode)) {
      inf_catalogList[i - 1]->name[0] = '/';
      strncpy(inf_catalogList[i - 1]->name + 1, d_catalogList[i]->d_name,
              strlen(d_catalogList[i]->d_name));
      inf_catalogList[i - 1]->status = 0;
    } else if (result == 0 && S_ISREG(stats.st_mode)) {
      if (stats.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH)) {
        inf_catalogList[i - 1]->name[0] = '*';
        strncpy(inf_catalogList[i - 1]->name + 1, d_catalogList[i]->d_name,
                strlen(d_catalogList[i]->d_name));
        inf_catalogList[i - 1]->status = 1;
      } else {
        strncpy(inf_catalogList[i - 1]->name, d_catalogList[i]->d_name,
                strlen(d_catalogList[i]->d_name));
        inf_catalogList[i - 1]->status = 2;
      }
    }

    if (result == 0) {
      inf_catalogList[i - 1]->size = stats.st_size;
      format_mc_time(stats.st_mtime, inf_catalogList[i - 1]->edit_time, 20);
    }
  }
  return 0;
}

void free_dir(dirent** catalogList, int size) {
  for (int i = 0; i < size; ++i) {
    free(catalogList[i]);
  }
  free(catalogList);
}

void free_inf(infoCatalog** catalogList, int size) {
  for (int i = 0; i < size; ++i) {
    free(catalogList[i]);
  }
  free(catalogList);
}

int create_infoCatalog(infoCatalog*** inf_catalogList, path_t* Path_catalog,
                       auxiliaryStructure* auxStr, int idx) {
  dirent** d_catalogList;
  int size_d = 0;

  size_d = scanningDirectory(&d_catalogList, Path_catalog);
  sortList(d_catalogList, Path_catalog, size_d);

  auxStr->size[idx] = size_d - 1;

  inf_catalogList[idx] =
      (infoCatalog**)calloc(auxStr->size[idx], sizeof(infoCatalog*));
  for (int i = 0; i < auxStr->size[idx]; ++i) {
    inf_catalogList[idx][i] = (infoCatalog*)calloc(1, sizeof(infoCatalog));
  }

  update_infoCatalog(d_catalogList, inf_catalogList[idx], Path_catalog, size_d);

  free_dir(d_catalogList, size_d);

  return 0;
}

int changePathCatalog(char* name, path_t* Pc) {
  int size_name = strlen(name);

  if ((Pc->size + size_name) >= Pc->capacity) {
    char* temp = realloc(Pc->path, Pc->capacity * 2 * sizeof(char));
    printf("YES\n");
    Pc->path = temp;
    Pc->capacity *= 2;
  }

  strncpy(Pc->path + Pc->size, name, size_name);
  Pc->size += size_name;
  Pc->path[Pc->size] = '/';
  Pc->path[Pc->size + 1] = '\0';
  Pc->size++;

  return 0;
}

int create_path_t(path_t** path_catalog) {
  path_catalog[0] = (path_t*)malloc(sizeof(path_t));
  path_catalog[1] = (path_t*)malloc(sizeof(path_t));

  path_catalog[0]->path = calloc(256, sizeof(char));
  path_catalog[1]->path = calloc(256, sizeof(char));

  path_catalog[0]->path[0] = '.';
  path_catalog[0]->path[1] = '/';
  path_catalog[0]->size = 2;
  path_catalog[0]->capacity = 256;

  path_catalog[1]->path[0] = '.';
  path_catalog[1]->path[1] = '/';
  path_catalog[1]->size = 2;
  path_catalog[1]->capacity = 256;

  return 0;
}

void free_path_t(path_t** path) {
  free(path[0]->path);
  free(path[1]->path);
  free(path[0]);
  free(path[1]);
}

void run() {
  auxiliaryStructure auxStr = {0};
  auxStr.run = 1;
  path_t* Path_catalog[2];
  create_path_t(Path_catalog);
  auxStr.current_catalog = 0;

  while (auxStr.run) {
    infoCatalog** inf_catalogList[2];

    auxStr.current_index[0] = 0;
    auxStr.current_index[1] = 0;
    create_infoCatalog(inf_catalogList, Path_catalog[0], &auxStr, 0);
    create_infoCatalog(inf_catalogList, Path_catalog[1], &auxStr, 1);

    main_window(inf_catalogList, &auxStr, Path_catalog);

    free_inf(inf_catalogList[0], auxStr.size[0]);
    free_inf(inf_catalogList[1], auxStr.size[1]);
  }

  free_path_t(Path_catalog);
}
