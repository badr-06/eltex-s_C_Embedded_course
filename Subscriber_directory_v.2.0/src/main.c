#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../lib/lib.h"

void clean_buffer() {
  int c;
  while (((c = getchar()) != '\n') && c != EOF)
    ;
}

int main() {
  printf(
      "1) Добавить абонента\n"
      "2) Удалить абонента\n"
      "3) Поиск абонентов по имени\n"
      "4) Вывод всех записей\n"
      "5) Выход\n");

  Abonent *array = malloc(sizeof(Abonent));
  array->size = 0;
  array->node = NULL;

  bool start = true;

  int ch = 0;
  while (start) {
    int prov = scanf("%d", &ch);

    clean_buffer();

    if (prov < 1) {
      printf("Проверь правильность введенной клавиши из меню\n");
      continue;
    }

    switch (ch) {
      case 1:
        insert_abonent(array);
        break;
      case 2:
        delete_abonent(array);
        break;
      case 3:
        search_abonent(array);
        break;
      case 4:
        print_all_abonents(array);
        break;
      case 5:
        start = false;
        break;
      default:
        printf("Проверь правильность введенной клавиши из меню\n");
        break;
    }
  }

  return 0;
}
