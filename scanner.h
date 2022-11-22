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
    SYM_OCT2, //fsm fix
    INT_S,
    DECIMAL,
    FLOAT_S,
    EXPO,
    EXPO_SIG,
    FLOAT_E_S,
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
} TYPES;

typedef struct{
    TYPES type;
    char* string;
    int integer;
    double decimal;
    KEYWORDS keyword;
} token_t;

// returns KEYWORD if keyword, -1 if not
int isKeyword(char *string);

bool GetToken(token_t *token);

#endif // __SCANNER_H__
