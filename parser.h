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
 * @brief   CHECKS IF DECLARE IS PRESENT AND CONTINUES WITH PARSING      
 * @return  TRUE IF NO ERROR OCCURED OTHERWISE FALSE
 */
int prog();

/**
 * @brief INITIALIZES SYMTABLES ETC, CHECKS PROLOG AND JUMPS INTO FIRST RULE OF LL
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE FALSE
 */
int parse();

/**
 * @brief   CHECKS IF SEQUENCE OF TOKENS IS WHAT WE EXPECT FROM DECLARE FORMAT
 * @return  TRUE IF SEQUENCE WAS COMPLETED SUCCESFULLY OTHERWISE FALSE
 */
int declareCheck();


/**
 * @brief   CHECKS IF EXPECTED TOKENS ARE PRESENT, THEY ARE PART OF LARGER STATEMENTS, STARTS TO DIVE DEEPER INTO LL
 * @param table - local/main symtable so we can define our vars
 * @param ret_type - we have to set the return type of variable for example $x = 5;  
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE FALSE
 */
int statement_list(struct htab *localTab);

/**
 * @brief THESE ARE THE STATEMENTS ASSIGNED TO VAR FOR EXAMPLE <ID> = 5 + 5 - 2;
 * @param table - local/main symtable so we can define our vars
 * @param ret_type - we have to set the return type of variable for example $x = 5;    
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE FALSE
 */
int statement_list_inside(struct htab *table, p_return *ret_type);

/**
 * @brief ASSIGNMENTS TO VAR ARE MANAGED HERE, SYMTABLE ADDITIONS ETC  
 * @param  table - ACCEPTS local/main symtable
 * @return TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE FALSE 
 */
int expression_check(struct htab *table);

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

