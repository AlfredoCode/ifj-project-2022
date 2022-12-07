/*
 * =================================================== *
 * Name:       expr_stack.h                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#ifndef __STACK_H__
#define __STACK_H__

#include "scanner.h"

typedef enum {
    sym_add,
    sym_sub,
    sym_mul,
    sym_div,
    sym_con,
    sym_lt,
    sym_gt,
    sym_lte,
    sym_gte,
    sym_eq,
    sym_neq,
    sym_lbr,
    sym_rbr,
    sym_id,
    sym_int,
    sym_float,
    sym_str,
    sym_null,
    sym_end,
    term_add,
    term_sub,
    term_mul,
    term_div,
    term_con,
    term_lt,
    term_gt,
    term_lte,
    term_gte,
    term_eq,
    term_neq,
    term_bracket,
    term_id,
    term_int,
    term_float,
    term_str,
    term_bool,
    term_null,
    err_uhoh
} p_symbol;

typedef struct stack_token {
    token_t *token;
    p_symbol symbol;
    struct stack_token *next;
} stack_token_t;


typedef struct prec_stack {
    int size;
    stack_token_t *arr;
} stack_t;

/**
 * @brief   Inits a new prec_stack
 * @param   Size of prec_stack
 * @retrun  New pre_stack
 */
stack_t *stackInit();

/**
 * @brief   Pushes new item on stack
 * @param   Stack to push to
 * @param   Ptr for item to push
 */
void stackPush(stack_t *stack, token_t *item);

/**
 * @brief   Pops item from stack
 * @param   Stack to pop from
 * @return  Ptr to poped item
 */
stack_token_t *stackPop(stack_t *stack);

/**
 * @brief   Returns item on stack
 * @param   Stack to peek on
 * @param   Index to peek on
 * @return  Item on index
 *
 * Index 0 means stack->top
 * You peak on stack->top - index
 */
stack_token_t *stackPeek(stack_t *stack, int index);

/**
 * @brief   Clears and frees stack
 * @param   Stack to clear
 */
void stackClear(stack_t *stack);

#endif
