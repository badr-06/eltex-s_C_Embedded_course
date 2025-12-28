#include <check.h>
#include <stdlib.h>

#include "../lib/lib.h"

// Глобальная переменная для тестирования
struct abonent test_array[MAX_LEN] = {0};

// Вспомогательная функция для очистки массива
void init_test_array(struct abonent* array) {
    memset(array, 0, sizeof(struct abonent) * MAX_LEN);
}

// Тест 1: Базовая вставка и вывод
START_TEST(test_basic_insert_print) {
    
    init_test_array(test_array);
    
    // Вставляем 3 абонентов
    insert(&test_array[0], "Ivan", "Ivanov", "111111");
    insert(&test_array[1], "Petr", "Petrov", "222222");
    insert(&test_array[2], "Sergey", "Sidorov", "333333");
    
    // возвращает индекс первоой структуры без данных
    int count = find_free_index(test_array);

    ck_assert_int_eq(count, 3);
}
END_TEST

// Тест 2: Поиск абонента
START_TEST(test_search) {
    
    init_test_array(test_array);
    
    // Добавляем тестовые данные
    insert(&test_array[0], "Anton", "Ivanov", "123456");
    insert(&test_array[1], "Anton", "Petrov", "654321"); // Дубликат имени
    insert(&test_array[2], "Petr", "Sidorov", "987654");
    
    // возвращает индекс первого абонента с таким именем
    int result = search(test_array, "Anton");
    
    ck_assert_int_eq(result, 0);

    // возвращает -1 если нет абонента с таким именем
    result = search(test_array, "NonExistent");
    ck_assert_int_eq(result, -1);
}
END_TEST

// Тест 3: Удаление одного абонента
START_TEST(test_single_delete) {
    
    init_test_array(test_array);
    
    // Добавляем данные
    insert(&test_array[0], "User1", "Test1", "111");
    insert(&test_array[1], "User2", "Test2", "222");
    insert(&test_array[2], "User3", "Test3", "333");

    ck_assert_int_eq(3, find_free_index(test_array));
    
    // Помечаем второго абонента как удаленного
    memset(test_array[1].name, '\0', 10);
    memset(test_array[1].second_name, '\0', 10);
    memset(test_array[1].tel, '\0', 10);
    
    // Вызываем delete_ для сжатия
    delete_(test_array, 1, MAX_LEN);
    
    ck_assert_int_eq(2, find_free_index(test_array));
}
END_TEST

// Тест 4: Удаление всех с определенным именем
START_TEST(test_delete_all_by_name) {
    
    init_test_array(test_array);
    
    // Добавляем несколько абонентов с именем Anton
    insert(&test_array[0], "Anton", "Ivanov", "111");
    insert(&test_array[1], "Petr", "Petrov", "222");
    insert(&test_array[2], "Anton", "Sidorov", "333");
    insert(&test_array[3], "Sergey", "Serov", "444");
    insert(&test_array[4], "Anton", "Petrov", "555");
    
    ck_assert_int_eq(5, find_free_index(test_array));
    
    // Помечаем всех Anton как удаленных
    for (int i = 0; i < MAX_LEN; i++) {
        if (strcmp(test_array[i].name, "Anton") == 0) {
            memset(test_array[i].name, '\0', 10);
            memset(test_array[i].second_name, '\0', 10);
            memset(test_array[i].tel, '\0', 10);
        }
    }
    
    delete_(test_array, 0, MAX_LEN);

    ck_assert_int_eq(2, find_free_index(test_array));
}
END_TEST

// Тест 5: Граничные случаи
START_TEST(test_edge_cases_compact) {
    
    init_test_array(test_array);
    
    // Тест с пустым массивом
    int found = search_abonent(test_array);
    ck_assert_int_eq(-2, found);
    
    // Тест с максимальной длиной имени
    insert(&test_array[0], "ABCDEFGHIJ", "KLMNOPQRST", "1234567890");
    ck_assert_int_eq(1, find_free_index(test_array));
    
    // Тест delete_ с несуществующим индексом
    delete_(test_array, 999, MAX_LEN);

    ck_assert_int_eq(1, find_free_index(test_array));
    
}
END_TEST

// Тест 6: Проверка find_free_index
START_TEST(test_find_free_index) {
    
    init_test_array(test_array);
    
    ck_assert_int_eq(0, find_free_index(test_array));
    
    insert(&test_array[0], "Test", "User", "123");
    ck_assert_int_eq(1, find_free_index(test_array));
    
    insert(&test_array[1], "Test2", "User2", "456");
    ck_assert_int_eq(2, find_free_index(test_array));
}
END_TEST

Suite *example_suite_create(void) {
  Suite *suite = suite_create("Test_Home_Work_2");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, test_basic_insert_print);
  tcase_add_test(tcase_core, test_search);
  tcase_add_test(tcase_core, test_single_delete);
  tcase_add_test(tcase_core, test_delete_all_by_name);
  tcase_add_test(tcase_core, test_edge_cases_compact);
  tcase_add_test(tcase_core, test_find_free_index);

  suite_add_tcase(suite, tcase_core);

  return suite;
}

int main(void) {
  Suite *suite = example_suite_create();
  SRunner *suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  unsigned failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  if (failed_count != 0) {
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

