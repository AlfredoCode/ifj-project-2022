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
#include "symtable.h"
#include "error.h"
#include "expr_parser.h"

#define HTABSIZE 10


int token_res = 0;

// TODO MAKE IT STATIC BOI
bool insideIf = false;
bool insideFunc = false;
bool insideWhile = false;

//Token
token_t token;

//Statement struct
stat_t *statement;

//Internal variables
int currentToken = 0;
p_return currentReturnType = 0;
// Expressions
expression_T *expression, *allTokens;
expr_El expr; // FOR DEBUG PRINTS

// Symtable
htab_t *symtable, *funTable;


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

expr_El getExpr(expression_T *exprList){
    if(exprList->activeElement == NULL){
        exprList->activeElement = exprList->lastElement;
        return exprList->activeElement; 
    }
    exprList->activeElement = exprList->activeElement->previous;
    return exprList->activeElement;
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
        // printf("SUP");

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
    if(expression == NULL){
        fprintf(stderr,"Malloc failure\n");
        return INTERNAL_ERR;
    }
    expressionInit(expression);

    symtable = htab_init(HTABSIZE); // Var symtable
    if(symtable == NULL){
        return INTERNAL_ERR;
    }
    funTable = htab_init(HTABSIZE); // Function symtable
    if(funTable == NULL){
        return INTERNAL_ERR;
    }
    htab_lookup_add(funTable, "reads");
    htab_lookup_add(funTable, "readi");
    htab_lookup_add(funTable, "readf");
    htab_lookup_add(funTable, "write");
    htab_lookup_add(funTable, "strlen");
    htab_lookup_add(funTable, "substring");
    htab_lookup_add(funTable, "ord");
    htab_lookup_add(funTable, "chr");
    statement = (stat_t *)malloc(sizeof(*statement));
    if(statement == NULL){
        return INTERNAL_ERR;
    }

    token_res = GetToken(&token);   
    if(!token_res){
        // printf("AHoj\n");
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
    //GENERATE
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
        if(insideIf || insideWhile || insideFunc){
            fprintf(stderr,"Syntax error ---> EOF inside If statement <---\n");     // DONT ASK ME
            return SYNTAX_ERR;
        }
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    token_t *expr_tok;
    switch(token.type){
        case EOF_T: // FOUND ?>     
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
                case FUNCTION:
                    if(insideFunc){ // Vnořené funkce nechceme
                        fprintf(stderr,"Correct, we do not want nested functions\n"); //TODO WHICH ERROR IS THIS?
                        return SYNTAX_ERR;
                    }
                    res = functionCheck();
                    return res;
                case WHILE:
                    res = checkWhile();
                    return res;
                case RETURN:
                    // if(currentReturnType){
                    return SUCCESS_ERR;
                    // }
                    
                    return SYNTAX_ERR;  // Void function cannot have return
                default:
                    return SYNTAX_ERR;
            }
        case ID:
            
            token_res = GetToken(&token);  
            if(!token_res){
                fprintf(stderr,"Lexical error\n");
                return LEX_ERR;
            }
            if(token.type == L_PAR){
                res = builtinParams();
                if(res != SUCCESS_ERR){
                     
                    return SYNTAX_ERR;  // ADD SEMICOL TO LL ON ITS OWN!!!
                }
                
                // semicol();
                token_res = GetToken(&token);  
                // printf("Token type is %d\n", token.type); // DEBUG 
                if(!token_res){
                    fprintf(stderr,"Lexical error\n");
                    return LEX_ERR;
                }
                if(token.type != SEMICOL){
                    fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
                    return SYNTAX_ERR;
                }
                res = statement_list();
                if(res != SUCCESS_ERR){
                    return res;
                }
                
                return res;  
            }
            return SYNTAX_ERR;
        case INT_T: case FLOAT_T: case STRING_T:
            expr_tok = (token_t*) malloc(sizeof(*expr_tok));
            if(expr_tok == NULL){
                return INTERNAL_ERR;
            }
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = separators();
            return res;
        default:
            if((insideIf && token.type == R_BRAC) || (insideWhile && token.type == R_BRAC)){
                return SUCCESS_ERR;
            }
            return SYNTAX_ERR;

    }


    return res;
}

