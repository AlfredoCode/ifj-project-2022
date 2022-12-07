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
#include "generator.h"

#define HTABSIZE 10


int token_res = 0;
char *currentPOP = NULL;

// TODO MAKE IT STATIC BOI
bool insideIf = false;
bool insideFunc = false;
bool insideWhile = false;

//Token
token_t token;

//Statement struct
stat_t *statement;


//Internal variables
p_return currentReturnType;

// Expressions
expression_T *expression, *allTokens;
// expr_El expr; // FOR DEBUG PRINTS

// Symtable
htab_t *symtable, *funTable;
htab_list *sym_list;

// Instruction list for generation
instructList_T *iList;


void expressionInit(expression_T *exprList){
    exprList->firstElement = NULL;
	exprList->activeElement = NULL;		// Nastavení default hodnot seznamu
	exprList->lastElement = NULL;
}

void insertExpr(expression_T *exprList, token_t *token){

    expr_El newElement = (expr_El) malloc(sizeof(expr_El));
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


void exprListDispose( expression_T *exprList ) {
	expr_El firstEl;
	expr_El nextEl;	// Deklarace dvou pomocných prvků typu ListElementPtr
	firstEl = exprList->firstElement;
	while(firstEl != NULL){
		nextEl= firstEl->next; 	// Cyklus, který maže vždy první prvek seznamu, dokud tam nějaký prvek je
        //free(firstEl->token->string);
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
        errHandler(LEX_ERR, "Lexical error\n");
    }
    if(token.type != L_PAR){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }

    token_res = GetToken(&token);   // strict_types
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != ID){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }
    // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU ID


    token_res = GetToken(&token);   // rovná se
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != ASSIG){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }


    token_res = GetToken(&token);   // int
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != INT_T){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }
    // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU INT


    token_res = GetToken(&token);   // pravá závorka
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != R_PAR){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }

    token_res = GetToken(&token);   // středník
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != SEMICOL){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING SEMICOL <---\n");
    }
    return SUCCESS_ERR;
}

/********************************PARSING SECTION********************************************************/

void builtinInit(htab_t *funTable){
    stat_t *newStat = (stat_t*)malloc(sizeof(stat_t));
    if(newStat == NULL){
        exit(INTERNAL_ERR);
    }
    newStat = htab_lookup_add(funTable, "reads");
    newStat->value = "Ss";  // S means it can return nil or string
    newStat = htab_lookup_add(funTable, "readi");
    newStat->value = "Ii";  // I means it can return nil or int
    newStat = htab_lookup_add(funTable, "readf");
    newStat->value = "Ff";  // F means it can return nil or float
    newStat = htab_lookup_add(funTable, "write");   
    newStat->value = "";    // 0 idea how to do infinite params
    newStat = htab_lookup_add(funTable, "strlen");
    newStat->value = "is";  //returns int, takes string
    newStat = htab_lookup_add(funTable, "substring");
    newStat->value = "Ssii";  //returns string or nil, takes string, int, int
    newStat = htab_lookup_add(funTable, "ord");
    newStat->value = "is";  //returns int, takes string
    newStat = htab_lookup_add(funTable, "chr");
    newStat->value = "si";
}

