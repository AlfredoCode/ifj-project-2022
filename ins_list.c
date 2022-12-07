/*
 * =================================================== *
 * Name:       ins_list.c                              *
 * Authors:    xhofma11                                * 
 *             xjadrn03                                *
 *             xsafar27                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include "error.h"
#include "ins_list.h"

// ============ INSTRUCTION LIST ============
void initInstList(instructList_T *instrList)
{
    instrList->activeElement = NULL;
    instrList->firstElement = NULL;
    instrList->lastElement = NULL;
}

void First(instructList_T *instrList)
{
	instrList->activeElement = instrList->firstElement;
}

void Next(instructList_T *instrList)
{
    if (instrList->activeElement != NULL) {
        instrList->activeElement= instrList->activeElement->next;
    }
}

//Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser
int insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest)
{
    instructElem newElement = (instructElem) malloc(sizeof(*newElement));
	if (!newElement) errHandler(INTERNAL_ERR, "Failed to malloc newElement\n");

	newElement->op1 = op1;
    newElement->op2 = op2;
    newElement->dest = dest;
    newElement-> operation = operation;

	newElement->previous = instrList->lastElement;	// Nastavení dat nového prvku seznamu
	newElement->next = NULL;
	if (instrList->lastElement != NULL) {
		instrList->lastElement->next = newElement;	
	} else {
		instrList->firstElement = newElement;		// Navázání prvku do seznamu
	}
	instrList->lastElement = newElement;	
    return SUCCESS_ERR;
}

int insertInstructionT(instructList_T *iList, instructElem instruction)
{
    instruction->previous = iList->lastElement;
    instruction->next = NULL;
	if (iList->lastElement != NULL) {
		iList->lastElement->next = instruction;
	} else {
		iList->firstElement  = instruction;
	}
	iList->lastElement = instruction;
    return SUCCESS_ERR;
}

instructElem popInstruction(instructList_T *iList)
{
    // Cant pop from empty stack
    if (!iList->lastElement) return NULL;

    // "Remove" active element ptr if I pop the element
    if (iList->activeElement == iList->lastElement) iList->activeElement = NULL;

    // instruction to pop
    instructElem instruction = iList->lastElement;

    // If the list has one item in it
    if (iList->firstElement == iList->lastElement){
        iList->firstElement = NULL;
        iList->lastElement = NULL;

        return instruction;
    }

    iList->lastElement = iList->lastElement->previous;
    iList->lastElement->next = NULL;

    return instruction;
}

void Dispose(instructList_T *instrList)
{
	while (instrList->firstElement != NULL) {
		instructElem tmpElementPtr;
		tmpElementPtr = instrList->firstElement;
		if (instrList->firstElement == instrList->activeElement) {
			instrList->activeElement = NULL;
		}
		if (instrList->firstElement == instrList->lastElement) {
			instrList->firstElement = NULL;
			instrList->lastElement = NULL;
		} else {
			instrList->firstElement = tmpElementPtr->next;
			instrList->firstElement->previous = NULL;
		}
		free(tmpElementPtr);
	}
}

