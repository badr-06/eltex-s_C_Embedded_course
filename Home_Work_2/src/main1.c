#include <stdio.h>

#define N 3
#define M 3

#include "../lib/lib.h"

int main() {
  int array[N][M] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

  print_array(N, M, array);

  return 0;
}
