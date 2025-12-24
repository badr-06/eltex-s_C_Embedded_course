#include <stdio.h>

#define N 3
#define M 2

#include "../lib/lib.h"

// int main() {
//   int array[N][M];

//   for (int i = 0; i < N; ++i) {
//     for (int j = 0; j < M; ++j) {
//       array[i][j] = 2;
//     }
//   }

//   change_array(N, M, array);

//   print_array(N, M, array);

//   return 0;
// }

// Функция которая заполняет верхний треугольник матрицы 0, а нижний вместе с
// центральной диагональю 1
void change_array(int n, int m, int array[n][m]) {
  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      array[i - 1][j - 1] = (j > m - i) ? 1 : 0;
    }
  }
}