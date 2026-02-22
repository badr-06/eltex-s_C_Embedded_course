#include "header.h"

void clean_buffer() {
  int c;
  while (((c = getchar()) != '\n') && c != EOF);
}

int library_processing(const char* lib_name, lib_func* list_func, int i) {
  if (lib_name == NULL) {
    fprintf(stderr, "Ошибка: указатель на NULL в функции \"%s\"", __func__);
    return 1;
  }
  list_func[i].func = NULL;
  list_func[i].lib_so = NULL;
  list_func[i].name[0] = '\0';

  list_func[i].lib_so = dlopen(lib_name, RTLD_LAZY);
  if (!list_func[i].lib_so) {
    fputs(dlerror(), stderr);
    return 1;
  }

  char name_func[255] = {0};
  char* str = strstr(lib_name, ".so");
  int idx = str - lib_name;
  strncpy(name_func, lib_name, idx);
  name_func[idx] = '\0';

  list_func[i].func = dlsym(list_func[i].lib_so, name_func);

  char* error = NULL;
  if ((error = dlerror()) != NULL) {
    fputs(error, stderr);
    return 1;
  }
  strncpy(list_func[i].name, name_func, strlen(name_func));
  list_func[i].name[strlen(name_func)] = '\0';

  return 0;
}

void menu(lib_func* list_func, const info_lib_func* info_list_func) {
  if (list_func == NULL || info_list_func == NULL) {
    fprintf(stderr, "указатель на NULL в функции \"%s\"\n", __func__);
    return;
  }

  for (unsigned i = 0; i < info_list_func->size; ++i) {
    if (strcmp(list_func[i].name, "add") == 0) {
      printf("%d) Сложение\n", i + 1);
    } else if (strcmp(list_func[i].name, "sub") == 0) {
      printf("%d) Вычитание\n", i + 1);
    } else if (strcmp(list_func[i].name, "mul") == 0) {
      printf("%d) Умножение\n", i + 1);
    } else if (strcmp(list_func[i].name, "div") == 0) {
      printf("%d) Деление\n", i + 1);
    }
  }
  printf("%d) Выход\n", info_list_func->size + 1);

  while (1) {
    unsigned num = 0;
    if (scanf("%d", &num) != 1 || num < 1 || num > info_list_func->size + 1) {
      fprintf(stderr, "проверьте 'num' на валидность\n");
      clean_buffer();
      continue;
    }

    if (num == info_list_func->size + 1) break;

    int x, y;
    printf("Введите два параметра: ");
    if (scanf("%d %d", &x, &y) != 2) {
      fprintf(stderr, "проверьте параметры 'x' и 'y' на валидность\n");
      clean_buffer();
      continue;
    }
    list_func[num - 1].func(x, y);
  }
}