int parse(){

    int res = SUCCESS_ERR;

    expression  = (expression_T*)malloc(sizeof(*expression));
    if(expression == NULL){
        errHandler(INTERNAL_ERR,"Malloc failure\n");
    }
    expressionInit(expression);

    iList  = (instructList_T*)malloc(sizeof(*iList));
    if(iList == NULL){
        errHandler(INTERNAL_ERR,"Malloc failure\n");
    }
    initInstList(iList);

    symtable = htab_init(HTABSIZE); // Var symtable
    if(symtable == NULL){
        return INTERNAL_ERR;
    }
    insertInstruction(iList, DEFVAR_LOC_I, NULL, NULL, "main");
    funTable = htab_init(HTABSIZE); // Function symtable
    if(funTable == NULL){
        return INTERNAL_ERR;
    }

    sym_list  = (htab_list*)malloc(sizeof(*sym_list));
    if(sym_list == NULL){
        errHandler(INTERNAL_ERR,"Malloc failure\n");
    }
    initSymList(sym_list);
    insertSymtable(funTable, sym_list);
    insertSymtable(symtable, sym_list);
    builtinInit(funTable);
    
    statement = (stat_t *)malloc(sizeof(*statement));
    if(statement == NULL){
        return INTERNAL_ERR;
    }

    if(GetProlog()){    // modif was not here before
        res = prog();
        if(res == SUCCESS_ERR){
            generatorInit(iList, sym_list);
        }
        return res;
    }
    token_res = GetToken(&token);   
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }

    switch(token.type){
        case EOF_T:
            errHandler(SYNTAX_ERR,"Syntax error ---> EMPTY FILE <---\n");
            break;
        default:
            errHandler(LEX_ERR,"Syntax error ---> UNKNOWN BEFORE PROLOG <---\n");
    }
    //GENERATE
    return SUCCESS_ERR;
}

int prog(){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);   
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    switch(token.type){
        case ID:
            res = declareCheck();
            if(res == SUCCESS_ERR){
                res = statement_list(symtable);
            }
            return res; 
        default:
            errHandler(SYNTAX_ERR,"Syntax error ---> MISSING DECLARE <---\n");
    }
    errHandler(SYNTAX_ERR,"Syntax error\n");
    return res;
}


int statement_list(htab_t *localTable){
    int res = SYNTAX_ERR;
    
    token_res = GetToken(&token);  
    if(!token_res){
        if(insideIf || insideWhile || insideFunc){
            errHandler(SYNTAX_ERR,"Syntax error ---> EOF inside If statement <---\n");     // DONT ASK ME
        }
        errHandler(LEX_ERR,"Lexical error\n");
    }
    token_t *expr_tok;
    stat_t *funcName;
    switch(token.type){
        case EOF_T: // FOUND ?>     

            free(expression);
            free(allTokens);
            
            return SUCCESS_ERR;
        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            res = expression_check(localTable);

            return res;
        case KEYWORD: 
            //  SEMANTIC - if, function, else, while, return only, otherwise SYNTAX_ERR
            switch(token.keyword){
                case IF:
                    res = condiCheck(localTable);
                    return res;
                case FUNCTION:
                    if(insideFunc){ // Vnořené funkce nechceme
                        errHandler(SYNTAX_ERR,"Correct, we do not want nested functions\n"); //TODO WHICH ERROR IS THIS?
                    }
                    res = functionCheck();
                    return res;
                case WHILE:
                    res = checkWhile(localTable);
                    return res;
                case RETURN:
                    // if(currentReturnType){
                    return SUCCESS_ERR;
                    // }
                    
                default:
                    errHandler(SYNTAX_ERR,"Syntax error\n");
                    return SYNTAX_ERR;
            }
        case ID:
           
            funcName = htab_find(funTable, token.string);   // Was function defined before??
            if(funcName == NULL){
                errHandler(SEM_FUNC_ERR, "Semantic Error ---> FUNCTION NOT DEFINED <---\n");
            }
            char *currFuncName = token.string;
            token_res = GetToken(&token);  
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");
            }
            if(token.type == L_PAR){
                insertInstruction(iList, PUSHS_NIL_I, NULL, NULL, NULL);    // PUSHES NIL TO STACK SO WE KNOW WHERE TO STOP
                res = builtinParams(0);
                if(res != SUCCESS_ERR){
                    errHandler(SYNTAX_ERR,"Syntax error\n");// ADD SEMICOL TO LL ON ITS OWN!!!
                }
                
                token_res = GetToken(&token);  
                // printf("Token type is %d\n", token.type); // DEBUG 
                if(!token_res){
                    errHandler(LEX_ERR,"Lexical error\n");
                }
                if(token.type != SEMICOL){
                    errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
                }
                insertFunctionCall(iList, currFuncName);
                res = statement_list(localTable);
                if(res != SUCCESS_ERR){
                    return res;
                }
                
                return res;  
            }
            errHandler(SYNTAX_ERR, "Syntax error\n");
            break;
        case INT_T: case FLOAT_T: case STRING_T:
            expr_tok = (token_t*) malloc(sizeof(*expr_tok));
            if(expr_tok == NULL){
                return INTERNAL_ERR;
            }
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            p_return *ret_type;
            res = separators(localTable, ret_type);
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
        errHandler(LEX_ERR,"Lexical error\n");
    }
    switch(token.type){
        case R_PAR: // NO PARAM 
            if(multipleParams){
                multipleParams = false;
                return SYNTAX_ERR;
            }
            return SUCCESS_ERR;
        case DOLLAR:
            token_res = GetToken(&token);   // $ <ID>
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
            }
            if(token.type != ID){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER $ BUILTIN <---\n");
            }
            // SEMANTIC
            insertInstruction(iList, PUSHS_ID_I, token.string, NULL, NULL);
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
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
            if(token.type == INT_T){
                insertInstruction(iList, PUSHS_INT_I, token.string, NULL, NULL);
            }
            else if(token.type == STRING_T){
                insertInstruction(iList, PUSHS_STRING_I, token.string, NULL, NULL);
            }
            else{
                insertInstruction(iList, PUSHS_FLOAT_I, token.string, NULL, NULL);
            }
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
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


