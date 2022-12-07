/*
 * =================================================== *
 * Name:       parser.c                                *
 * Authors:    xhofma11                                * 
 * Last modif: 12/07/2022                              *
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
#include "expr_list.h"
#include "generator.h"

#define HTABSIZE 10


int token_res = 0;
char *currentPOP = NULL;

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

// Symtable
htab_t *symtable, *funTable;
htab_list *sym_list;

// Instruction list for generation
instructList_T *iList;


int declareCheck(){
    
    token_res = GetToken(&token);       // LEFT PARENTHESIS
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
    

    token_res = GetToken(&token);   // EQUALS
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

    token_res = GetToken(&token);   // RIGHT PARENTHESIS
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != R_PAR){
        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");
    }

    token_res = GetToken(&token);   // SEMICOL
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
    newStat = htab_lookup_add(funTable, "floatval");
    newStat->value = "fi";
    newStat = htab_lookup_add(funTable, "intval");
    newStat->value = "if";
    newStat = htab_lookup_add(funTable, "strval");
    newStat->value = "ss";
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
        errHandler(INTERNAL_ERR,"Malloc failure\n");
    }

    if(GetProlog()){    // CHECKS PROLOG 
        res = prog();
        if(res == SUCCESS_ERR){
            generatorInit(iList, sym_list); // GENERATE
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
            errHandler(SYNTAX_ERR,"Syntax error ---> EOF inside If statement <---\n");    
        }
        errHandler(LEX_ERR,"Lexical error\n");
    }
    token_t *expr_tok;
    stat_t *funcName;
    switch(token.type){
        case EOF_T: // FOUND ?>     
            free(allTokens);
            return SUCCESS_ERR;

        case DOLLAR:    // $ <ID> <SEPARATOR_PICK>  
            res = expression_check(localTable);
            if(res != SUCCESS_ERR){
                return res;
            }
            return statement_list(localTable);

        case KEYWORD: 
            switch(token.keyword){
                case IF:
                    res = condiCheck(localTable);
                    return res;

                case FUNCTION:
                    if(insideFunc){ // WE DO NOT WANT NESTED FUNCTIONS
                        errHandler(SYNTAX_ERR,"Correct, we do not want nested functions\n"); 
                    }
                    res = functionCheck();
                    if(res != SUCCESS_ERR){
                        return res;
                    }
                    return statement_list(localTable);

                case WHILE:
                    res = checkWhile(localTable);
                    return res;

                case RETURN:
                    return SUCCESS_ERR;
                    
                default:
                    errHandler(SYNTAX_ERR,"Syntax error\n");
                    return SYNTAX_ERR;
            }
        case ID:
            funcName = htab_find(funTable, token.string);   // WAS FUNCTION DEFINED BEFORE?
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
                    errHandler(SYNTAX_ERR,"Syntax error\n");
                }
                
                token_res = GetToken(&token);  
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
            p_return *ret_type = malloc(sizeof(p_return));
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
                errHandler(LEX_ERR,"Lexical error\n");  // LOOKING FOR ID
            }
            if(token.type != ID){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER $ BUILTIN <---\n");
            }
            // SEMANTIC
            insertInstruction(iList, PUSHS_ID_I, token.string, NULL, NULL);
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n"); 
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
                errHandler(LEX_ERR,"Lexical error\n"); 
            }
            switch(token.type){
                case R_PAR:
                    return SUCCESS_ERR;

                case COMMA:
                    multipleParams = true;
                    return builtinParams();     //bar(string $y,...)

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
    if(token.type != L_PAR){                   // L_PAR CHECK (
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
    }
    expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    res = checkIfStat(localTable);    // CHECK LEFT SIDE OF CONDITION IN WHILE
    if(res != SUCCESS_ERR){
        return res;
    }

    res = checkIfStat(localTable);    // CHECK RIGHT SIDE OF CONDITION IN WHILE
    if(res != SUCCESS_ERR){
        if(token.type == L_BRAC){
            goto ONLY_OP_W;
        }
        return res;
    }
    
    
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    ONLY_OP_W:
    insertInstruction(iList, LABEL_I, NULL, NULL, label);     // GENERATE ELSE LABEL  
    if(token.type != L_BRAC){               // L_BRAC CHECK {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    expr_tok = (token_t*) malloc(sizeof(*expr_tok));
    if(expr_tok == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    expr_parse(localTable, expression, iList, NULL); 
    exprListDispose(expression);
    
    insertInstruction(iList, GENERATE_WHILE_I, NULL, NULL, label); // CHECK CONDITION AND JUMP ON AFTER ELSE IF NECESARY
    insideWhile = true;
    res = statement_list(localTable);  // INSIDE WHILE CHECK
    insertInstruction(iList, JUMP_I, NULL, NULL, label);     // GENERATE AFTER WHILE JUMP  
    insertInstruction(iList, LABEL_END_I, NULL, NULL, label);     // GENERATE AFTER WHILE LABEL  

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

    stat_t *statement;
    statement = (stat_t*) malloc(sizeof(*statement));
    if(statement == NULL){
        return INTERNAL_ERR;
    }

    token_res = GetToken(&token);   // function <ID> (
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_PAR){
        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING LEFT PARENTHESIS IN FUNCTION <---\n");
    }
    res = funcParams(localTable, statement, 0); // FUNC PARAMS
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

    if(token.type != KEYWORD){  
        token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type
        if(!token_res){
            errHandler(LEX_ERR,"Lexical error\n");
        }
        if(token.type != KEYWORD){
            return SYNTAX_ERR;
        }
    }

    switch(token.keyword){   
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
            return SYNTAX_ERR;  // while, if etc..
    }
    
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

            case INT_T: case FLOAT_T: case STRING_T:    
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
        if(expr_parse(localTable, expression, iList, NULL) != currentReturnType){   // sending return expression to expr_parser
            errHandler(SEM_PARAM_ERR, "Wrong return type\n");
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
                errHandler(LEX_ERR,"Lexical error\n"); 
            }
            if(token.type != DOLLAR){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED $ <---\n");
            }

            token_res = GetToken(&token);   // type $ <ID>
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n");  // LOOKING FOR ID
            }
            if(token.type != ID){
                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER TYPE IN FUCNTION <---\n");
            }
            // SEMANTIC
            if(htab_find(localTable, token.string)){
                errHandler(SEM_PARAM_ERR, "Semantic error PARAMS REDEFINITION\n"); 
            }
            statementIn = htab_lookup_add(localTable, token.string);  // ADD PARAM TO LOCAL VAR TABLE
            statementIn->type = statement->type;
            
            token_res = GetToken(&token);   // type <ID>,   OR type <ID>)
            if(!token_res){
                errHandler(LEX_ERR,"Lexical error\n"); 
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
                return SYNTAX_ERR;  
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
        case R_PAR:
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
    if(token.type != L_PAR){                   // L_PAR CHECK (
        errHandler(res,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");
    }
    *expr_tok = token;
    insertExpr(expression, expr_tok);
    res = checkIfStat(table);    // CHECK LEFT SIDE OF CONDITION IN IF
    if(res != SUCCESS_ERR){
        return res;
    }
    res = checkIfStat(table);    // CHECK RIGHT SIDE OF CONDITION IN IF
    if(res != SUCCESS_ERR){
        if(token.type == L_BRAC){
            goto ONLY_OP;
        }
        return res;
    }

    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    ONLY_OP:
    if(token.type != L_BRAC){               // L_BRAC CHECK {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    
    token_t *expr_tok2 = (token_t*) malloc(sizeof(*expr_tok2));
    if(expr_tok2 == NULL){
        return INTERNAL_ERR;
    }
    *expr_tok2 = token;
    insertExpr(expression, expr_tok2);
    expr_parse(table, expression, iList, NULL);  
    insertInstruction(iList, GENERATE_IF_I, NULL, NULL, label); // CHECK CONDITION AND JUMP ON AFTER ELSE IF NECESARY
    exprListDispose(expression);
    insideIf = true;
    res = statement_list(table);  // INSIDE IF CHECK
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
    if(token.keyword != ELSE){               // WE NEED ELSE TOKEN STRICTLY
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING ELSE <---\n");
    }
    token_res = GetToken(&token);  
    if(!token_res){
        errHandler(LEX_ERR,"Lexical error\n");
    }
    if(token.type != L_BRAC){               // L_BRAC CHECK {
        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");
    }
    insideIf = true;
    res = statement_list(localTable); // ELSE INSIDE CHECK
    if(res != SUCCESS_ERR){
        return res;
    }
    return SUCCESS_ERR;
}


int expression_check(htab_t *table){
    int res = SYNTAX_ERR;

    token_res = GetToken(&token);  
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
        
        res = statement_list_inside(table, ret_type);
        statement->type = (int)*ret_type;
        return res;  
    }
    if(token.type == SEMICOL){
        errHandler(SEM_UNDEF_VAR_ERR, "Variable not defined\n");
    }
    return res;
}



int statement_list_inside(htab_t *table, p_return *ret_type){
    int res = SUCCESS_ERR;
    token_res = GetToken(&token);  
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
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = separators(table, ret_type);
            return res;

        case L_PAR:
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            return statement_list_inside(table, ret_type);

        case INT_T: 
        case FLOAT_T: 
        case STRING_T:
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
                return SYNTAX_ERR;  
            }
            *expr_tok = token;
            insertExpr(expression, expr_tok);
            res = separators(table, ret_type);
            return res;

        case ID:    
            funcName = htab_find(funTable, token.string);   // Was function defined before??
            switch(funcName->value[0]){
                case 'S': case 's':
                    *ret_type = ret_string;
                    break;

                case 'I': case 'i':
                    *ret_type = ret_int;
                    break;

                case 'F': case 'f':
                    *ret_type = ret_float;
                    break;
            }
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
                    errHandler(SYNTAX_ERR, "Syntax error\n");
                }
                token_res = GetToken(&token);  
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
           
            char *passPOP;
            if(currentPOP){
            passPOP = malloc(sizeof(char)* strlen(currentPOP));
            strcpy(passPOP, currentPOP);
            }
            else{
                passPOP = NULL;
            }
            *ret_type = expr_parse(table, expression, iList, passPOP);
            free(currentPOP);
            exprListDispose(expression);
            return SUCCESS_ERR;

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
 * DOPSAT EPSILON DO SEPARATORU V LL
 * VOLÁNÍ FUNKCÍ V LL
 * RET WRONG TYPE 4 - vracím 2, očekává 4
 * MUltiple statements, Single statement - err 2, should be 0
 * 
 */





