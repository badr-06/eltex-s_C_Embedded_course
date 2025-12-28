#include <check.h>
#include <stdlib.h>
#include "../lib/lib.h"

// Ваши функции здесь...

START_TEST(test_mystrlen_basic) {
    char str1[] = "Hello";
    ck_assert_uint_eq(mystrlen(str1), strlen(str1));
    
    char str2[] = "";
    ck_assert_uint_eq(mystrlen(str2), strlen(str2));
    
    char str3[] = "Hello\nWorld";
    // mystrlen останавливается на \n, обычный strlen нет
    ck_assert_uint_eq(mystrlen(str3), 5);
}
END_TEST

START_TEST(test_mystrlen_with_newline) {
    char str1[] = "Test\nString";
    ck_assert_uint_eq(mystrlen(str1), 4);
    
    char str2[] = "\n";
    ck_assert_uint_eq(mystrlen(str2), 0);
    
    char str3[] = "NoNewline";
    ck_assert_uint_eq(mystrlen(str3), 9);
}
END_TEST

START_TEST(test_find_substring_basic) {
    char str[] = "Hello World!";
    char sub1[] = "World";
    char* result = find_substring(str, sub1);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "World!");
    
    char sub2[] = "Hello";
    result = find_substring(str, sub2);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "Hello World!");
    
    char sub3[] = "NotHere";
    result = find_substring(str, sub3);
    ck_assert_ptr_null(result);
}
END_TEST

START_TEST(test_find_substring_edge_cases) {
    
    // Подстрока длиннее строки
    char short_str[] = "Hi";
    char long_sub[] = "Hello";
    ck_assert_ptr_null(find_substring(short_str, long_sub));
    
    // Полное совпадение
    char str3[] = "ExactMatch";
    char sub3[] = "ExactMatch";
    char* result = find_substring(str3, sub3);
    ck_assert_ptr_nonnull(result);
    ck_assert_str_eq(result, "ExactMatch");
}
END_TEST

START_TEST(test_find_substring_multiple_occurrences) {
    char str[] = "acaqabab";
    char sub[] = "aba";
    char* result = find_substring(str, sub);
    ck_assert_ptr_nonnull(result);
    // Ваша функция возвращает последнее вхождение
    // Проверяем, что нашли правильную позицию
    ck_assert_int_eq(result - str, 4);
    ck_assert_str_eq(result, "abab");
}
END_TEST

Suite *example_suite_create(void) {
  Suite *suite = suite_create("Test_Home_Work_2");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, test_mystrlen_basic);
  tcase_add_test(tcase_core, test_mystrlen_with_newline);
  tcase_add_test(tcase_core, test_find_substring_basic);
  tcase_add_test(tcase_core, test_find_substring_edge_cases);
  tcase_add_test(tcase_core, test_find_substring_multiple_occurrences);

  suite_add_tcase(suite, tcase_core);

  return suite;
}

int main(void) {
  Suite *suite = example_suite_create();
  SRunner *suite_runner = srunner_create(suite);

  srunner_run_all(suite_runner, CK_NORMAL);
  // Получаем количество проваленных тестов.
  unsigned failed_count = srunner_ntests_failed(suite_runner);
  srunner_free(suite_runner);

  if (failed_count != 0) {
    // Сигнализируем о том, что тестирование прошло неудачно.
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}