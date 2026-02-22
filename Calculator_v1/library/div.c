#include "library.h"

int div(int x, int y) {
  if (y == 0) {
    fprintf(stderr, "На ноль делить нельзя\n");
    return 1;
  }

  printf("%d / %d = %d\n", x, y, x / y);

  return 0;
}