/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xhofma11                                * 
 * Last modif: 10/29/2022                              *
 * =================================================== *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "scanner.h"
#include "error.h"

bool token_res = 0;
token_t token;

// int errHandler(int result, char* msg){
//     if(!result){
//         if(!strcmp(msg, "lexical")){
//             fprintf(stderr,"Lexical error ---> UNKNOWN TOKEN <---\n");
//             return LEX_ERR;
//         }
//         else if(!strcmp(msg, "missing_declare")){

//         }
        
//     }

// }

int declareCheck(){
    // SEMANTIC  - Podívat se do tabulky symbolů, zda je ID rovno 'declare'
token_res = GetToken(&token);       // levá závorka
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_PAR){
        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
        return SYNTAX_ERR;
    }

    token_res = GetToken(&token);   // strict_types
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != ID){
        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
        return SYNTAX_ERR;
    }
    // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU ID


    token_res = GetToken(&token);   // rovná se
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != ASSIG){
        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
        return SYNTAX_ERR;
    }


    token_res = GetToken(&token);   // int
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != INT_T){
        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
        return SYNTAX_ERR;
    }
    // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU INT


    token_res = GetToken(&token);   // pravá závorka
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != R_PAR){
        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
        return SYNTAX_ERR;
    }

    token_res = GetToken(&token);   // středník
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != SEMICOL){
        fprintf(stderr, "Syntax error ---> MISSING SEMICOL <---\n");
        return SYNTAX_ERR;
    }
    
    return SUCCESS_ERR;
}

/********************************PARSING SECTION********************************************************/

int parse(){

    int res = SUCCESS_ERR;
    
    token_El *token;


    // LEXER
    // For int
        char* val = "5";
    //For string
        char *str = "SuperID";

        char* empty = "";

    // SIMULATION OF TOKEN LIST
    insertToken(&tokenList, DOLLAR, empty);
    insertToken(&tokenList, ID, str); 
    insertToken(&tokenList, EQ, empty);    
    insertToken(&tokenList, INT, val);
    insertToken(&tokenList, COMMA, empty);
    insertToken(&tokenList, TOK_EOF, empty);

    token = getToken(&tokenList); // GETING THE FIRST TOKEN


    // printf("----------------------------------------\n");

    // RESET the activeElement


    tokenList.activeElement = tokenList.lastElement;
    while((token = getToken(&tokenList))){

    switch(token.type){
        case EOF_T:
            fprintf(stderr,"Syntax error ---> EMPTY FILE <---\n");
            return SYNTAX_ERR;  // EMPTY FILE 
        case PHP:
            res = prog(); // First rule of LL
            return res;
        default:
            fprintf(stderr,"Syntax error ---> MISSING PROLOG <---\n");
            return SYNTAX_ERR;  // EMPTY FILE 
        
        }

    return SUCCESS_ERR;
    }
}

int prog(){
    int res;
    token_res = GetToken(&token);   
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    parse();
    switch(token.type){
        case ID:
            res = declareCheck();
            if(res == SUCCESS_ERR){
                res = statement_list();
            }
            // printf("%d\n",res); //DEBUG
            return res; 
        default:
            fprintf(stderr,"Syntax error ---> MISSING DECLARE <---\n");
            return SYNTAX_ERR;   

    }

}


int statement_list(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }

    switch(token.type){
        case EOF_T: // FOUND ?>       TODO, it is possible to do if(...){ ?> which is not valid
            // printf("Found the end\n");   //DEBUG
            return SUCCESS_ERR;
        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            res = expression_check();

            return res;
        case KEYWORD: //  SEMANTIC - if, function, else, while, return only, otherwise SYNTAX_ERR

        default:
            return SYNTAX_ERR;

    }


    return res;
}


int expression_check(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(!(token.type == ID)){
        fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
        return SYNTAX_ERR;
    }

    token_res = GetToken(&token);
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if((token.type == ASSIG) || (token.type == KONKAT) || (token.type == SEMICOL)){
        switch(token.type){
            case ASSIG:
                res = expression_check();
        }
    }

    return res;
}
