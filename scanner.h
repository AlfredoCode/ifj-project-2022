/*
 * =================================================== *
 * Name:       scanner.h                               *
 * Authors:    xmoskv01                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    S,
    DIV_S,
    BLOCK_COM_S,
    BLOCK_COM,
    LINE_COM_S,
    ID_S,
    TYPE_S,
    ENDMARK,
    ASSIG_S,
    EQEQ,
    LESS_S,
    GREAT_S,
    NOT,
    NOT_EQ_S,
    STRING_START,
    ESCAPE_SEQ,
    SYM_HEX1,
    SYM_HEX2,
    SYM_OCT1,
    SYM_OCT2,
    INT_S,
    DECIMAL,
    FLOAT_S,
    EXPO,
    EXPO_SIG,
    FLOAT_E_S,
    OPENMARK,
    OPENMARK2,
    OPENMARK3,
    OPENMARK4,
} STATES;

typedef enum {
    ELSE,
    FLOAT,
    FUNCTION,
    IF,
    INT,
    NULL_K,
    RETURN,
    STRING,
    VOID,
    WHILE,
} KEYWORDS;

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    DOLLAR,
    ID,
    KEYWORD,
    TYPE,
    EOF_T,
    R_BRAC,
    L_BRAC,
    R_PAR,
    L_PAR,
    SEMICOL,
    ASSIG,
    EQ,
    LESS,
    LESS_EQ,
    PHP,
    GREAT,
    GREAT_EQ,
    KONKAT,
    NOT_EQ,
    COL,
    STRING_T,
    INT_T,
    FLOAT_T,
    COMMA
} TYPES;

typedef struct{
    TYPES type;
    char* string;
    int integer;
    double decimal;
    KEYWORDS keyword;
} token_t;

/**
 * @brief   checkf if string is keyword
 * @param   string to check
 * @return  KEYWORD, or -1 if not
 */
int isKeyword(char *string);

/**
 * @brief check if there is a prolog at the begging of the file
 * @return true if its there, false if not
 */
bool GetProlog();

/**
 * @brief gets a token based on read characters from source program
 * @param token is an output parameter containing information about returned token
 * @return true if sequence is valid, false if not
 */
bool GetToken(token_t *token);

#endif // __SCANNER_H__
