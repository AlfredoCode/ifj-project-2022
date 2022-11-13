/*
 * =================================================== *
 * Name:       prec_stack.h                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/09/2022                              *
 * =================================================== *
 */

#ifndef __STACK_H__
#define __STACK_H__

#include "scanner.h"

typedef struct stack_token {
    token_t *token;
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
stack_t *stackInit(int size);

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
token_t *stackPop(stack_t *stack);

/**
 * @brief   Returns item on stack
 * @param   Stack to peek on
 * @param   Index to peek on
 * @return  Item on index
 *
 * Index 0 means stack->top
 * You peak on stack->top - index
 */
token_t *stackPeek(stack_t *stack, int index);

/**
 * @brief   Clears and frees stack
 * @param   Stack to clear
 */
void stackClear(stack_t *stack);

#endif
