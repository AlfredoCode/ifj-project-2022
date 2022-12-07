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
 * @return  TRUE IF NO ERROR OCCURED OTHERWISE ERR
 */
int prog();

/**
 * @brief INITIALIZES SYMTABLES ETC, CHECKS PROLOG AND JUMPS INTO FIRST RULE OF LL
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE ERR
 */
int parse();

/**
 * @brief   CHECKS IF SEQUENCE OF TOKENS IS WHAT WE EXPECT FROM DECLARE FORMAT
 * @return  TRUE IF SEQUENCE WAS COMPLETED SUCCESFULLY OTHERWISE ERR
 */
int declareCheck();


/**
 * @brief   CHECKS IF EXPECTED TOKENS ARE PRESENT, THEY ARE PART OF LARGER STATEMENTS, STARTS TO DIVE DEEPER INTO LL
 * @param table - local/main symtable so we can define our vars
 * @param ret_type - we have to set the return type of variable for example $x = 5;  
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE ERR
 */
int statement_list(struct htab *localTab);

/**
 * @brief THESE ARE THE STATEMENTS ASSIGNED TO VAR FOR EXAMPLE <ID> = 5 + 5 - 2;
 * @param table - local/main symtable so we can define our vars
 * @param ret_type - we have to set the return type of variable for example $x = 5;    
 * @return  TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE ERR
 */
int statement_list_inside(struct htab *table, p_return *ret_type);

/**
 * @brief ASSIGNMENTS TO VAR ARE MANAGED HERE, SYMTABLE ADDITIONS ETC  
 * @param  table - ACCEPTS local/main symtable
 * @return TRUE IF PARSING WAS SUCCESSFULL, OTHERWISE ERR 
 */
int expression_check(struct htab *table);

/**
 * @brief CHECKS IF STATEMENT CONTAINS OPERATORS, SEMICOL OR OTHER SEPARATOR   
 * @param table - ACCEPTS local/main symtable  
 * @param  ret_type - we have to set the return type of variable for example $x = 5;    
 * @return TRUE IF PARSING WAS SUCCESSFULL OTHERWISE ERR
 */
int separators(struct htab *table, p_return *ret_type);

/**
 * @brief  EVALUATES GIVEN CONDITION, CHECKS ITS SYNTAX AND SEMANTICS
 * @param table -  ACCEPTS local/main symtable   
 * @return  TRUE IF STATEMENT WAS CORRECTLY WRITTEN, ERR IF NOT
 */
int condiCheck(struct htab *table);

/**
 * @brief CHECKS "IF STATEMENT" AND ITS SUBSECTIONS
 * @param table - local / main symtable  
 * @return  TRUE IF "IF STATEMENT" AND ITS SUBSECTIONS PARSED CORRECTLY OTHERWISE ERR
 */
int checkIfStat(struct htab *table);

/**
 * @brief CHECKS CONDITION OPERATORS
 * @return  TRUE IF OPERATOR OR R_PAR WAS FOUND, ERR IF NOT
 */
int checkIfOperators();

/**
 * @brief   CHECKS IF ELSE AND ITS SUBSECTIONS ARE PRESENT
 * @param table - ACCEPTS local/main symtable
 * @return TRUE IF ELSE AND ITS SUBSECTIONS ARE PRESENT, ERR IF NOT     
 */
int elseCheck(struct htab *table);

/**
 * @brief TRIES TO ADD FUNC ID TO FUNC SYMTABLE, SETS CURRENTRETURNTYPE OF FUNCTION AND COMPARE IT TO ITS ACTUAL RETURN VALUE,
 *          HANDLES SYNTAX OF FUNCTION AND IS SUBSECTIONS 
 * @return  TRUE IF ADDING TO SYMTABLE WAS SUCCESSFULL && SYNTAX AND SEMANTICS OF ALL SUBSECTION WAS CORRECT, ERR OTHERWISE
 */
int functionCheck();

/**
 * @brief HANDLES FUNCTION PARAMS, ADD THEM TO SYMTABLE, SETS THEIR DATA TYPE  
 * @param localTable - LOCAL TABLE OF FUNCTION, THAT IS WHERE WE SET NEW VARIABLES   
 * @param statementIn - PARAM - WE WANT TO SET ITS DATA TYPE
 * @param index - THIS IS INTERN INDEXER FOR THE EXPR_PARSER AND GENERATOR  
 * @return TRUE IF EVERY PARAM WAS SUCCESFULLY ADDED TO SYMTABLE && SYNTAX AND SEMANTICS WERE CORRECT, ERR OTHERWISE,  
 */
int funcParams(struct htab *localTab, stat_t *statementIn, int index);

/**
 * @brief HANDLES WHILE STATEMENT, CHECKS CONDITIONS VIA CHECKIFSTATEMENT  
 * @param localTab - local / main SYMTABLE SO WE KNOW WHICH VARIABLES WERE DECLARED
 * @return TRUE IF SEMANTICS AND SYNTAX WERE CORRECT, ERR OTHERWISE
 */
int checkWhile(struct htab *localTab);

/**
 * @brief FUNCTION TO HANDLE FUNCTION PARAMS WHEN CALLING A FUNCTION
 * @return SUCCESS IF SEMANTICS, SYNTAX WERE CORRECT, ERR IF OTHERWISE
 */
int builtinParams();

#endif  // __PARSER_H__

