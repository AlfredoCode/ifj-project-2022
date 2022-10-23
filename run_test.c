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

#include "htab.h"

#define HTABSIZE 5

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


void null_test_success(void **state)
{
}


void htab_insert_test(void **state)
{
    htab_lookup_add(*state, "Ananas");
    htab_pair_t *pair = htab_find(*state, "Ananas");
    assert_string_equal(pair->key, "Ananas");
}

void htab_insert_multiple_test(void **state)
{
    htab_lookup_add(*state, "Ananas");
    htab_lookup_add(*state, "Broskev");
    htab_lookup_add(*state, "Mandarinka");
    htab_lookup_add(*state, "Mango");
    htab_lookup_add(*state, "Hrozen");

    htab_pair_t *pair = htab_find(*state, "Mandarinka");
    assert_string_equal(pair->key, "Mandarinka");

    pair = htab_find(*state, "Ananas");
    assert_string_equal(pair->key, "Ananas");
    assert_int_equal(pair->value, 2);
}

void htab_erase_test(void **state)
{
    bool erase = htab_erase(*state, "Mandarinka");
    assert_true(erase);
}

void htab_resize_up_test(void **state)
{
    htab_lookup_add(*state, "Rajce");
    htab_lookup_add(*state, "Jablko");
    htab_lookup_add(*state, "Hruska");
    htab_lookup_add(*state, "Pomeranc");
    htab_lookup_add(*state, "Tresen");
    htab_lookup_add(*state, "Visne");
    htab_lookup_add(*state, "Svestka");
    htab_lookup_add(*state, "Bluma");
    htab_lookup_add(*state, "Sliva");
    htab_lookup_add(*state, "Mirabelka");
    htab_lookup_add(*state, "Broskev");
    htab_lookup_add(*state, "Oskeruse");
    htab_lookup_add(*state, "Mandle");
    htab_lookup_add(*state, "Boruvka");
    htab_lookup_add(*state, "Kastan");
    htab_lookup_add(*state, "Brusinka");
    
    assert_int_equal(htab_bucket_count(*state), 5);
}

int main (void)
{
    const struct CMUnitTest htab[] = {
        cmocka_unit_test(null_test_success),
        cmocka_unit_test(htab_insert_test),
        cmocka_unit_test(htab_insert_multiple_test),
        cmocka_unit_test(htab_erase_test),
        cmocka_unit_test(htab_resize_up_test),
    };

    cmocka_run_group_tests(htab, htab_setup, htab_teardown);
    return 0;
}
