/*
 * =================================================== *
 * Name:       expr_list.h                             *
 * Authors:    xhofma11                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#ifndef __EXPR_LIST_H__
#define __EXPR_LIST_H__

#include "scanner.h"

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

/**
 * @brief   Inits expression list
 * @param   Expression list to init 
 */
void expressionInit(expression_T *exprList);

/**
 * @brief   Gets next expr in expr list
 * @param   List to get expr from
 * @return  Next expression from list
 */
expr_El getExpr(expression_T *exprList);

/**
 * @brief  Inserts epxr at the end of the list 
 * @param  List to insert to
 * @param  Token to insrt
 */
void insertExpr(expression_T *exprList, token_t *token);

/**
 * @brief Disposes expression list
 * @param List to dispose
 */
void exprListDispose( expression_T *exprList );

#endif
