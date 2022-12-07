/*
 * =================================================== *
 * Name:       expr_stack.c                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <stdio.h>

#include "expr_stack.h"
#include "error.h"

stack_t *stackInit()
{
    stack_t *stack = malloc(sizeof(stack_t));
    if (!stack) errHandler(INTERNAL_ERR, "Failed to malloc stack in stackInit");

    stack->size = 0;
    stack->arr = NULL;
      
    return stack;
}


void stackPush(stack_t *stack, token_t *item)
{
    stack_token_t *token = malloc(sizeof(stack_token_t));
    if (!token) errHandler(INTERNAL_ERR, "Failed to malloc token in stackPush");

    stack->size++;

    token->token = item;
    token->next = stack->arr;
    stack->arr = token;
}


stack_token_t *stackPop(stack_t *stack)
{
    stack->size--;

    stack_token_t *ret = stack->arr;
    stack->arr = stack->arr->next;
    return ret;
}


stack_token_t *stackPeek(stack_t *stack, int index)
{
    stack_token_t *ret = stack->arr;
    while(index--) {
        ret = ret->next;
    }
    return ret;
}


void stackClear(stack_t *stack)
{
    stack_token_t *token = stack->arr;
    
    while (token) {
        stack->arr = token->next;
        free(token);
        token = stack->arr;
    }
    
    free(stack);
}
