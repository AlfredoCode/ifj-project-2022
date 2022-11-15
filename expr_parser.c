/*
 * =================================================== *
 * Name:       expr_parse.c                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/16/2022                              *
 * =================================================== *
 */

#include <stdio.h>

#include "expr_parser.h"
#include "expr_stack.h"
#include "error.h"
#include "scanner.h"
#include "symtable.h"

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

stack_token_t *nextNonTerm(stack_t *stack)
{
    // I want to return sym, not term.
    // So I look past the first one if its term.
    if (stackPeek(stack, 0)->symbol > sym_end) {
        return stackPeek(stack, 1);
    } else {
        return stackPeek(stack, 0);
    }
}

int arithmetic_check(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);
    
    if (tok1->symbol == term_str) {
        if (tok2->symbol == term_str) {
            // Remove top two terms
            stackPop(stack);
            stackPop(stack);

            // Change top item symbol to str
            // I only care about the symbol now anyways
            stackPeek(stack, 0)->symbol = term_str;

            return 0;
        }
        err_msg(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
        return 1;
    } 

    // I hate this thing, but here we go
    // if both tokens are either int or float
    if ((tok1->symbol == term_float || tok1->symbol == term_int)
           && (tok2->symbol == term_float || tok2->symbol == term_int)) {
        // Set the type to float if at least one of them is float, int otherwise
        if (tok1->symbol == term_float || tok2->symbol == term_float) {
            stackPeek(stack, 2)->symbol = term_float;     
        } else {
            stackPeek(stack, 2)->symbol = term_int;     
        }

        // Pop the two items that are not needed anymore
        stackPop(stack);
        stackPop(stack);

        return 0;
    }
    
    err_msg(SYNTAX_ERR, "Error in expresion - incompatible datatypes"); 
    return 1;
}

int evaluate_bool(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);

    if (tok1->symbol == term_str) {
        if (tok2->symbol == term_str) {
            // same as in line 87
            stackPop(stack);
            stackPop(stack); 

            stackPeek(stack, 0)->symbol = term_bool;

            return 0;
        }
        err_msg(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
        return 1; 
    }

    // Much easier, are both numbers? Great, its a bool.
    if ((tok1->symbol == term_float || tok1->symbol == term_int)
            && (tok2->symbol == term_float || tok2->symbol == term_int)) {
        stackPop(stack);
        stackPop(stack);

        stackPeek(stack, 0)->symbol = term_bool;
        
        return 0;
    }

    err_msg(SYNTAX_ERR, "Error in expresion - incompatible datatypes"); 
    return 1;
}

int evaluate_brackets(stack_t *stack);

int evaluate(stack_t *stack);

p_return expr_parse(htab_t *symTable, token_t start_token);
