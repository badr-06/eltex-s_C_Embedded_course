#include <stdio.h>

#include "../lib/lib.h"

int main() {
  int x;
  scanf("%d", &x);

  unsigned char *ptr = ((unsigned char *)&x) + 2;
  printf("3 байт числа %d равен %d\n", x, *ptr);

  int substitute;
  scanf("%d", &substitute);

  *ptr = substitute;

  printf("3 байт числа %d равен %d\n", x, *ptr);

  return 0;
}