int builtinParams(){
    int res = SYNTAX_ERR;
    static bool multipleParams = false;
    token_res = GetToken(&token);   // bar( <PARAMS> )
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    switch(token.type){
        case R_PAR: // NO PARAM 
            if(multipleParams){
                multipleParams = false;
                return SYNTAX_ERR;
            }
            return SUCCESS_ERR;
        case DOLLAR:
          
            token_res = GetToken(&token);   // type $
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            if(token.type != DOLLAR){
                fprintf(stderr, "Syntax error ---> EXPECTED $ <---\n");
                return SYNTAX_ERR;
            }

            token_res = GetToken(&token);   // type $ <ID>
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            if(token.type != ID){
                fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER AFTER $ BUILTIN <---\n");
                return SYNTAX_ERR;
            }
            // SEMANTIC
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            switch(token.type){
                case R_PAR:
                    return SUCCESS_ERR;
                case COMMA:
                    multipleParams = true;
                    return builtinParams();
                default:
                    return SYNTAX_ERR;
            }
        case INT_T: case STRING_T: case FLOAT_T:
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            switch(token.type){
                case R_PAR:
                    return SUCCESS_ERR;
                case COMMA:
                    multipleParams = true;
                    return builtinParams();
                default:
                    return SYNTAX_ERR;
            }
        default:
            return SYNTAX_ERR;

    }
    return res;
}


int checkWhile(){
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
    insideWhile = true;
    res = statement_list();  // Kontrola vnitřku funkce
    if(res != SUCCESS_ERR){
        return res;
    }
    
    insideWhile = false;
    

    return statement_list();
}


