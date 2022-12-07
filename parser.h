/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__
#include "scanner.h"
#include "symtable.h"
#include "expr_parser.h"








// ACTUAL FUNCTIONS USED IN PARSER


int prog(); // Entry point to LL grammar rules
int parse();
int declareCheck();
int statement_list(struct htab *localTab);
int statement_list_inside(struct htab *table, p_return *ret_type);
int expression_check(struct htab *table);
int expression_check_inside(struct htab *table);
int separators(struct htab *table, p_return *ret_type);

int condiCheck(struct htab *table);
int checkIfStat(struct htab *table);
int checkIfOperators();
int elseCheck(struct htab *table);
int functionCheck();
int funcParams(struct htab *localTab, stat_t *statementIn, int index);
int checkWhile(struct htab *localTab);
int builtinParams();


#endif  // __PARSER_H__
