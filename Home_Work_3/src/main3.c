#include <stdio.h>

#define N 10

void print_array(int *array, int size){

  for (int i = 0; i < size; ++i){
    printf("%d ", *array);
    ++array;
  }
    
}

int main() {
  int array[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  print_array(array, N);

  return 0;
}
