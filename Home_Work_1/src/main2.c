#include <stdio.h>

int main(){

    int x;
    printf("Ведите число: ");
    scanf("%d", &x);

    for(int i = 0; i < 32; ++i){
        printf("%d", (x >> 31) & 1);
        x <<= 1;
    }

   return 0;
}