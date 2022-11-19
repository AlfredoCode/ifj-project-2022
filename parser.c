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
#include "scanner.h"
#include "parser.h"

#include "error.h"

int token_res = 0;
token_t token;
int currentToken = 0;
expression_T *expression;

expr_El *expr; // FOR DEBUG PRINTS


void expressionInit(expression_T *exprList){
    exprList->firstElement = NULL;
	exprList->activeElement = NULL;		// Nastavení default hodnot seznamu
	exprList->lastElement = NULL;
}

void insertExpr(expression_T *exprList, token_t *token){

    expr_El newElement = (expr_El ) malloc(sizeof(newElement));
	if(newElement == NULL){
		return; // return 99
	}
	newElement->token = token;
	newElement->next = exprList->firstElement;	// Nastavení dat nového prvku seznamu
	newElement->previous = NULL;
	if(exprList->lastElement != NULL){
		exprList->firstElement->previous = newElement;	

	}
	else{
		exprList->lastElement = newElement;		// Navázání prvku do seznamu
	}
	
	exprList->firstElement = newElement;	

}

expr_El *getExpr(expression_T *exprList){
    // We will not lose any token if we change activeElement only
    expr_El expr = exprList->activeElement;
    if(expr->previous != NULL){
        exprList->activeElement = expr->previous;
    }
    
    return expr;
}


// -------------------------------------------------------------------------------- //



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
    expression  = (expression_T*)malloc(sizeof(*expression));
    expressionInit(expression);
    token_res = GetToken(&token);   
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }

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

int prog(){
    int res;
    token_res = GetToken(&token);   
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
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
    int res = SYNTAX_ERR;
    
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
    int res = SYNTAX_ERR;

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
    if(token.type == ASSIG){
        // ZAVOLAT EXPRESSION PARSER
        res = statement_list_inside();

        return res;  
    }


    return res;
}





int expression_check_inside(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    switch(token.type){
        case ID:
        case INT_T:
        case FLOAT_T:
        case STRING_T:
            
            res = separators();
            
            return res;
        default:
            return SYNTAX_ERR;
    }


    return res;
}


int statement_list_inside(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    
    switch(token.type){
        
        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            token_res = GetToken(&token);  
            if(!token_res){
                fprintf(stderr,"Lexical error\n");
                return LEX_ERR;
            
            }
            if(token.type != ID){
                return SYNTAX_ERR;
            }

            insertExpr(expression, &token);
            printf("%s is last, %s is first\n",expression->lastElement->token->string, expression->firstElement->token->string);    // DEBUG
            res = separators();
            
                
            
            return res;
        case INT_T: case FLOAT_T: case STRING_T:
            
            res = separators();
            return res;
        
        default:
            return SYNTAX_ERR;

    }


    return res;
}

int separators(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    switch(token.type){ // SEPARATORS

        case SEMICOL:

           

        
        
            // SEMANTIC CHECK - IS EXPRESSION SEMANTICALLY CORRECT? for example $x = 5.5.5.5;
            // EXPRESSION LIST DISPOSE
            return statement_list(); // $x=$y; || $x=5; || $x = $y.$z;
        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            
            res = statement_list_inside();
            return res;
        default:
            return SYNTAX_ERR;
        
    }

    return res;
}


