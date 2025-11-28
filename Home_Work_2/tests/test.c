#include <check.h>
#include <stdlib.h>

#include "../lib/lib.h"

START_TEST(test_swap_array_basic) {
  int array[] = {1, 2, 3, 4, 5};
  int expected[] = {5, 4, 3, 2, 1};

  swap_array(5, array);

  for (int i = 0; i < 5; i++) {
    ck_assert_int_eq(array[i], expected[i]);
  }
}
END_TEST

START_TEST(test_swap_array_empty) {
  int array[] = {};

  // проверка на пустой массив
  swap_array(0, array);

  ck_assert(1);  // Если дошли сюда - тест пройден
}
END_TEST

START_TEST(test_swap_array_single_element) {
  int array[] = {42};
  int expected[] = {42};

  swap_array(1, array);

  ck_assert_int_eq(array[0], expected[0]);
}
END_TEST

START_TEST(test_swap_array_negative_numbers) {
  int array[] = {-1, -2, -3, -4};
  int expected[] = {-4, -3, -2, -1};

  swap_array(4, array);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(array[i], expected[i]);
  }
}
END_TEST

START_TEST(test_swap_array_even_length) {
  int array[] = {1, 2, 3, 4};
  int expected[] = {4, 3, 2, 1};

  swap_array(4, array);

  for (int i = 0; i < 4; i++) {
    ck_assert_int_eq(array[i], expected[i]);
  }
}
END_TEST

START_TEST(test_swap_array_odd_length) {
  int array[] = {1, 2, 3, 4, 5};
  int expected[] = {5, 4, 3, 2, 1};

  swap_array(5, array);

  for (int i = 0; i < 5; i++) {
    ck_assert_int_eq(array[i], expected[i]);
  }
}
END_TEST

// ========================================================

START_TEST(test_change_array_3x3) {
  int array[3][3] = {{0}};
  int expected[3][3] = {{0, 0, 1}, {0, 1, 1}, {1, 1, 1}};

  change_array(3, 3, array);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_int_eq(array[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_change_array_4x4) {
  int array[4][4] = {{0}};
  int expected[4][4] = {{0, 0, 0, 1}, {0, 0, 1, 1}, {0, 1, 1, 1}, {1, 1, 1, 1}};

  change_array(4, 4, array);

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ck_assert_int_eq(array[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_change_array_2x2) {
  int array[2][2] = {{0}};
  int expected[2][2] = {{0, 1}, {1, 1}};

  change_array(2, 2, array);

  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      ck_assert_int_eq(array[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_change_array_1x1) {
  int array[1][1] = {{0}};
  int expected[1][1] = {{1}};

  change_array(1, 1, array);

  ck_assert_int_eq(array[0][0], expected[0][0]);
}
END_TEST

START_TEST(test_change_array_5x5) {
  int array[5][5] = {{0}};
  int expected[5][5] = {{0, 0, 0, 0, 1},
                        {0, 0, 0, 1, 1},
                        {0, 0, 1, 1, 1},
                        {0, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1}};

  change_array(5, 5, array);

  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      ck_assert_int_eq(array[i][j], expected[i][j]);
    }
  }
}
END_TEST

// =============================================================

START_TEST(test_snail_array_2x2) {
  int n = 2;
  int array[2][2];
  int array_res[2][2];

  int expected[2][2] = {{1, 2}, {4, 3}};

  snail_array(n, array, array_res);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(array_res[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_snail_array_3x3) {
  int n = 3;
  int array[3][3];
  int array_res[3][3];

  // Ожидаемая улитка для 3x3
  int expected[3][3] = {{1, 2, 3}, {8, 9, 4}, {7, 6, 5}};

  snail_array(n, array, array_res);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(array_res[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_snail_array_1x1) {
  int n = 1;
  int array[1][1];
  int array_res[1][1];

  int expected[1][1] = {{1}};

  snail_array(n, array, array_res);

  ck_assert_int_eq(array_res[0][0], expected[0][0]);
}
END_TEST

START_TEST(test_snail_array_4x4) {
  int n = 4;
  int array[4][4];
  int array_res[4][4];

  // Ожидаемая улитка для 4x4
  int expected[4][4] = {
      {1, 2, 3, 4}, {12, 13, 14, 5}, {11, 16, 15, 6}, {10, 9, 8, 7}};

  snail_array(n, array, array_res);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(array_res[i][j], expected[i][j]);
    }
  }
}
END_TEST

START_TEST(test_snail_array_5x5) {
  int n = 5;
  int array[5][5];
  int array_res[5][5];

  // Ожидаемая улитка для 5x5
  int expected[5][5] = {{1, 2, 3, 4, 5},
                        {16, 17, 18, 19, 6},
                        {15, 24, 25, 20, 7},
                        {14, 23, 22, 21, 8},
                        {13, 12, 11, 10, 9}};

  snail_array(n, array, array_res);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ck_assert_int_eq(array_res[i][j], expected[i][j]);
    }
  }
}
END_TEST

Suite *example_suite_create(void) {
  Suite *suite = suite_create("Test_Home_Work_2");
  TCase *tcase_core = tcase_create("core");

  tcase_add_test(tcase_core, test_swap_array_basic);
  tcase_add_test(tcase_core, test_swap_array_empty);
  tcase_add_test(tcase_core, test_swap_array_single_element);
  tcase_add_test(tcase_core, test_swap_array_negative_numbers);
  tcase_add_test(tcase_core, test_swap_array_even_length);
  tcase_add_test(tcase_core, test_swap_array_odd_length);

  tcase_add_test(tcase_core, test_change_array_3x3);
  tcase_add_test(tcase_core, test_change_array_4x4);
  tcase_add_test(tcase_core, test_change_array_2x2);
  tcase_add_test(tcase_core, test_change_array_1x1);
  tcase_add_test(tcase_core, test_change_array_5x5);

  tcase_add_test(tcase_core, test_snail_array_1x1);
  tcase_add_test(tcase_core, test_snail_array_2x2);
  tcase_add_test(tcase_core, test_snail_array_3x3);
  tcase_add_test(tcase_core, test_snail_array_4x4);
  tcase_add_test(tcase_core, test_snail_array_5x5);

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