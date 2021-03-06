#include <stdio.h>
#include <stdlib.h>
#include <check.h>

#include "lib.h"

char * query_string = "key1&key2=value2&key3=value3";

START_TEST (printing_for_coverage)
{
    print_http_headers();
}
END_TEST

Suite * coverage_bs_suite(void)
{
    Suite * s = NULL;
    TCase * t = NULL;

    s = suite_create("c-cgi-framework coverage bs tests");
    t = tcase_create("");

    tcase_add_test(t, printing_for_coverage);

    suite_add_tcase(s, t);

    return s;
}

START_TEST (query_string_empty)
{
    get_request_vars();

    char * value = get_request_var("key1");
    ck_assert(value == NULL);

    free_request_vars();
}
END_TEST

START_TEST (query_string_no_match)
{
    setenv("QUERY_STRING", query_string, 0);
    get_request_vars();

    char * value = get_request_var("key4");
    ck_assert(value == NULL);

    free_request_vars();
    unsetenv("QUERY_STRING");
}
END_TEST

START_TEST (query_string_match)
{
    setenv("QUERY_STRING", query_string, 0);
    get_request_vars();

    char * value = get_request_var("key2");
    ck_assert_str_eq(value, "value2");

    free_request_vars();
    unsetenv("QUERY_STRING");
}
END_TEST

START_TEST (query_string_match_no_value)
{
    setenv("QUERY_STRING", query_string, 0);
    get_request_vars();

    char * value = get_request_var("key1");
    ck_assert_str_eq(value, "key1");

    free_request_vars();
    unsetenv("QUERY_STRING");
}
END_TEST

Suite * query_string_suite(void)
{
    Suite * s = NULL;
    TCase * t = NULL;

    s = suite_create("c-cgi-framework query_string tests");
    t = tcase_create("");

    tcase_add_test(t, query_string_empty);
    tcase_add_test(t, query_string_no_match);
    tcase_add_test(t, query_string_match);
    tcase_add_test(t, query_string_match_no_value);

    suite_add_tcase(s, t);

    return s;
}

START_TEST (a_few_good_elements)
{
    char * html =
        h1(
            "attr",
                "attr1=\"value1\"",
            "text1",
            div(
                input(
                    "attr",
                        "attr2=\"value2\""
                ),
                "text2"
            )
        );

    ck_assert_str_eq(html, "<h1 attr1=\"value1\">text1<div><input attr2=\"value2\" />text2</div></h1>");
}
END_TEST

Suite * element_suite(void)
{
    Suite * s = NULL;
    TCase * t = NULL;

    s = suite_create("c-cgi-framework element tests");
    t = tcase_create("");

    tcase_add_test(t, a_few_good_elements);

    suite_add_tcase(s, t);

    return s;
}

START_TEST (add_null_ptr_to_ptr_list)
{
    add_ptr_to_free_list(NULL);
}
END_TEST

START_TEST (free_ptr_list)
{
    free_ptrs();
}
END_TEST

Suite * ptr_list_suite(void)
{
    Suite * s = NULL;
    TCase * t = NULL;

    s = suite_create("c-cgi-framework ptr_list tests");
    t = tcase_create("");

    tcase_add_test(t, add_null_ptr_to_ptr_list);
    tcase_add_test(t, free_ptr_list);

    suite_add_tcase(s, t);

    return s;
}

int main()
{
    int failed = 0;
    int i = 0;
    Suite * coverage_bs_s = coverage_bs_suite();
    Suite * query_string_s = query_string_suite();
    Suite * element_s = element_suite();
    Suite * ptr_list_s = ptr_list_suite();
    SRunner * runner[4] = { NULL };

    runner[0] = srunner_create(coverage_bs_s);
    runner[1] = srunner_create(query_string_s);
    runner[2] = srunner_create(element_s);
    runner[3] = srunner_create(ptr_list_s);

    for (i = 0; i < 4; i++) {
        srunner_set_fork_status(runner[i], CK_NOFORK);
        srunner_run_all(runner[i], CK_VERBOSE);
        failed += srunner_ntests_failed(runner[i]);
        srunner_free(runner[i]);
    }

    printf("\n\n\nFailures: %d\n\n", failed);

    return failed;
}
