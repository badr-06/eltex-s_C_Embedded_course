#include <stdbool.h>
#include <stdio.h>

#define N 5

#include "../lib/lib.h"

int main() {
  int array[N][N];
  int array_res[N][N];

  snail_array(N, array, array_res);

  print_array(N, N, array_res);

  return 0;
}
