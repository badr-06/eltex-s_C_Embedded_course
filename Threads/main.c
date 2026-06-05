#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUMBER_STORES 5
#define NUMBER_THREADS 3

pthread_mutex_t mutex[NUMBER_STORES] = {PTHREAD_MUTEX_INITIALIZER};
int Store[NUMBER_STORES] = {0};


void init_store(int *Store){

    srand(time(NULL));

    int n = NUMBER_STORES;
    while(n --> 0){
        Store[n] = ((rand() % 200) + 900);
    }
}

void* customer(void* number_pthread){

    int desire_spend = (rand() % 200) + 9900;
    int np = *(int *) number_pthread;

    while (1) {
        
        int numberStore = (rand() % 5);
        
        pthread_mutex_lock(&mutex[numberStore]);
        printf("Покупатель номер %d зашел в магазин номер %d\n", np, numberStore + 1);
        printf("Потребность покупателя %d: %d\n", np, desire_spend);

        if(!Store[numberStore]){
            printf("В магазие номер %d нет товара\n", numberStore + 1);
        }
        else {
            desire_spend -= Store[numberStore];
            printf("Покупатель номер %d скупил товара на сумму %d с магазина %d\n", np, Store[numberStore], numberStore + 1);
            printf("Потребность покупателя %d: %d\n", np, desire_spend);
            Store[numberStore] = 0;
        }

        pthread_mutex_unlock(&mutex[numberStore]);
        printf("Покупатель номер %d вышел с магазина номер %d\n", np, numberStore + 1);

        if(desire_spend <= 0){
            printf("Потребность покупателя %d Закночилась, покупатель %d Ушел\n", np, np);
            break;
        }
        else {
            printf("Покупатель номер %d заснул на 2 секунды\n", np);
            sleep(2);
            printf("Покупатель номер %d проснулся\n", np);
       }
    }

}

void* Supplier(void* arg){

    sleep(1);
    printf("Погручик проснулся\n");

    while (1)
    {
        int numberStore = (rand() % 5);

        pthread_mutex_lock(&mutex[numberStore]);
        printf("Погрузчик зашел в магазин номер %d, в магазине товара на сумму %d\n", numberStore + 1, Store[numberStore]);

        Store[numberStore] += 200;
        printf("Погрузчик пополнил магазин номер %d товаром на сумму %d, в магазине теперь товара на сумму %d\n", numberStore + 1, 200, Store[numberStore]);
        
        pthread_mutex_unlock(&mutex[numberStore]);
        printf("Погрузчик вышел из магазина номер %d\n", numberStore + 1);

        printf("Погручзик ушел спать на 1 секунду\n");
        sleep(1);
        printf("Погручик проснулся\n");
    }
    

}

int main(){

    init_store(Store);
    int indicates[NUMBER_THREADS] = {0};

    pthread_t threads_stores[NUMBER_THREADS];
    for(int i = 0; i < NUMBER_THREADS; ++i){
        indicates[i] = i + 1;
        pthread_create(&threads_stores[i], NULL, customer, (void*) &indicates[i]);
    }
    pthread_t threads_supplier;
    pthread_create(&threads_supplier, NULL, Supplier, NULL);

    for(int i = 0; i < NUMBER_THREADS; ++i){
        pthread_join(threads_stores[i], NULL);
    }
    pthread_cancel(threads_supplier);


    return 0;
}