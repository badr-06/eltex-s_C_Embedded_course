#include <stdbool.h>
#include <stdio.h>

#define N 5

#include "../lib/lib.h"

// int main() {
//   int array[N][N];
//   int array_res[N][N];

//   snail_array(N, array, array_res);

//   print_array(N, N, array_res);

//   return 0;
// }

// Функция для инициализации матрицы
void initial_array(int n, int array[n][n], int array_res[n][n], int num) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      array_res[i][j] = -1;
      array[i][j] = num;
      ++num;
    }
  }
}

// Функция для проверки в какуб сторону идем дальше
int check(int direction) { return (direction % 4) + 1; }

// Функция для заполнения массива улиткой
void snail_array(int n, int array[n][n], int array_res[n][n]) {
  initial_array(n, array, array_res, 1);

  bool flag_i = false;
  int direction = 1;  // пременная для направления
  for (int i = 0, j = 0, k = 0, l = 0, s = 0; s < n * n; ++s) {
    if (((i + 1 == n || array_res[i + 1][j] != -1) && direction == 2) ||
        ((j + 1 == n || array_res[i][j + 1] != -1) && direction == 1) ||
        ((i - 1 == -1 || array_res[i - 1][j] != -1) && direction == 4) ||
        ((j - 1 == -1 || array_res[i][j - 1] != -1) && direction == 3)) {
      direction = check(direction);

      if (!flag_i) {
        flag_i = true;
      } else {
        flag_i = false;
      }
    }

    array_res[i][j] = array[l][k];

    if (k == n - 1) {
      k = 0;
      ++l;
    } else
      ++k;

    if (flag_i) {
      if (direction == 2)
        ++i;
      else
        --i;
    } else {
      if (direction == 1)
        ++j;
      else
        --j;
    }
  }
}