int functionCheck(){
    int res = SYNTAX_ERR;  
    token_res = GetToken(&token);   // function <ID>
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != ID){
        fprintf(stderr, "Syntax error ---> FUNCTION IS MISSING IDENTIFIER <---\n");
        return res;
    }
    // INSERT ID INTO FUNCTION HTAB
    statement = htab_find(funTable, token.string);
    if(statement != NULL){
        fprintf(stderr,"SEMANTIC ERROR ---> Function redefinition <---\n");
        return SEM_FUNC_ERR;
    }
    statement = htab_lookup_add(funTable, token.string);   // add  func identifier to symtable

    stat_t *statement;
    statement = (stat_t*) malloc(sizeof(*statement));
    if(statement == NULL){
        return INTERNAL_ERR;
    }
    statement->type = t_fun;
    statement->name = token.string;
    // printf("type is %d and name is %s\n", statement->type, statement->name);    // DEBUG
    // -----------------------
    token_res = GetToken(&token);   // function <ID> (
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_PAR){
        fprintf(stderr, "Syntax error ---> MISSING LEFT PARENTHESIS IN FUNCTION <---\n");
        return SYNTAX_ERR;
    }
    res = funcParams(); // Parametry funkce
    if(res != SUCCESS_ERR){
        fprintf(stderr, "Syntax error ---> Wrong parameters in function <---\n");
        return res;
    }

    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ):
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != COL){
        fprintf(stderr, "Syntax error ---> MISSING COLON IN FUNCTION <---\n");
        return SYNTAX_ERR;
    }
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }

    if(token.type != KEYWORD){  // OR IT CAN BE TYPE TOKEN
        token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type
        if(!token_res){
            fprintf(stderr,"Lexical error\n");
            return LEX_ERR;
        }
        if(token.type != KEYWORD){
            return SYNTAX_ERR;
        }

    }


    switch(token.keyword){   // Co za keyword jsme dostali?
        case STRING: 
            currentReturnType = ret_string;
            break;
        case INT: 
            currentReturnType = ret_int;
            break;
        case FLOAT: 
            currentReturnType = ret_float;
            break;
        case VOID:
            break;
        
            
        default:
            return SYNTAX_ERR;  // while, if apod..
    }
    
    //  int currentReturnType if token.type == TYPE TODO
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type {
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_BRAC){
        fprintf(stderr, "Syntax error ---> MISSING LEFT BRACKET IN FUNCTION <---\n");
        return SYNTAX_ERR;
    }
    insideFunc = true;
    res = statement_list(); // function <ID> ( <FUNC_PARAMS> ): type{ <ST_L>
    if(res != SUCCESS_ERR){
        return SYNTAX_ERR;
    }
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type {
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string){
        token_t *expr_tok;
        switch(token.type){
            case DOLLAR:
                token_res = GetToken(&token);   // looking for ID
                if(!token_res){
                    fprintf(stderr,"Lexical error\n");
                    return LEX_ERR;
                }
                if(token.type != ID){
                    fprintf(stderr, "Syntax error ---> MISSING RETURN STATEMENT <---\n");
                    return SYNTAX_ERR;
                }
                token_res = GetToken(&token);   // Looking for semicol;
                if(!token_res){
                    fprintf(stderr,"Lexical error\n");
                    return LEX_ERR;
                }
                break;
            case INT_T: case FLOAT_T: case STRING_T:    // Teoreticky muzu vratit 5+5 apod, ale to ted neresme
                expr_tok = (token_t*) malloc(sizeof(*expr_tok));
                if(expr_tok == NULL){
                    return INTERNAL_ERR;
                }
                *expr_tok = token;
                insertExpr(expression, expr_tok);
                token_res = GetToken(&token);   // Looking for semicol;
                if(!token_res){
                    fprintf(stderr,"Lexical error\n");
                    return LEX_ERR;
                }
                break;
            case KEYWORD:
                if(token.keyword == NULL_K){
                    fprintf(stderr, "null as a return type of function\n");
                    return SEM_PARAM_ERR;
                }
                else{
                    fprintf(stderr, "other keyword as a return type of function\n");
                    return SYNTAX_ERR;  
                }
            default:
                return SYNTAX_ERR;
        }
    }
    if(token.type != SEMICOL){
        fprintf(stderr, "Syntax error ---> MISSING SEMICOL AFTER RETURN <---\n");
        return SYNTAX_ERR;
    }
    token_t *expr_tok_semicol;
    expr_tok_semicol = (token_t*) malloc(sizeof(*expr_tok_semicol));
    if(expr_tok_semicol == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok_semicol = token;
    insertExpr(expression, expr_tok_semicol);
    if(currentReturnType == ret_string || currentReturnType == ret_int || currentReturnType == ret_float){
        if(expr_parse(symtable, expression) != currentReturnType){   // sending return expression to expr_parser
            fprintf(stderr, "Wrong return type\n");
            return SEM_PARAM_ERR;
        }
        exprListDispose(expression);
    }
    token_res = GetToken(&token);   // Looking for R_BRAC;
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != R_BRAC){
        fprintf(stderr, "Syntax error ---> MISSING RIGHT BRACKET AFTER RETURN STATEMENT <---\n");
        return SYNTAX_ERR;
    }
    // printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG
    insideFunc = false;
    return statement_list();
}

