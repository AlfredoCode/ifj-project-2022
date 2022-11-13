/*
 * =================================================== *
 * Name:       prec_stack.c                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/13/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <stdio.h>

#include "prec_stack.h"

stack_t *stackInit(int size)
{
    stack_t *stack = malloc(sizeof(stack_t));

    if (!stack) {
        fprintf(stderr, "Failed to malloc stack in stackInit");
        exit(99);
    }

    stack->size = size;
    stack->arr = malloc(sizeof(token_t) * size);

    if (!stack->arr) {
        fprintf(stderr, "Failed to malloc stack->arr in stackInit");
        exit(99);

    }

    return stack;
}


void stackPush(stack_t *stack, token_t *item)
{
    stack_token_t *token = malloc(sizeof(stack_token_t));
    token->token = item;
    token->next = stack->arr;
    stack->arr = token;
}


token_t *stackPop(stack_t *stack)
{
    stack_token_t *ret = stack->arr;
    stack->arr = stack->arr->next;
    return ret->token;
}


token_t *stackPeek(stack_t *stack, int index)
{
    stack_token_t *ret = stack->arr;
    while(index--) {
        ret = ret->next;
    }
    return ret->token;
}


void stackClear(stack_t *stack)
{
    stack_token_t *token = stack->arr;
    
    while (token) {
        stack->arr = token->next;
        free(token->token);
        free(token);
        token = stack->arr;
    }
    
    free(stack);
}
