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

/**
 * @brief   
 * @return  
 */
int prog();

/**
 * @brief   
 * @return  
 */
int parse();

/**
 * @brief   
 * @return  
 */
int declareCheck();

/**
 * @brief   
 * @param   
 * @param   
 * @return  
 */
int statement_list(struct htab *localTab);

/**
 * @brief
 * @param
 * @param
 * @return
 */
int statement_list_inside(struct htab *table, p_return *ret_type);

/**
 * @brief   
 * @param   
 * @return  
 */
int expression_check(struct htab *table);

/**
 * @brief   
 * @param   
 * @return   
 */
int expression_check_inside(struct htab *table);

/**
 * @brief   
 * @param   
 * @param   
 * @returm
 */
int separators(struct htab *table, p_return *ret_type);

/**
 * @brief   
 * @param   
 * @return  
 */
int condiCheck(struct htab *table);

/**
 * @brief   
 * @param   
 * @return  
 */
int checkIfStat(struct htab *table);

/**
 * @brief   
 * @return  
 */
int checkIfOperators();

/**
 * @brief   
 * @param   
 * @return  
 */
int elseCheck(struct htab *table);

/**
 * @brief   
 * @return  
 */
int functionCheck();

/**
 * @btierf  
 * @param   
 * @param   
 * @param   
 * @return  
 */
int funcParams(struct htab *localTab, stat_t *statementIn, int index);

/**
 * @brief   
 * @param
 * @return
 */
int checkWhile(struct htab *localTab);

/**
 * @brief   
 * @return  
 */
int builtinParams();

#endif  // __PARSER_H__

