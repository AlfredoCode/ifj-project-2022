/*
 * =================================================== *
 * Name:       run_test.c                              *
 * Authors:    xsafar27                                * 
 * Last modif: 10/19/2022                              *
 * =================================================== *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "htab.h"

#define HTABSIZE 100

static int setup(void **state)
{
    htab_t *htab = htab_init(HTABSIZE);
    if (htab == NULL) { return -1; }

    *state = htab;

    return 0;
}

static int teardown(void **state)
{
    htab_free(*state);
    return 0;
}


void null_test_success(void **state)
{
}


void htab_insert_test(void **state)
{
    htab_lookup_add(*state, "Ananas");
    htab_pair_t *pair = htab_find(*state, "Ananas");
    assert_string_equal(pair->key, "Ananas");
}


int main (void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(htab_insert_test),
    };

    cmocka_run_group_tests(tests, setup, teardown);
    return 0;
}