int checkWhile(htab_t *localTable){
    int res = SYNTAX_ERR;
    char *label = UniqueLabel("WHILE");

    token_res = GetToken(&token);  
    token_t *expr_tok;
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_PAR){                   // Kontrola (
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
    }
    expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    res = checkIfStat(localTable);    // Zkontroluj levou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }
    

    res = checkIfStat(localTable);    // Zkontroluj pravou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }
    
    insertInstruction(iList, LABEL_I, NULL, NULL, label);     // GENERATE ELSE LABEL  
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_BRAC){               // Kontrola {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    expr_parse(localTable, expression, iList, NULL); // modif not poping
    exprListDispose(expression);
    
    insertInstruction(iList, GENERATE_WHILE_I, NULL, NULL, label); // CHECK CONDITION AND JUMP ON AFTER ELSE IF NECESARY
    insideWhile = true;
    res = statement_list(localTable);  // Kontrola vnitřku funkce
    insertInstruction(iList, JUMP_I, NULL, NULL, label);     // GENERATE ELSE LABEL  
    insertInstruction(iList, LABEL_END_I, NULL, NULL, label);     // GENERATE ELSE LABEL  

    if(res != SUCCESS_ERR){
        return res;
    }
    
    insideWhile = false;
    

    return statement_list(localTable);
}


