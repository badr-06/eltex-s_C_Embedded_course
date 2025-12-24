#include <stdlib.h>
#include <check.h>

START_TEST(test_name)
{
    ck_assert_int_eq(1, 1);
}
END_TEST

Suite *example_suite_create(void){
    Suite *suite = suite_create("Test_Home_Work_1");
    TCase *tcase_core = tcase_create("core");

    tcase_add_test(tcase_core, test_name);
    suite_add_tcase(suite, tcase_core);

    return suite;
}

int main(void){

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