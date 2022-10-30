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

void insertToken(token_T *tokenList, int type){
    token_El *token = (token_El *)malloc(sizeof(*token));
    if(token == NULL){
        fprintf(stderr,"Memmory allocation failure\n");
        return;
    }
    token->type = type;  
    token->next = tokenList->firstElement;
    token->previous = NULL;
    if(tokenList->lastElement != NULL){
       tokenList->firstElement->previous = token;  
    }
    else{
        tokenList->lastElement = token;
        tokenList->activeElement = token;
    } 
    tokenList->firstElement = token;   
}

int getToken(token_T *tokenList){
    // We will not lose any token if we change activeElement only
    token_El *token = tokenList->activeElement;
    tokenList->activeElement = token->previous;
    return token->type;
}

int prologCheck(){
    
    return 0;
}


int main(){
    
    init(&tokenList);
    
    int token;


    // SIMULATION OF TOKEN LIST
    insertToken(&tokenList, DOLLAR);
    insertToken(&tokenList, ID);
    insertToken(&tokenList, EQ);
    insertToken(&tokenList, INT);
    insertToken(&tokenList, COMMA);
    insertToken(&tokenList, TOK_EOF);

    token = getToken(&tokenList); // GETING THE FIRST TOKEN

    switch(token){

    }











    // printf("----------------------------------------\n");

    // RESET the activeElement


    tokenList.activeElement = tokenList.lastElement;
    // while((token = getToken(&tokenList)) != TOK_EOF){
    //     printf("Token Type %d\n", token);
    // }
   
    
    

    


    return 0;
}


