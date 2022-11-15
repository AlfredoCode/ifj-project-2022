/*
 * =================================================== *
 * Name:       prec_parse.c                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/14/2022                              *
 * =================================================== *
 */

#include <stdio.h>

#include "prec_parser.h"
#include "prec_stack.h"
#include "error.h"
#include "scanner.h"

p_symbol tokenToTerminal(token_t *token)
{
    switch (token->type) {
        case ADD:
            return sym_add;
        case SUB:
            return sym_sub;
        case MUL:
            return sym_mul;
        case DIV:
            return sym_div;
        case DOLLAR:
            return sym_dollar;
        case ID:
            return sym_id;
        case R_PAR:
            return sym_rbr;
        case L_PAR:
            return sym_lbr;
        case EQ:
            return sym_eq;
        case LESS:
            return sym_lt;
        case LESS_EQ:
            return sym_lte;
        case GREAT:
            return sym_gt;
        case GREAT_EQ:
            return sym_gte;
        case KONKAT:
            return sym_con;
        case NOT_EQ:
            return sym_neq;
        case STRING_T:
            return sym_str;
        case INT_T:
            return sym_int;
        case FLOAT_T:
            return sym_float;

        case SEMICOL:
        case L_BRAC:
            return sym_end;

        default:
            err_msg(SYNTAX_ERR, "Bad expression");
            return err_uhoh;
    }

    // how did you even get here
    return err_uhoh;
}

stack_token_t nextNonTerm(stack_t *stack)
{
    if (stackPeek(stack, 0) > 
}

int arithmetic_check(stack_t *stack);

int evaluate_bool(stack_t *stack);

int evaluate_brackets(stack_t *stack);

int evaluate(stack_t *stack);
