/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 * Last modif: 10/29/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__


// Enum for statement types
typedef enum {
    t_fun,
    t_int,
    t_float,
    t_str,
}stat_type;

// Internal structure used to store variables and functions inside htab
typedef struct statement {
    stat_type type;
    char* name;
    char* value;
} stat_t;


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
expr_El *getExpr(expression_T *exprList);
void insertExpr(expression_T *exprList, token_t *token);
void exprListDispose( expression_T *exprList );



// ACTUAL FUNCTIONS USED IN PARSER


int prog(); // Entry point to LL grammar rules
int parse();
int declareCheck();
int statement_list();
int statement_list_inside();
int expression_check();
int expression_check_inside();
int separators();

int condiCheck();
int checkIfStat();
int checkIfOperators();
int elseCheck();


#endif  // __PARSER_H__