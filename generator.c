/*
 * =================================================== *
 * Name:       generator.c                             *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 *             xjadrn03                                *
 * Last modif: 11/28/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "generator.h"
#include "error.h"

instructList_T *instrList;

/*List functions*/
void initInstList(instructList_T *instrList){
    instrList->activeElement = NULL;
    instrList->firstElement = NULL;
    instrList->lastElement = NULL;

}

int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest){ //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser
    instructElem newElement = (instructElem) malloc(sizeof(*newElement));
	if(newElement == NULL){
		return INTERNAL_ERR; // return 99
	}
	newElement->op1 = op1;
    newElement->op2 = op2;
    newElement->dest = dest;
    newElement-> operation = operation;


	newElement->next = instrList->firstElement;	// Nastavení dat nového prvku seznamu
	newElement->previous = NULL;
	if(instrList->lastElement != NULL){
		instrList->firstElement->previous = newElement;	

	}
	else{
		instrList->lastElement = newElement;		// Navázání prvku do seznamu
	}
	
	instrList->firstElement = newElement;	
    return SUCCESS_ERR;

}

int main(){
    instrList  = (instructList_T*)malloc(sizeof(*instrList));
    if(instrList == NULL){
        fprintf(stderr,"Malloc failure\n");
        return INTERNAL_ERR;
    }
    initInstList(instrList);
    insertInstruction(instrList, ADD, "x","y","z");

    return 0;
}
