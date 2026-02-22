#include "header.h"


int main(void) {
  struct dirent** namelist;
  int n;

  info_lib_func info_list_func = {0, 4};
  lib_func* list_func = malloc(sizeof(lib_func) * info_list_func.capacity);

  n = scandir("plagins", &namelist, NULL, NULL);
  if (n == -1) {
    perror("scandir");
    exit(1);
  }

  for (int i = 0; i < n; ++i) {
    if ((strstr(namelist[i]->d_name, ".so") != NULL)) {
      int error = library_processing(namelist[i]->d_name, list_func,
                                     info_list_func.size);
      if (!error) info_list_func.size++;

      if (info_list_func.size == info_list_func.capacity - 1) {
        info_list_func.capacity *= 2;
        list_func =
            realloc(list_func, sizeof(lib_func) * info_list_func.capacity);
      }
    }
  }

  menu(list_func, &info_list_func);

  for (size_t i = 0; i < info_list_func.size; ++i) {
    dlclose(list_func[i].lib_so);
  }
  free(list_func);
  for (int i = 0; i < n; ++i) {
    free(namelist[i]);
  }
  free(namelist);
}
