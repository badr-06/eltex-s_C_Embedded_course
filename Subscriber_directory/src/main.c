#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../lib/lib.h"

int main() {
    
    printf(
        "1) Добавить абонента\n"
        "2) Удалить абонента\n"
        "3) Поиск абонентов по имени\n"
        "4) Вывод всех записей\n"
        "5) Выход\n"
    );

    struct abonent array[MAX_LEN] = {0};
    // strncpy(array[0].name, "Ivanoivan", 9);
    // strncpy(array[0].second_name, "Ivanov", 9);
    // strncpy(array[0].tel, "8978", 9);

    bool start = true;
    
    int ch = 0;
    while (start)
    {

        int prov = scanf("%d", &ch);
        clean_buffer();

        if(prov < 1){
            printf("Проверь правильность введенной клавиши из меню\n");
            continue;
        }
        
        switch (ch)
        {
        case 1:
            insert_abonent(array);
            break;
        case 2:
            delete_abonent(array);
            break;
        case 3:
            search_abonent(array);
            break;
        case 4:
            print_all_abonents(array);
            break;
        case 5:
            start = false;
            break;
        default:
            printf("Проверь правильность введенной клавиши из меню\n");
            break;
        }
    }
    

    return 0;
}
