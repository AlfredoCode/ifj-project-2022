/*
 * =================================================== *
 * Name:       expr_parse.c                            *
 * Authors:    xsafar27                                * 
 * Last modif: 11/20/2022                              *
 * =================================================== *
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "expr_parser.h"
#include "expr_stack.h"
#include "error.h"
#include "scanner.h"
#include "parser.h"
#include "symtable.h"

// Global table for parsing
const char prec_table [19][19] = {
/* :)       +    -    *    /    .    <    >    <=   >=  ===  !==   (    )    ID  INT  FLT  STR  NUL   $   */
/*  +  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '<', '>'},
/*  -  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '<', '>'},
/*  *  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '<', '>'},
/*  /  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '<', '>'},
/*  .  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '<', '>'},
/*  <  */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/*  >  */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/*  <= */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/*  >= */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/* === */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/* !== */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},
/*  (  */{ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', 'X'},
/*  )  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/*  ID */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/* INT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/* FLT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/* STR */{ 'X', 'X', 'X', 'X', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/* NUL */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},
/*  $  */{ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', '<', '<', '<', '<', '$'},
};

p_symbol tokenToTerminal(token_t *token)
{
    switch (token->type) {
        case ADD:
            return sym_add;
        case SUB:
            return sym_sub;
        case MUL:
            return sym_mul;
        case DIV:
            return sym_div;
        case ID:
            return sym_id;
        case R_PAR:
            return sym_rbr;
        case L_PAR:
            return sym_lbr;
        case EQ:
            return sym_eq;
        case LESS:
            return sym_lt;
        case LESS_EQ:
            return sym_lte;
        case GREAT:
            return sym_gt;
        case GREAT_EQ:
            return sym_gte;
        case KONKAT:
            return sym_con;
        case NOT_EQ:
            return sym_neq;
        case STRING_T:
            return sym_str;
        case INT_T:
            return sym_int;
        case FLOAT_T:
            return sym_float;

        case SEMICOL:
        case COL:
        case L_BRAC:
            return sym_end;
       
        case KEYWORD:
            if (token->keyword == NULL_K) {
                return sym_null;
                break;
            }
        // if not null
        // fallthru 

        default:
            errHandler(SYNTAX_ERR, "Bad expression");
            return err_uhoh;
    }

    // how did you even get here
    return err_uhoh;
}

stack_token_t *nextNonTerm(stack_t *stack)
{
    // I want to return sym, not term.
    // So I look past the first one if its term.
    if (stackPeek(stack, 0)->symbol > sym_end) {
        return stackPeek(stack, 1);
    } else {
        return stackPeek(stack, 0);
    }
}

int arithmetic_check(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);
    
    if (tok1->symbol == term_str) {
        if (tok2->symbol == term_str || tok2->symbol == term_null) {
            // Remove top two terms
            stackPop(stack);
            stackPop(stack);

            // Change top item symbol to str
            // I only care about the symbol now anyways
            stackPeek(stack, 0)->symbol = term_str;

            return 0;
        }
        errHandler(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
        return 1;
    } 

    // NULL time
    if (tok1->symbol == term_null){
        // need to check if both are NULL
        if (tok2->symbol == term_null){
            stackPeek(stack, 2)->symbol = term_null;
        }
        // Just pop the two tokens
        stackPop(stack);
        stackPop(stack);

        return 0;

    } else if (tok2->symbol == term_null) {
        // Copy the type of the first token
        tok2->symbol = tok1->symbol;
        stackPop(stack);
        stackPop(stack);
        
        return 0;
    }

    // I hate this thing, but here we go
    // if both tokens are either int or float
    if ((tok1->symbol == term_float || tok1->symbol == term_int)
           && (tok2->symbol == term_float || tok2->symbol == term_int)) {
        // Set the type to float if at least one of them is float, int otherwise
        if (tok1->symbol == term_float || tok2->symbol == term_float) {
            stackPeek(stack, 2)->symbol = term_float;     
        } else {
            stackPeek(stack, 2)->symbol = term_int;     
        }

        // Pop the two items that are not needed anymore
        stackPop(stack);
        stackPop(stack);

        return 0;
    }
    
    errHandler(SYNTAX_ERR, "Error in expresion - incompatible datatypes"); 
    return 1;
}

int evaluate_bool(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);

    // null is valid too
    if (tok1->symbol == term_null || tok2->symbol == term_null) {
        stackPop(stack);
        stackPop(stack);
        
        stackPeek(stack, 0)->symbol = term_bool;

        return 0;
    }

    // Strings
    if (tok1->symbol == term_str) {
        if (tok2->symbol == term_str) {
            // same as in line 87
            stackPop(stack);
            stackPop(stack); 

            stackPeek(stack, 0)->symbol = term_bool;

            return 0;
        }
        errHandler(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
        return 1; 
    }

    // Much easier, are both numbers? Great, its a bool.
    if ((tok1->symbol == term_float || tok1->symbol == term_int)
            && (tok2->symbol == term_float || tok2->symbol == term_int)) {
        stackPop(stack);
        stackPop(stack);

        stackPeek(stack, 0)->symbol = term_bool;
        
        return 0;
    }

    errHandler(SYNTAX_ERR, "Error in expresion - incompatible datatypes."); 
    return 1;
}

int evaluate_brackets(stack_t *stack)
{
    // I want to save the middle token
    stack_token_t *tok = stackPeek(stack, 1);

    // Pop both brackets and the middle one (the one I saved)
    stackPop(stack);
    stackPop(stack);
    stackPop(stack);

    // Return the thing between brackets back
    stackPush(stack, tok->token);
    stackPeek(stack, 0)->symbol = tok->symbol;

    return 0;
}

int evaluate_concatenation(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);

    // Null checks, again
    if (tok1->symbol == term_null || tok2->symbol == term_null){
        stackPop(stack);
        stackPop(stack);
        
        // already the token on top, so why not
        tok2->symbol = term_str;
        return 0;
    }

    // I need both to be strings
    if (!(tok1->symbol == term_str && tok2->symbol == term_str)){
        errHandler(SYNTAX_ERR, "Error in expresion - incompatible datatypes.");
        return 1;
    }

    // Just need to pop the first two, since the third one is already a term_str
    stackPop(stack);
    stackPop(stack);

    return 0;
}

