/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 * Last modif: 10/29/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__

// Enum for statement types
typedef enum {
    t_fun,
    t_int,
    t_float,
    t_str,
}stat_type;

// Internal structure used to store variables and functions inside htab
typedef struct statement {
    stat_type type;
    char* name;
    char* value;
} stat_t;





// FOR INSERTING TOKENS PURPOSE UNTIL LEXER IS DONE


typedef struct tokenElement{
    int type;
    char* attribute;
    struct tokenElement *next;
    struct tokenElement *previous;
}token_El;

typedef struct {
	token_El* firstElement;
    token_El* lastElement;
    token_El* activeElement;
} token_T;

typedef enum {
    DOLLAR, ID, EQ, INT, COMMA, TOK_EOF, TOK_FUNCTION, LBRACKET, RBRACKET
}tokenTypes;

void init(token_T *tokenList);
token_El *getToken(token_T *tokenList);
void insertToken(token_T *tokenList, int type, void *attrib);




// ACTUAL FUNCTIONS USED IN PARSER


int prog(); // Entry point to LL grammar rules
int prologCheck(); // First token has to be prolog


#endif  // __PARSER_H__

