#include <stdio.h>

#include "library/library.h"

int main() {
  printf(
      "1) Сложение\n"
      "2) Вычитание\n"
      "3) Умножение\n"
      "4) Деление\n"
      "5) Выход\n");

  while (1) {
    int num = 0;
    scanf("%d", &num);

    if (num == 5) break;

    int x, y;
    printf("Введите два параметра: ");
    scanf("%d %d", &x, &y);

    switch (num) {
      case 1:
        add(x, y);
        break;
      case 2:
        sub(x, y);
        break;
      case 3:
        mul(x, y);
        break;
      case 4:
        div(x, y);
        break;
      default:
        break;
    }
  }

  return 0;
}