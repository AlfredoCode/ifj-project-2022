/*
 * =================================================== *
 * Name:       run_test.c                              *
 * Authors:    xsafar27                                * 
 * Last modif: 11/21/2022                              *
 * =================================================== *
 */

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdio.h>
#include <stdlib.h>

#include "symtable.h"
#include "expr_stack.h"
#include "scanner.h"
#include "parser.h"

#define HTABSIZE 10

// SETUP & TEARDOWN
static int htab_setup(void **state)
{
    htab_t *htab = htab_init(HTABSIZE);
    if (!htab) return -1;

    *state = htab;
    return 0;
}

static int htab_teardown(void **state)
{
    htab_free(*state);
    return 0;
}

static int expr_stack_setup(void **state)
{
    stack_t *stack = stackInit();
    if (!stack) return -1;

    *state = stack;
    return 0;
}

static int expr_stack_teardown(void **state)
{
    stack_t *stack = (stack_t*) *state;
    stack_token_t *token = stack->arr;
    while (token) {
        free(token->token);
        token = token->next;
    }

    stackClear(*state);
    return 0; 
}

static int expr_parse_setup(void **state)
{
    return 0;
}

static int expr_parse_teardown(void **state)
{
    return 0;
}

// HTAB
void htab_insert_test(void **state)
{
    stat_t *retStat = htab_lookup_add(*state, "$ananas");
    assert_string_equal("$ananas", retStat->name);
}

void htab_insert_again_test(void **state)
{
    stat_t *retStat = htab_lookup_add(*state, "$ananas");
    assert_string_equal("$ananas", retStat->name);
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

void htab_find_existing_test(void **state)
{
    stat_t *retStat = htab_find(*state, "$ananas");
    assert_string_equal("$ananas", retStat->name);
}

void htab_find_nonexisting_test(void **state)
{
    stat_t *retStat = htab_find(*state, "$okurka");
    assert_null(retStat);
}

void htab_insert_lots(void **state)
{
    htab_lookup_add(*state, "$jablko");
    htab_lookup_add(*state, "$okurka");
    htab_lookup_add(*state, "$feferonka");
    htab_lookup_add(*state, "$pazitka");
    htab_lookup_add(*state, "$avokado");
    htab_lookup_add(*state, "$celer");
    htab_lookup_add(*state, "$mrkev");
    htab_lookup_add(*state, "$brambora");
    htab_lookup_add(*state, "$tresen");

    assert_int_equal(10, htab_size(*state));
}

void print_stat(stat_t *data)
{
    printf("%s\n", data->name);
}

void htab_for_each_test(void **state)
{
    htab_for_each(*state, *print_stat);
}

// PREC_STACK
void expr_stack_push_test(void **state)
{
    token_t *token = malloc(sizeof(token_t));
    token->string = "Auto";

    stackPush(*state, token);
    assert_string_equal(stackPeek(*state, 0)->token->string, "Auto");
}

void expr_stack_push_more_test(void **state)
{
    token_t *token = malloc(sizeof(token_t));
    token->string = "Orangutan";
    stackPush(*state, token);

    token = malloc(sizeof(token_t));
    token->string = "Banan";
    stackPush(*state, token);

    assert_string_equal(stackPeek(*state, 0)->token->string, "Banan");
    assert_string_equal(stackPeek(*state, 1)->token->string, "Orangutan");
}

void expr_stack_pop_test(void **state)
{
    stack_token_t *token = stackPop(*state);
    assert_string_equal(token->token->string, "Banan"); 
    free(token->token);
    free(token);
}


// MAIN
int main (void)
{
    const struct CMUnitTest htab[] = {
        cmocka_unit_test(htab_insert_test),
        cmocka_unit_test(htab_insert_again_test),
        cmocka_unit_test(htab_insert_second_test),
        cmocka_unit_test(htab_erase_test),
        cmocka_unit_test(htab_find_existing_test),
        cmocka_unit_test(htab_find_nonexisting_test),
        cmocka_unit_test(htab_insert_lots),
        // cmocka_unit_test(htab_for_each_test)
    };

    const struct CMUnitTest expr_stack[] = {
        cmocka_unit_test(expr_stack_push_test),
        cmocka_unit_test(expr_stack_push_more_test),
        cmocka_unit_test(expr_stack_pop_test),
    };

    cmocka_run_group_tests(htab, htab_setup, htab_teardown);
    cmocka_run_group_tests(expr_stack, expr_stack_setup, expr_stack_teardown);
    return 0;
}
