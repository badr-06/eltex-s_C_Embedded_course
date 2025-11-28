#include <stdio.h>

#define N 3
#define M 2

#include "../lib/lib.h"

int main() {
  int array[N][M];

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      array[i][j] = 2;
    }
  }

  change_array(N, M, array);

  print_array(N, M, array);

  return 0;
}
