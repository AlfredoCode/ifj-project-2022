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

#include <stdio.h>
#include <stdlib.h>

#include "htab.h"
#include "parser.h"

#define HTABSIZE 10

static int htab_setup(void **state)
{
    htab_t *htab = htab_init(HTABSIZE);
    if (htab == NULL) { return -1; }

    *state = htab;
    return 0;
}

static int htab_teardown(void **state)
{
    htab_free(*state);
    return 0;
}


void htab_insert_test(void **state)
{
    stat_t *retStat = htab_lookup_add(*state, "$ananas");
    assert_string_equal("$ananas", retStat->name);
}

void htab_insert_again_test(void **state)
{
    stat_t *retStat = htab_lookup_add(*state, "$ananas");
    assert_null(retStat);
}

void htab_insert_second_test(void **state)
{
    stat_t *retStat = htab_lookup_add(*state, "$feferonka");
    assert_string_equal("$feferonka", retStat->name);
}

void htab_erase_test(void **state)
{
    assert_true(htab_erase(*state, "$feferonka"));
    assert_false(htab_erase(*state, "$broskev"));
}

int main (void)
{
    const struct CMUnitTest htab[] = {
        cmocka_unit_test(htab_insert_test),
        cmocka_unit_test(htab_insert_again_test),
        cmocka_unit_test(htab_insert_second_test),
        cmocka_unit_test(htab_erase_test),
    };

    cmocka_run_group_tests(htab, htab_setup, htab_teardown);
    return 0;
}