int functionCheck(){
    int res = SYNTAX_ERR;  
    stat_t *statementFun;
    statementFun = (stat_t*) malloc(sizeof(*statementFun));
    if(statementFun == NULL){
        return INTERNAL_ERR;
    }
    htab_t *localTable;
    localTable = htab_init(HTABSIZE); // Function local var symtable
    if(localTable == NULL){
        return INTERNAL_ERR;
    }
    insertSymtable(localTable, sym_list);
    
    token_res = GetToken(&token);   // function <ID>
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != ID){
        errHandler(SYNTAX_ERR, "Syntax error ---> FUNCTION IS MISSING IDENTIFIER <---\n");
    }
    // INSERT ID INTO FUNCTION HTAB
    statementFun = htab_find(funTable, token.string);
    if(statementFun != NULL){
        errHandler(SEM_FUNC_ERR,"SEMANTIC ERROR ---> Function redefinition <---\n");
    }
    char *currentFuncName = token.string;
    insertInstruction(iList, FUNC_S_I, NULL, NULL, token.string);
    insertInstruction(iList, DEFVAR_LOC_I, NULL, NULL, "local");
    statementFun = htab_lookup_add(funTable, token.string);   // add  func identifier to symtable

    statementFun->type = t_fun;
    statementFun->name = token.string;
    // statementFun->value = ??

    stat_t *statement;
    statement = (stat_t*) malloc(sizeof(*statement));
    if(statement == NULL){
        return INTERNAL_ERR;
    }
    // printf("type is %d and name is %s\n", statement->type, statement->name);    // DEBUG
    // -----------------------
    token_res = GetToken(&token);   // function <ID> (
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_PAR){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING LEFT PARENTHESIS IN FUNCTION <---\n");
    }
    res = funcParams(localTable, statement, 0); // Parametry funkce
    if(res != SUCCESS_ERR){
        errHandler(res, "Syntax error ---> Wrong parameters in function <---\n");
    }

    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ):
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != COL){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING COLON IN FUNCTION <---\n");
    }
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }

    if(token.type != KEYWORD){  // OR IT CAN BE TYPE TOKEN
        token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type
        if(!token_res){
            errHandler(LEX_ERR,"Lexical error\n");
        }
        if(token.type != KEYWORD){
            return SYNTAX_ERR;
        }

    }


    switch(token.keyword){   // Co za keyword jsme dostali?
        case STRING: 
            currentReturnType = ret_string;
            statementFun->value = "s";
            break;
        case INT: 
            currentReturnType = ret_int;
            statementFun->value = "i";
            break;
        case FLOAT: 
            currentReturnType = ret_float;
            statementFun->value = "f";
            break;
        case VOID:
            break;
        
            
        default:
            return SYNTAX_ERR;  // while, if apod..
    }
    
    //  int currentReturnType if token.type == TYPE TODO
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type {
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_BRAC){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING LEFT BRACKET IN FUNCTION <---\n");
    }
    insideFunc = true;
    res = statement_list(localTable); // function <ID> ( <FUNC_PARAMS> ): type{ <ST_L>
    if(res != SUCCESS_ERR){
        if(res == SEM_FUNC_ERR){
            return res;
        }
        if(!(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string)){  // VOID FUNCTION WITHOUT RETURN STATEMENT
            insertInstruction(iList, FUNC_E_I, NULL, NULL, currentFuncName);
            return SUCCESS_ERR;
        }
      
        if(token.type != KEYWORD && token.keyword != RETURN){
            return SEM_PARAM_ERR;   // NO RETURN FOUND
        }
        return res;
    }
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type { <ST_L> return <>
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    
    if(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string){
        token_t *expr_tok;
        switch(token.type){
            case DOLLAR:
                token_res = GetToken(&token);   // looking for ID
                if(!token_res){
                    errHandler(LEX_ERR,"Lexical error\n");
                }
                if(token.type != ID){
                    errHandler(SYNTAX_ERR, "Syntax error ---> MISSING RETURN STATEMENT <---\n");   
                }
                expr_tok = (token_t*) malloc(sizeof(*expr_tok));
                if(expr_tok == NULL){
                    return INTERNAL_ERR;
                }
                *expr_tok = token;
                insertExpr(expression, expr_tok);
                token_res = GetToken(&token);   // Looking for semicol;
                if(!token_res){
                    errHandler(LEX_ERR,"Lexical error\n");
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
                    errHandler(LEX_ERR,"Lexical error\n");
                }
                break;
            case KEYWORD:
                if(token.keyword == NULL_K){
                    errHandler(SEM_PARAM_ERR, "null as a return type of function\n");
                }
                else{
                    errHandler(SYNTAX_ERR, "other keyword as a return type of function\n");
                }
                break;
            default:
                return SEM_RETURN_ERR;   // NO RETURN VALUE
        }
    }
    if(token.type != SEMICOL){
        if(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string){
            errHandler(SYNTAX_ERR, "Syntax error ---> MISSING SEMICOL AFTER RETURN <---\n");
        }
        errHandler(SEM_RETURN_ERR, "Syntax error ---> VOID FUNCTION RETURNING VALUE <---\n");
    }
    token_t *expr_tok_semicol;
    expr_tok_semicol = (token_t*) malloc(sizeof(*expr_tok_semicol));
    if(expr_tok_semicol == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok_semicol = token;
    insertExpr(expression, expr_tok_semicol);
    if(currentReturnType == ret_string || currentReturnType == ret_int || currentReturnType == ret_float){

        // statement = htab_find(localTable,statement->name);    // DEBUG
        //     printf("name is %s, value is %s\n",statement->name, statement->value);  
        
        
        if(expr_parse(localTable, expression, iList, NULL) != currentReturnType){   // sending return expression to expr_parser

            fprintf(stderr, "Wrong return type\n");
            return SEM_PARAM_ERR;
        }
        
    }
    exprListDispose(expression);
    token_res = GetToken(&token);   // Looking for R_BRAC;
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != R_BRAC){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING RIGHT BRACKET AFTER RETURN STATEMENT <---\n");
    }
    insertInstruction(iList, FUNC_E_I, NULL, NULL, currentFuncName);
    // printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG
    insideFunc = false;
    return statement_list(symtable);
}

