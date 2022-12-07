/*
 * =================================================== *
 * Name:       expr_list.c                             *
 * Authors:    xhofma11                                * 
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#include "expr_list.h"
#include "scanner.h"
#include "error.h"

void expressionInit(expression_T *exprList)
{
    exprList->firstElement = NULL;
	exprList->activeElement = NULL;		// Nastavení default hodnot seznamu
	exprList->lastElement = NULL;
}

expr_El getExpr(expression_T *exprList)
{
    if (exprList->activeElement == NULL) {
        exprList->activeElement = exprList->lastElement;
        return exprList->activeElement; 
    }
    exprList->activeElement = exprList->activeElement->previous;
    return exprList->activeElement;
}

void insertExpr(expression_T *exprList, token_t *token)
{
    expr_El newElement = (expr_El) malloc(sizeof(expr_El));
	if(!newElement) errHandler(INTERNAL_ERR, "Failed to malloc newWlement\n");
	
	newElement->token = token;
	newElement->next = exprList->firstElement;	// Nastavení dat nového prvku seznamu
	newElement->previous = NULL;
	if(exprList->lastElement != NULL){
		exprList->firstElement->previous = newElement;	
	}
	else{
		exprList->lastElement = newElement;		// Navázání prvku do seznamu
	}
	
	exprList->firstElement = newElement;	
}

void exprListDispose( expression_T *exprList )
{
    // Deklarace dvou pomocných prvků typu ListElementPtr
	expr_El nextEl;	               
	expr_El firstEl;
	firstEl = exprList->firstElement;

    // Cyklus, který maže vždy první prvek seznamu, dokud tam nějaký prvek je
	while(firstEl != NULL){
		nextEl= firstEl->next; 	    
        free(firstEl->token);
		free(firstEl);
		firstEl = nextEl;
	}
    
    // Uvedení seznamu do init stavu
	exprList->activeElement = NULL;
	exprList->firstElement = NULL;
	exprList->lastElement = NULL;
}
