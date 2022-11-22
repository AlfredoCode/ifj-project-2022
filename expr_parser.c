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
const char prec_table [18][18] = {
/* :)       +    -    *    /    .    <    >    <=   >=  ===  !==   (    )    ID  INT  FLT  STR   $   */
/*  +  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>'},
/*  -  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>'},
/*  *  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>'},
/*  /  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', 'X', '>'},
/*  .  */{ '>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '>', '<', '<', '<', '<', '>'},
/*  <  */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/*  >  */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/*  <= */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/*  >= */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/* === */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/* !== */{ '<', '<', '<', '<', '>', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '>'},
/*  (  */{ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', 'X'},
/*  )  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>'},
/*  ID */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>'},
/* INT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>'},
/* FLT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>'},
/* STR */{ 'X', 'X', 'X', 'X', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', '>'},
/*  $  */{ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', 'X', '<', '<', '<', '<', '$'},
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
        case DOLLAR:
            return sym_dollar;
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
        case L_BRAC:
            return sym_end;

        default:
            err_msg(SYNTAX_ERR, "Bad expression");
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
        if (tok2->symbol == term_str) {
            // Remove top two terms
            stackPop(stack);
            stackPop(stack);

            // Change top item symbol to str
            // I only care about the symbol now anyways
            stackPeek(stack, 0)->symbol = term_str;

            return 0;
        }
        err_msg(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
        return 1;
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
    
    err_msg(SYNTAX_ERR, "Error in expresion - incompatible datatypes"); 
    return 1;
}

int evaluate_bool(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);

    if (tok1->symbol == term_str) {
        if (tok2->symbol == term_str) {
            // same as in line 87
            stackPop(stack);
            stackPop(stack); 

            stackPeek(stack, 0)->symbol = term_bool;

            return 0;
        }
        err_msg(SYNTAX_ERR, "Cant do anything with string and non-string, no str-num here");
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

    err_msg(SYNTAX_ERR, "Error in expresion - incompatible datatypes."); 
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

    return 0;
}

int evaluate_concatenation(stack_t *stack)
{
    stack_token_t *tok1 = stackPeek(stack, 0);
    stack_token_t *tok2 = stackPeek(stack, 2);

    // I need both to be strings
    if (!(tok1->symbol == term_str && tok2->symbol == term_str)){
        err_msg(SYNTAX_ERR, "Error in expresion - incompatible datatypes.");
        return 1;
    }

    // Just need to pop the first two, since the third one is already a term_str
    stackPop(stack);
    stackPop(stack);

    return 0;
}

int evaluate(stack_t *stack, htab_t *symtable)
{   
    stack_token_t *top = stackPeek(stack, 0);
    // for use in div check
    stack_token_t *tok; 
    stat_t *id;
    
    switch (top->symbol){
        // Check if var is declared.
        // Set type according to var type.
        case sym_id:
            // Looks if the var is defined in passed symtable
            id = htab_find(symtable, top->token->string);
            // I only get NULL if its not defined
            if (!id) {
               err_msg(SEM_UNDEF_VAR_ERR, "Undefined variable in expression!");
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
                    err_msg(SEM_OTHERS_ERR, "What did you do.");
                    break;
            }

            return 0;
        
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
        
        // sym_[add|sub|mul]
        case sym_add:
        case sym_sub:
        case sym_mul:
            arithmetic_check(stack);
            break;
        
        // sym_div
        case sym_div:
            // Need to check div by zero
            // Since we dont store int and float vals in the same place
            tok = stackPeek(stack, 0);
            if (tok->symbol == term_int) {
                if (!tok->token->integer){
                    err_msg(SEM_OTHERS_ERR, "Attempted div by 0.");
                    break;
                }
            } else if (tok->symbol == term_float) {
                if (!tok->token->decimal){
                    err_msg(SEM_OTHERS_ERR, "Attempted div by 0.");
                    break;
                }
            }

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
            err_msg(SYNTAX_ERR, "No rules applicable for this expression.");
            return 1;
            break;
    } 

    // If nothing
    return err_uhoh;
}

p_return get_last(stack_t *stack)
{
    // Top element is $
    stack_token_t *last = stackPeek(stack, 1);

    switch (last->symbol){
        case term_int:
            return ret_int;

        case term_float:
            return ret_float;

        case term_str:
            return ret_string;

        case term_bool:
            return ret_bool;
            //
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
    
    // First symbol needs to be dollar
    stackPeek(stack, 0)->symbol = sym_dollar;
   
    // Check if I start with operator and not operand 
    token_t *start_token = getNext(list)->token;
    p_symbol curSymbol = tokenToTerminal(start_token);
    if (curSymbol < sym_lbr) err_msg(SYNTAX_ERR, "Bad first token in expression.");

    // Bools
    bool parse = true;
    bool operand = true;

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
                curToken = getNext(list)->token;
                curSymbol = tokenToTerminal(curToken);
               
                // Check if I didnt get ab or ++ 
                if (operand) {
                    if (
                        curSymbol == sym_id     ||
                        curSymbol == sym_int    ||
                        curSymbol == sym_float  ||
                        curSymbol == sym_str
                    ) {
                      err_msg(SYNTAX_ERR, "Operand cant go after operand.");      
                    }

                    operand = false;
                } else {
                    if (curSymbol < sym_lbr) {
                        err_msg(SYNTAX_ERR, "Operator cant go after operator.");
                    }

                    operand = true;
                }
                break;

            // Reduce expression
            case '>':
                evaluate(stack, symtable);
                break;

            // Non-rule
            case 'X':
                err_msg(SEM_TYPE_ERR, "Bad operands, non-rule in precedence table.");
                break;

            // Again, how did you get here
            default:
                err_msg(SYNTAX_ERR, "Rule not in precedence table.");
                break;
        }
    } 
    
    p_return ret = get_last(stack);
    stackClear(stack);
    return ret;
}