int funcParams(htab_t *localTable, stat_t *statementIn, int index){
    int res = SYNTAX_ERR;
    static bool multipleParams = false;
    index++;
    token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> )
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
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
                    errHandler(LEX_ERR,"Lexical error\n");
                }
                if(token.type != KEYWORD){
                    return SYNTAX_ERR;
                }
            }
            switch(token.keyword){
                case FLOAT: 
                    statement->type = t_float;
                    break;
                case INT: 
                    statement->type = t_int;
                    break;
                case STRING:
                    statement->type = t_str;
                    break;
                default:
                    return SYNTAX_ERR;  // FOUND WHILE, IF ETC..
            }
            
        
            
            
            token_res = GetToken(&token);   // type $
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
            }
            if(token.type != DOLLAR){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED $ <---\n");
            }

            token_res = GetToken(&token);   // type $ <ID>
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
            }
            if(token.type != ID){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER TYPE IN FUCNTION <---\n");
            }
            // SEMANTIC
            if(htab_find(localTable, token.string)){
                errHandler(SEM_PARAM_ERR, "Semantic error PARAMS REDEFINITION\n"); 
            }
            statementIn = htab_lookup_add(localTable, token.string);  // ADD PARAM TO LOCAL VAR TABLE
            // if(statementIn->type != 99){
            //     errHandler(SEM_PARAM_ERR, "Semantic error PARAMS REDEFINITION\n");
            // }
            
            statementIn->type = statement->type;
            // insertInstruction(iList, POPS_I, token.string, NULL, NULL);
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID
            }
            switch(token.type){
                case R_PAR:
                    return SUCCESS_ERR;
                case COMMA:
                    multipleParams = true;
                    return funcParams(localTable, statement, index);
                default:
                    return SYNTAX_ERR;
            }

        default:
            return SYNTAX_ERR;

    }
    return res;
}
int separators_if(htab_t *table){
    int res = SYNTAX_ERR;
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    switch(token.type){ // SEPARATORS
        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = checkIfStat(table);
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

int checkIfStat(htab_t *table){
    int res = SYNTAX_ERR;

    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    switch(token.type){
        
        case DOLLAR:    // $ <ID> <SEPARATOR>  
            token_res = GetToken(&token);  
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");
            }
            if(token.type != ID){
                return SYNTAX_ERR;
            }
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            break;
            

        case INT_T: case FLOAT_T: case STRING_T:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            break;
            
        case KEYWORD:
            if(token.keyword != NULL_K){
                return SYNTAX_ERR;  //TODO WHICH EXIT CODE IS THIS
            }
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            break;
            
        default:
        
            return SYNTAX_ERR;
    }
    res = checkIfOperators();
    if(res == SUCCESS_ERR){
        return SUCCESS_ERR;
    }
    else if(res == LEX_ERR){
        return res;
    }
    
    res = separators_if(table);
    if(res != SUCCESS_ERR){
        return res;
    }
    return SUCCESS_ERR;
}


int checkIfOperators(){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
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

int condiCheck(htab_t *table){
    char * label = UniqueLabel("ELSE");
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_PAR){                   // Kontrola (
        errHandler(res,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    res = checkIfStat(table);    // Zkontroluj levou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }
    res = checkIfStat(table);    // Zkontroluj pravou stranu vyrazu v ifu
    if(res != SUCCESS_ERR){
        return res;
    }

    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_BRAC){               // Kontrola {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    token_t *expr_tok2 = (token_t*) malloc(sizeof(*expr_tok2));
    if(expr_tok2 == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok2 = token;
    insertExpr(expression, expr_tok2);
    // expr = expression->lastElement;          // DEBUG
    // while(expr != NULL){
    //     // printf("%d ",expr->token->type); 
    //     expr = expr->previous;
    // }
    // putchar('\n');
    expr_parse(table, expression, iList, NULL);   // not poping
    // insertInstruction(iList, PUSHS_INT_I, "1", NULL, NULL); // CONDITIONAL JUMP BASED ON STACK VALUE
    // insertInstruction(iList, JUMPIFNEQS_I, NULL, NULL, NULL);
    
    insertInstruction(iList, GENERATE_IF_I, NULL, NULL, label); // CHECK CONDITION AND JUMP ON AFTER ELSE IF NECESARY
    exprListDispose(expression);
    insideIf = true;
    res = statement_list(table);  // Kontrola vnitřku funkce
    if(res != SUCCESS_ERR){
        return res;
    }
    
    insideIf = false;
    insertInstruction(iList, JUMP_END_I, NULL, NULL, label);    // IF NOT TRUE, JUMP ON ELSE
    insertInstruction(iList, LABEL_I, NULL, NULL, label);     // GENERATE ELSE LABEL  
    res = elseCheck(table);
    if(res != SUCCESS_ERR){
        errHandler(res,"Syntax error ---> WRONG ELSE FORMAT <---\n");
    }
    insertInstruction(iList, LABEL_END_I, NULL, NULL, label); // GENERATE LABEL AFTER ELSE
    return statement_list(table);
    
}

int elseCheck(htab_t *localTable){
    int res = SYNTAX_ERR;
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.keyword != ELSE){               // Kontrola tokenu else
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING ELSE <---\n");
    }
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_BRAC){               // Kontrola {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    insideIf = true;
    res = statement_list(localTable); // Kontrola těla else
    if(res != SUCCESS_ERR){
        return res;
    }
    
    return SUCCESS_ERR;
}


int expression_check(htab_t *table){
    int res = SYNTAX_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(!((token.type == ID) || (token.type == KEYWORD))){
        errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
    }
   
    statement = htab_lookup_add(table, token.string);   // add  identifier to symtable
    currentPOP = malloc(strlen(token.string)*sizeof(char));
    strcpy(currentPOP, token.string);
    token_res = GetToken(&token);
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    p_return *ret_type = malloc(sizeof(p_return));
    if(token.type == ASSIG){
        
        // ZAVOLAT EXPRESSION PARSER
        res = statement_list_inside(table, ret_type);
        // insertInstruction(iList, POPS_I, passPOP, NULL, NULL);
        statement->type = *ret_type;
        return res;  
    }
    // TODO $x; UNDEF VAR
    
    return res;
}





int expression_check_inside(htab_t *table){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    p_return *ret_type;
    switch(token.type){
        case ID:
        case INT_T:
        case FLOAT_T:
        case STRING_T:
            res = separators(table, ret_type);
            return res;
        default:
            return SYNTAX_ERR;
    }
    return res;
}


int statement_list_inside(htab_t *table, p_return *ret_type){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);  
    // printf("Token type is %d\n", token.type); // DEBUG 
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    stat_t *funcName;
    switch(token.type){
        
        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            token_res = GetToken(&token);  
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");
            }
            if(token.type != ID){
                return SYNTAX_ERR;
            }
            // statement->value = token.string;    // value of statement of variable   TODO
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            // printf("%s is last, %s is first\n",expression->lastElement->token->string, expression->firstElement->token->string);    // DEBUG
            res = separators(table, ret_type);
            
                
            
            return res;
        case L_PAR:
        
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            return statement_list_inside(table, ret_type);
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
            res = separators(table, ret_type);
            return res;
        case KEYWORD:
            if(token.keyword != NULL_K){
                return SYNTAX_ERR;  // TODO WHICH ERR IS THIS?
            }
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = separators(table, ret_type);
            return res;
        case ID:    
            funcName = htab_find(funTable, token.string);   // Was function defined before??
            if(funcName == NULL){
                errHandler(SEM_FUNC_ERR, "Semantic Error ---> FUNCTION NOT DEFINED <---\n");
            }
            char *currFuncName = token.string;
            token_res = GetToken(&token);  
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");
            }

            if(token.type == L_PAR){
                insertInstruction(iList, PUSHS_NIL_I, NULL, NULL, NULL);    // PUSHES NIL TO STACK SO WE KNOW WHERE TO STOP
                res = builtinParams();
                if(res != SUCCESS_ERR){
                    errHandler(SYNTAX_ERR, "Syntax error\n"); // ADD SEMICOL TO LL ON ITS OWN!!!
                }
                token_res = GetToken(&token);  
                // printf("Token type is %d\n", token.type); // DEBUG 
                if(!token_res){
                    errHandler(LEX_ERR,"Lexical error\n");
                }
                if(token.type != SEMICOL){
                    errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER <---\n");
                }
                char *passPOP;
                if(currentPOP){
                    passPOP = malloc(sizeof(char)* strlen(currentPOP));
                    strcpy(passPOP, currentPOP);
                }
                else{
                    passPOP = NULL;
                }
                insertFunctionCall(iList, currFuncName);
                insertInstruction(iList, POPS_I, passPOP, NULL, NULL);
                insertInstruction(iList, CLEARS_I, NULL, NULL, NULL);
                res = statement_list(table);
                if(res != SUCCESS_ERR){
                    return res;
                }
                return res;  
            }
            errHandler(SYNTAX_ERR, "Syntax error\n");
            break;
        default:
            errHandler(SYNTAX_ERR, "Syntax error\n");
                
    }
    // How did you get there?
    return res;
}

