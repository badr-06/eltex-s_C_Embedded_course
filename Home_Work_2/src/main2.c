#include <stdio.h>

#define N 10

#include "../lib/lib.h"

// int main() {
//   int array[N];

//   for (int i = 0; i < N; ++i) {
//     array[i] = i + 1;
//   }

//   swap_array(N, array);

//   for (int i = 0; i < N; ++i) {
//     printf("%d ", array[i]);
//   }
//   putchar('\n');

//   return 0;
// }

void swap_array(int n, int array[n]) {
  for (int i = 0, j = n - 1; i < j; ++i, --j) {
    array[i] += array[j];
    array[j] = array[i] - array[j];
    array[i] -= array[j];
  }
}