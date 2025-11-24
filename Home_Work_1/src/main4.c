#include <stdio.h>

int main(){
    int x;
    printf("Введите число: ");
    scanf("%d", &x);

    printf("Введенное число в битовом представлении: ");
    for(int i = 0, j = 31; i < 32; ++i, --j){
        printf("%d", (x >> j) & 1);
    }
    putchar('\n');

    int bit = (x >> 2) & 1;

    printf("третий бит введенного числа в двоичном представлении равен %d: \n", bit);

    int replace;
    printf("Введите значение на которе хотите поменять этот бит(1 либо 0): ");
    scanf("%d", &replace);

    if(bit && !replace){
        x ^= (1 << 2);    
    }
    else if(!bit && replace){
        x ^= (replace << 2);
    }

    printf("Result:\nВ десятичном: %d\nВ двоичном: ", x);
    for(int i = 0, j = 31; i < 32; ++i, --j){
        printf("%d", (x >> j) & 1);
    }
    putchar('\n');
}
