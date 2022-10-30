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


token_T tokenList;


void init(token_T *tokenList){

    tokenList->firstElement = NULL;
    tokenList->activeElement = NULL;
    tokenList->lastElement = NULL;
    
}

void insertToken(token_T *tokenList, int type, void *attrib){
    token_El *token = (token_El *)malloc(sizeof(*token));
    if(token == NULL){
        fprintf(stderr,"Memmory allocation failure\n");
        return;
    }
    token->type = type;  
    token->next = tokenList->firstElement;

    
    
    token->attribute = attrib;

    // THIS SHIT IS CURSED
    // printf("INSIDE Token type: %d with data: %s on address %p\n", token->type, *(char **)&token->attribute, token->attribute);
    
    
    token->previous = NULL;
    if(tokenList->lastElement != NULL){
       tokenList->firstElement->previous = token;  
    }
    else{
        tokenList->lastElement = token;
        
    } 
    tokenList->activeElement = token;
    tokenList->firstElement = token;   
    
}

token_El *getToken(token_T *tokenList){
    // We will not lose any token if we change activeElement only
    token_El *token = tokenList->activeElement;
    if(token->previous != NULL){
        tokenList->activeElement = token->previous;
    }
    
    return token;
}

int prologCheck(){
    
    return 0;
}


int main(){
    
    init(&tokenList);
    
    token_El *token;


    // LEXER
    // For int
        int val = 5;
    //For string
        char *str = "SuperID";

        char* empty = "";

    // SIMULATION OF TOKEN LIST
    insertToken(&tokenList, DOLLAR, &empty);
    insertToken(&tokenList, ID, &str); 
    insertToken(&tokenList, EQ, &empty);    
    insertToken(&tokenList, INT, &val);
    insertToken(&tokenList, COMMA, &empty);
    insertToken(&tokenList, TOK_EOF, &empty);

    token = getToken(&tokenList); // GETING THE FIRST TOKEN












    // printf("----------------------------------------\n");

    // RESET the activeElement


    tokenList.activeElement = tokenList.lastElement;
    while((token = getToken(&tokenList))){

        if(token->type == TOK_EOF){
            break;
        }
        
        switch(token->type){
            case ID: case DOLLAR: case EQ: case COMMA:
                printf("Token type: %d\t Data: %s Memory %p\n", token->type, *(char**)token->attribute, token->attribute);
                break;
            case INT:
                
                printf("Token type: %d\t Data: %d Memory %p\n", token->type, *(int*)token->attribute, token->attribute);
                break;

            default:
                break;
        }

    }
   
    
    

    


    return 0;
}


