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
bool insideIf = false;
token_t token;
int currentToken = 0;
expression_T *expression, *allTokens;

expr_El expr; // FOR DEBUG PRINTS


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
    // We will not lose any token if we change activeElement only | CURRENTLY NOT IN USE
    expr_El expr = exprList->activeElement;
    if(expr->previous != NULL){
        exprList->activeElement = expr->previous;
    }
    
    return expr;
}


void exprListDispose( expression_T *exprList ) {
	expr_El firstEl;
	expr_El nextEl;	// Deklarace dvou pomocných prvků typu ListElementPtr
	firstEl = exprList->firstElement;
	while(firstEl != NULL){
		nextEl= firstEl->next; 	// Cyklus, který maže vždy první prvek seznamu, dokud tam nějaký prvek je
        free(firstEl->token->string);
        free(firstEl->token);
		free(firstEl);
		firstEl = nextEl;				// Rozhodně by to šlo vyřešit i jinak, nicméně tato možnost se mi jevila jako nejpříjemnější
	}
	exprList->activeElement = NULL;
	exprList->firstElement = NULL;	// Uvedení seznamu do init stavu
	exprList->lastElement = NULL;
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
            return SYNTAX_ERR;  // MISSING PROLOG
        
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
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    
    switch(token.type){
        case EOF_T: // FOUND ?>       TODO, it is possible to do if(...){ ?> which is not valid
            // printf("Found the end\n");   //DEBUG

            free(expression);
            free(allTokens);
   
            return SUCCESS_ERR;
        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            res = expression_check();

            return res;
        case KEYWORD: 
            //  SEMANTIC - if, function, else, while, return only, otherwise SYNTAX_ERR
            switch(token.keyword){
                case IF:
                    res = condiCheck();
                    return res;
                default:
                    return SYNTAX_ERR;
            }

        default:
            if(insideIf && token.type == R_BRAC){
                return SUCCESS_ERR;
            }
            return SYNTAX_ERR;

    }


    return res;
}

int separators_if(){
    int res = SYNTAX_ERR;

   
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return 99;
    }
    switch(token.type){ // SEPARATORS

        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = checkIfStat();
            return res;
        case R_PAR:
             *expr_tok = token;
            insertExpr(expression, expr_tok);   // POTENTIAL REMOVE
            return SUCCESS_ERR;
        default:
            return SYNTAX_ERR;
    }

    return res;
}

int checkIfStat(){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return 99;
    }
    switch(token.type){
        
        case DOLLAR:    // $ <ID> <SEPARATOR>  
            token_res = GetToken(&token);  
            if(!token_res){
                fprintf(stderr,"Lexical error\n");
                return LEX_ERR;
            
            }
            if(token.type != ID){
                return SYNTAX_ERR;
            }
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = checkIfOperators();
            if(res == SUCCESS_ERR){
                // printf("Found the triple EQ\n");
                return SUCCESS_ERR;
            }
            
            res = separators_if();
            if(res != SUCCESS_ERR){
                return res;
            }
            return SUCCESS_ERR;

        case INT_T: case FLOAT_T: case STRING_T:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = checkIfOperators();
            if(res == SUCCESS_ERR){
                // printf("Found the triple EQ\n");
                return SUCCESS_ERR;
            }
            res = separators_if();
            if(res != SUCCESS_ERR){
                return res;
            }
            return SUCCESS_ERR;
        
        default:
            return SYNTAX_ERR;

    }

    return res;
}


int checkIfOperators(){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }

    switch(token.type){
        case EQ: 
        case NOT_EQ:
        case LESS:
        case LESS_EQ:
        case GREAT:
        case GREAT_EQ:
            return SUCCESS_ERR;
        default:
            return res;
    }

}

int condiCheck(){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_PAR){                   // Kontrola (
        fprintf(stderr,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
        return res;
    }
    res = checkIfStat();    // Zkontroluj levou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }
    

    res = checkIfStat();    // Zkontroluj pravou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }


    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_BRAC){               // Kontrola {
        fprintf(stderr,"Syntax error ---> MISSING LEFT BRACKET <---\n");
        return SYNTAX_ERR;
    }
    insideIf = true;
    res = statement_list();  // Kontrola vnitřku funkce
    if(res != SUCCESS_ERR){
        return res;
    }
    
    insideIf = false;
    res = elseCheck();
    if(res != SUCCESS_ERR){
        fprintf(stderr,"Syntax error ---> WRONG ELSE FORMAT <---\n");
        return res;
    }

    return res;
    
}

int elseCheck(){
    int res = SYNTAX_ERR;

    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.keyword != ELSE){               // Kontrola tokenu else
        fprintf(stderr,"Syntax error ---> MISSING ELSE <---\n");
        return SYNTAX_ERR;
    }

    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_BRAC){               // Kontrola {
        fprintf(stderr,"Syntax error ---> MISSING LEFT BRACKET <---\n");
        return SYNTAX_ERR;
    }
    insideIf = true;
    res = statement_list(); // Kontrola těla else
    if(res != SUCCESS_ERR){
        return res;
    }
    
    return SUCCESS_ERR;
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
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return 99;
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
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            // printf("%s is last, %s is first\n",expression->lastElement->token->string, expression->firstElement->token->string);    // DEBUG
            res = separators();
            
                
            
            return res;
        case INT_T: case FLOAT_T: case STRING_T:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
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
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return 99;
    }
    switch(token.type){ // SEPARATORS

        case SEMICOL:

            // expr = expression->lastElement;          // DEBUG
            // while(expr != NULL){
            //     printf("%s",expr->token->string); 
            //     expr = expr->previous;
            // }
            // putchar('\n');
        
        
            // SEMANTIC CHECK - IS EXPRESSION SEMANTICALLY CORRECT? for example $x = 5.5.5.5;
            // EXPRESSION LIST DISPOSE
            exprListDispose(expression);
            return statement_list(); // $x=$y; || $x=5; || $x = $y.$z;
        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = statement_list_inside();
            return res;
        default:
            return SYNTAX_ERR;
        
    }

    return res;
}


/**TODO LIST
 * 
 * FREE ALL TOKENS AFTER THE COMPILATION IS DONE
 * FUNCTION SYNTAX CHEK
 * WHILE SYNTAX CHECK
 * SEMANTIC ACTIONS
 * DOPSAT EPSILON DO SEPARATORU V LL
 */





