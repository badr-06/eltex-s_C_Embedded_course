#include <errno.h>
#include "lib.h"



int insert_abonent(struct abonent* array){

    if(array == NULL){
        fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
        return -1;
    }

    size_t index = find_free_index(array);

    if(index > 100){
        printf("Ошибка: Справочник переполнен, (Макс 100 абонентов)\n");
        return 1;
    } 
    
    printf("Введите имя не больше 10 символов: ");
    char name[100];
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Введите фамилию не больше 10 символов: ");
    char second_name[100];
    fgets(second_name, 100, stdin);
    second_name[strcspn(second_name, "\n")] = '\0';

    printf("Введите номер телефона не больше 10 символов: ");
    char tel[100];
    fgets(tel, 100, stdin);
    tel[strcspn(tel, "\n")] = '\0';

    insert(&array[index], name, second_name, tel);

    return 0;

}

void insert(struct abonent *array, const char *name, const char* second_name, const char* tel)
{
    strncpy(array->name, name, 10);
    strncpy(array->second_name, second_name, 10);
    strncpy(array->tel, tel, 10);
}

void clean_buffer() {
    int c;
    while (((c = getchar()) != '\n') && c != EOF);
}


size_t find_free_index(struct abonent *array)
{
    size_t index = 0;
    while ((array[index].name[0] != 0 && array[index].second_name[0] != 0 && \
    array[index].tel[0] != 0) && index < 100) ++index;
    
    return index;
}

int print_all_abonents(struct abonent *array)
{
    if(array == NULL){
        fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
        return -1;
    } else if(find_free_index(array) == 0){
        fprintf(stderr, "Справочник пустой\n");
        return -2;
    }

    int index = find_free_index(array);
    for(int i = 0; i < index; ++i)
    {
        printf("Абонент %d:\n", i + 1);
        printf("Имя абонента: %s\n", array[i].name);
        printf("Фамилия абонента: %s\n", array[i].second_name);
        printf("Номер телефона абонента: %s\n", array[i].tel);
    }
    
    return 0;
}

int search_abonent(struct abonent *array)
{
    if(array == NULL){
        fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
        return -1;
    } else if(find_free_index(array) == 0){
        fprintf(stderr, "Справочник пустой\n");
        return -2;
    }

    printf("Введите имя абонента которого надо найти: ");

    char name[100] = {0};
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = '\0';

    int i = search(array, name);

    if(i == -1){
        printf("Абонента с именем %s нет в справочнике", name);
    } else {
            printf("Абонент %d:\n", i + 1);
            printf("Имя абонента: %s\n", array[i].name);
            printf("Фамилия абонента: %s\n", array[i].second_name);
            printf("Номер телефона абонента: %s\n", array[i].tel);
        
    }

    return 0;
}

int search(struct abonent *array, const char *name)
{
    int index = find_free_index(array);

    int idx = -1;
    for(int i = 0; i < index; ++i)
    {
        if(strcmp(array[i].name, name) == 0){
            idx = i;
            break;
        }
    }

    return idx;
}

int delete_abonent(struct abonent *array)
{
    if(array == NULL){
        fprintf(stderr, "Передача в Функцию \"%s\" нулевого указателя\n", __func__);
        return -1;
    } else if(find_free_index(array) == 0){
        fprintf(stderr, "Справочник пустой\n");
        return -2;
    }

    printf("Введите имя абонента которого надо удалить: ");

    char name[100] = {0};
    fgets(name, 100, stdin);
    name[strcspn(name, "\n")] = '\0';

    // индекс где заканичваются абоненты
    int index = find_free_index(array);

    int del_i = -1;
    int count_del = 0;
    for(int i = 0; i < index; ++i)
    {
        if(strcmp(array[i].name, name) == 0){
            strncpy(array[i].name, "", 10);
            strncpy(array[i].second_name, "", 10);
            strncpy(array[i].tel, "", 10);
            if(del_i == -1) del_i = i;
            ++count_del;
        }
    }

    // удаляем абонентов
    if(count_del > 0){
        delete_(array, del_i, index);
    }

    return 0;
}

void delete_(struct abonent *array, const int del_i, const int index)
{
       for (int l = del_i, r = l + 1; r < index;)
        {
            if(array[l].name[0] == '\0' && array[l].second_name[0] == '\0' && array[l].tel[0] == '\0'){

            if(array[r].name[0] != '\0' && array[r].second_name[0] != '\0' && array[r].tel[0] != '\0'){

                strncpy(array[l].name, array[r].name, 10);
                strncpy(array[l].second_name, array[r].second_name, 10);
                strncpy(array[l].tel, array[r].tel, 10);

                memset(array[r].name, '\0', 10);
                memset(array[r].second_name, '\0', 10);
                memset(array[r].tel, '\0', 10);
                ++l;
                ++r;
            } else {
                ++r;
            }
        } else {
            ++l;
        }
            

        }
}