int separators(htab_t *table, p_return *ret_type){
    int res = SUCCESS_ERR;

    token_res = GetToken(&token);
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        errHandler(INTERNAL_ERR,"Internal error\n");
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
            char *passPOP;
            if(currentPOP){
            passPOP = malloc(sizeof(char)* strlen(currentPOP));
            strcpy(passPOP, currentPOP);
            }
            else{
                passPOP = NULL;
            }
            *ret_type = expr_parse(table, expression, iList, passPOP);
            // SEMANTIC CHECK - IS EXPRESSION SEMANTICALLY CORRECT? for example $x = 5.5.5.5;
                     // expr_parse(expression_list, symtable);
            // EXPRESSION LIST DISPOSE

             
            // statement = htab_find(symtable,statement->name);    // DEBUG
            // printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG
            free(currentPOP);
            exprListDispose(expression);
            return statement_list(table); // $x=$y; || $x=5; || $x = $y.$z;
        case R_PAR:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            return separators(table, ret_type);
        case KONKAT: case DIV: case ADD: case SUB: case MUL:    // $x=$y.<IFSTAT> || $x=$y+<IFSTAT> etc.
            
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = statement_list_inside(table, ret_type);
            return res;
        default:
            errHandler(SYNTAX_ERR, "Syntax Error\n");
        
    }

    return res;
}


/**TODO LIST
 * 
 * FREE ALL TOKENS AFTER THE COMPILATION IS DONE
 * SEMANTIC ACTIONS
 * DOPSAT EPSILON DO SEPARATORU V LL
 * VOLÁNÍ FUNKCÍ V LL
 * while(null) if(null) atd
 * what if $x = func() what type is $x? it is not t_str, t_int etc
 * errHandler for returns
 * struct na ukládání symtables, kde bude název fce, ke které patří
 * pops instruction after expr_parse
 * How to generate inside of function before main??
 * CURRENT_POP could be deleted
 * 
 * podminka
 * telo ifu
 * jump afterelse
 * label else
 * telo else
 * label afterelse
 * 
 * 
 * 
 * 
 * pop vršek stacku 
 * na vršek stacku zavolat type
 * case přepsat do if else
 * case type
 * int, float, string, bool
 * int --> srovnat s 0 pokud false
 * float ---> srovna s 0.0 pokud false
 * string --> srovnat s "" pokud false
 * bool --> srovnat s bool@false
 * nil --> false
 * 
 */