int evaluate(stack_t *stack, htab_t *symtable)
{   
    stack_token_t *top = nextNonTerm(stack);
    stat_t *id;

    switch (top->symbol){
        // Check if var is declared.
        // Set type according to var type.
        case sym_id:
            // Looks if the var is defined in passed symtable
            id = htab_find(symtable, top->token->string);
            // I only get NULL if its not defined
            if (!id) {
               errHandler(SEM_UNDEF_VAR_ERR, "Undefined variable in expression!");
               return 1;
            }
            
            switch (id->type){
                case t_int:
                    top->symbol = term_int;
                    break;

                case t_float:
                    top->symbol = term_float;
                    break;

                case t_str:
                    top->symbol = term_str;
                    break;
                
                // My brother in christ, how did you get here
                default:
                    errHandler(SEM_OTHERS_ERR, "What did you do.");
                    break;
            }

            break;
        
        // sym_int
        case sym_int:
            top->symbol = term_int;
            break;
         
        // sym_float
        case sym_float:
            top->symbol = term_float;
            break;
        
        // sym_str
        case sym_str:
            top->symbol = term_str;
            break;
        
        // My old friend NULL
        case sym_null:
            top->symbol = term_null;
            break;

        // sym_[add|sub|mul]
        case sym_add:
        case sym_sub:
        case sym_mul:
            arithmetic_check(stack);
            break;
        
        // sym_div
        case sym_div:
            arithmetic_check(stack);
            break;

        // sym_con
        case sym_con:
            evaluate_concatenation(stack);
            break;
        
        // sym_rbr
        case sym_rbr:
            evaluate_brackets(stack);
            break;
        
        // sym_[lt|gt|lte|gte|eq|neq]
        case sym_lt:
        case sym_gt:
        case sym_lte:
        case sym_gte:
        case sym_eq:
        case sym_neq:
            evaluate_bool(stack);
            break;

        default:
            errHandler(SYNTAX_ERR, "No rules applicable for this expression.");
            return 1;
            break;
    } 

    return 0;
}

p_return get_last(stack_t *stack)
{
    stack_token_t *last = stackPeek(stack, 0);

    switch (last->symbol){
        case term_int:
            return ret_int;

        case term_float:
            return ret_float;

        case term_str:
            return ret_string;

        case term_bool:
            return ret_bool;

        case term_null:
            return ret_null;

        // How did you get here
        default:
            break;
    }
    return ret_uhoh;
}

p_return expr_parse(htab_t *symtable, expression_T *list)
{
    // Stack init
    stack_t *stack = stackInit();
    token_t *initTok = NULL;
    stackPush(stack, initTok);
    
    // First symbol needs to be end symbol
    stackPeek(stack, 0)->symbol = sym_end;
   
    // Check if I start with operator and not operand 
    token_t *start_token = getExpr(list)->token;
    
    p_symbol curSymbol = tokenToTerminal(start_token);
    if (curSymbol < sym_lbr) errHandler(SYNTAX_ERR, "Bad first token in expression.");

    // Bools
    bool parse = true;
    // bool operand = true;

    // Other defs
    stack_token_t *topToken;
    token_t *curToken = start_token;

    
    // Loop until done, done == break;
    while (parse) {
        topToken = nextNonTerm(stack);

        switch(prec_table[topToken->symbol][tokenToTerminal(curToken)]){
            // Parse end
            case '$':
                parse = false;
                break;

            // Next token
            case '=':
            case '<':
                // Push current token
                stackPush(stack, curToken);
                stackPeek(stack, 0)->symbol = curSymbol;

                // Get a new one
                curToken = getExpr(list)->token;
                curSymbol = tokenToTerminal(curToken);
                break;

            // Reduce expression
            case '>':
                evaluate(stack, symtable);
                break;

            // Non-rule
            case 'X':
                errHandler(SEM_TYPE_ERR, "Bad operands, non-rule in precedence table.");
                break;

            // Again, how did you get here
            default:
                errHandler(SYNTAX_ERR, "Rule not in precedence table.");
                break;
        }
    } 
    
    p_return ret = get_last(stack);
    stackClear(stack);
    return ret;
}
