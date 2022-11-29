/*
 * =================================================== *
 * Name:       generator.h                             *
 * Authors:    xsafar27                                *
 *             xhofma11                                *
 * Last modif: 11/28/2022                              *
 * =================================================== *
 */

#include "scanner.h"    // TYPES

#ifndef __GENERATOR_H__
#define __GENERATOR_H__


typedef struct instructionElement{
    char *op1;
    char *op2;
    char *dest;
    TYPES operation;

    struct instructionElement *next;
    struct instructionElement *previous;


}*instructElem;

typedef struct instructionList{
    instructElem firstElement;
    instructElem lastElement;
    instructElem activeElement;

}instructList_T;

void initInstList(instructList_T *instrList);
int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest); //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser


// Generate built-ins
// Reads
// Readi
// Readf
// Write
// Strlen
// Substring
// Ord
// Chr

// Functions

// Gen init

// ...

#endif // __GENERATOR_H__
