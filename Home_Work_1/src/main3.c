#include <stdio.h>

int main(){

    int x;
    printf("Ведите число: ");
    scanf("%d", &x);


    printf("Количество единиц в битовом представлении числа %d: ", x);

    unsigned count_one = 0;
    for(int i = 0; i < 32; ++i){
        if((x >> 31) & 1){
            ++count_one;
        }
        x <<= 1;
    }

    printf("%u\n", count_one);
    
   return 0;
}