int funcParams(){
    int res = SYNTAX_ERR;
    static bool multipleParams = false;
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> )
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    switch(token.type){
        case R_PAR: // NO PARAM 
            if(multipleParams){
                multipleParams = false;
                return SYNTAX_ERR;
            }
            return SUCCESS_ERR;
        case TYPE: case KEYWORD:
            if(token.type == TYPE){
                token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type
                if(!token_res){
                    fprintf(stderr,"Lexical error\n");
                    return LEX_ERR;
                }
                if(token.type != KEYWORD){
                    return SYNTAX_ERR;
                }
            }
            switch(token.keyword){
                case FLOAT: case INT: case STRING:
                    break;
                default:
                    return SYNTAX_ERR;  // FOUND WHILE, IF ETC..
            }
            
        
            
            
            token_res = GetToken(&token);   // type $
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            if(token.type != DOLLAR){
                fprintf(stderr, "Syntax error ---> EXPECTED $ <---\n");
                return SYNTAX_ERR;
            }

            token_res = GetToken(&token);   // type $ <ID>
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            if(token.type != ID){
                fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER AFTER TYPE IN FUCNTION <---\n");
                return SYNTAX_ERR;
            }
            // SEMANTIC
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                fprintf(stderr,"Lexical error\n");  // Hledáme ID
                return LEX_ERR;
            }
            switch(token.type){
                case R_PAR:
                    return SUCCESS_ERR;
                case COMMA:
                    multipleParams = true;
                    return funcParams();
                default:
                    return SYNTAX_ERR;
            }

        default:
            return SYNTAX_ERR;

    }
    return res;
}
int separators_if(){
    int res = SYNTAX_ERR;
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    switch(token.type){ // SEPARATORS
        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = checkIfStat();
            return res;
        case R_PAR:
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            
            return SUCCESS_ERR;
        default:
            return SYNTAX_ERR;
    }

    return res;
}

int checkIfStat(){
    int res = SYNTAX_ERR;

    token_res = GetToken(&token);  
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
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
            
            if(res != SUCCESS_ERR){
                // printf("Found the triple EQ\n");
                return res;
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
            else if(res == LEX_ERR){
                return res;
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
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    switch(token.type){
        case EQ: 
        case NOT_EQ:
        case LESS:
        case LESS_EQ:
        case GREAT:
        case GREAT_EQ:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            return SUCCESS_ERR;
        default:
            return res;
    }

}

int condiCheck(){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    if(!token_res){
        fprintf(stderr,"Lexical error\n");
        return LEX_ERR;
    }
    if(token.type != L_PAR){                   // Kontrola (
        fprintf(stderr,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
        return res;
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
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
    token_t *expr_tok2 = (token_t*) malloc(sizeof(*expr_tok2));
    if(expr_tok2 == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok2 = token;
    insertExpr(expression, expr_tok2);
    expr = expression->lastElement;          // DEBUG
    while(expr != NULL){
        // printf("%d ",expr->token->type); 
        expr = expr->previous;
    }
    putchar('\n');
    expr_parse(symtable, expression);
    
    exprListDispose(expression);
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

    return statement_list();
    
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
    if(!((token.type == ID) || (token.type == KEYWORD))){
        fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
        return SYNTAX_ERR;
    }

   
    statement = htab_lookup_add(symtable, token.string);   // add  identifier to symtable


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
            // statement->value = token.string;    // value of statement of variable   TODO
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            // printf("%s is last, %s is first\n",expression->lastElement->token->string, expression->firstElement->token->string);    // DEBUG
            res = separators();
            
                
            
            return res;
        case INT_T: 
        case FLOAT_T: 
        case STRING_T:
            // statement->value = token.string;    // value of statement of number // TODO
            if(token.type == INT_T){
                statement->type = t_int;
            }
            else if(token.type == STRING_T){
                statement->type = t_str;

            }
            else{
                statement->type = t_float;

            }
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
        return INTERNAL_ERR;
    }
    switch(token.type){ // SEPARATORS

        case SEMICOL:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            // expr = expression->lastElement;          // DEBUG
            // while(expr != NULL){
            //     // printf("%s ",expr->token->string); 
            //     expr = expr->previous;
            // }
            // putchar('\n');
            expr_parse(symtable, expression);
        
            // SEMANTIC CHECK - IS EXPRESSION SEMANTICALLY CORRECT? for example $x = 5.5.5.5;
                     // expr_parse(expression_list, symtable);
            // EXPRESSION LIST DISPOSE

             
            // statement = htab_find(symtable,statement->name);    // DEBUG
            // printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG

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
 * SEMANTIC ACTIONS
 * DOPSAT EPSILON DO SEPARATORU V LL
 * INICIALIZACE HTAB
 * PRIDAVAT RETURN VYRAZY DO LISTU A POSILAT EXPR PARSERU
 * VOLÁNÍ FUNKCÍ V LL
 */





