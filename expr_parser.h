/*
 * =================================================== *
 * Name:       prec_parser.h                           *
 * Authors:    xsafar27                                *
 * Last modif: 11/20/2022                              *
 * =================================================== *
 */

#ifndef __PREC_PARSER_H__
#define __PREC_PARSER_H__

#include "scanner.h"
#include "expr_stack.h"
#include "symtable.h"
// #include "parser.h"
#include "generator.h"







// Used for storing expression tokens
typedef struct exprElement{
    token_t *token;
    struct exprElement *next;
    struct exprElement *previous;
}*expr_El;

typedef struct {
	expr_El firstElement;
    expr_El lastElement;
    expr_El activeElement;
} expression_T;

void expressionInit(expression_T *exprList);
expr_El getExpr(expression_T *exprList);
void insertExpr(expression_T *exprList, token_t *token);
void exprListDispose( expression_T *exprList );

typedef enum {
    ret_bool,
    ret_int,
    ret_float,
    ret_string,
    ret_null,
    ret_uhoh
} p_return;

/**
 * @brief   converts token to symbol
 * @param   token to convert
 * @return  prec_parser rule
 *
 * Coverts token to p_symbol enum for use in prec_parser
 */ 
p_symbol tokenToSymbol(token_t *token);

/**
 * @brief   gets next non-terminal
 * @param   stack to pull from
 * @return  non-terminal
 *
 * Returns next non-terminal token from stack.
 */
stack_token_t *nextNonTerm(stack_t *stack);

/**
 * @brief   checks corectness of expression
 * @param   stack to check on
 * @return  0 if correct, 1 otherwise
 */
int arithmetic_check(stack_t *stack);

/**
 * @brief   evaluates bool expression
 * @param   stack to evaluate on
 * @return  1 if error, 0 otherwise
 *
 * Evaluates if bool expression is correct.
 * Reduces stack.
 */
int evaluate_bool(stack_t *stack);

/**
 * @brief   evaluates brackets
 * @param   stack to evaluate on
 * @return  1 if error, 0 otherwise
 *
 * Evaluates if bracket expression is correct.
 * Reduces stack.
 */
int evaluate_brackets(stack_t *stack);

/**
 * @brief   evaluates concatenation
 * @param   stack to evaluate on
 * @return  1 if error, 0 otherwise
 *
 * Evaluates if concatenation expression is correct.
 * Reduces stack.
 */
int evaluate_concatenation(stack_t *stack, instructList_T *iList);

/**
 * @brief   evaluates expression
 * @param   stack to evaluate on
 * @param   htab to check vars in
 * @return  1 if error, 0 otherwise
 *
 * Evaluates if expression is correct.
 * Reduces stack
 */
int evaluate(stack_t *stack, htab_t *symtable, instructList_T *iList);

/**
 * @brief   gets type of last token
 * @param   stack to check on
 * @return  type of last term
 */
p_return get_last(stack_t *stack);

/**
 * @brief   main expr_parser component
 * @param   symbol htab
 * @param   dll of tokens
 * @param   variable to return to
 * @return  final datatype
 */
p_return expr_parse(htab_t *symTable, expression_T *list, instructList_T *iList, char* retVar);

#endif // __PREC_PARSER_H__
