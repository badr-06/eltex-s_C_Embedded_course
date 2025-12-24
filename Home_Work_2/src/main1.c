#include <stdio.h>

#define N 3
#define M 3

#include "../lib/lib.h"

// int main() {
//   int array[N][M] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

//   print_array(N, M, array);

//   return 0;
// }

void print_array(int n, int m, int array[n][m]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      printf("%d ", array[i][j]);
    }
    putchar('\n');
  }
}