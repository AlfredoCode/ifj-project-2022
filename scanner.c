/*
 * =================================================== *
 * Name:       scanner.c                               *
 * Authors:    xmoskv01                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#include "scanner.h"

int isKeyword(char *str){
    if(strcmp(str, "else") == 0){
        return ELSE;
    }
    if(strcmp(str, "float") == 0){
        return FLOAT;
    }
    if(strcmp(str, "function") == 0){
        return FUNCTION;
    }
    if(strcmp(str, "if") == 0){
        return IF;
    }
    if(strcmp(str, "int") == 0){
        return INT;  
    }
    if(strcmp(str, "null") == 0){
        return NULL_K;
    }
    if(strcmp(str, "return") == 0){
        return RETURN;
    }
    if(strcmp(str, "string") == 0){
        return STRING;
    }
    if(strcmp(str, "void") == 0){
        return VOID;
    }
    if(strcmp(str, "while") == 0){
        return WHILE;
    }
    return -1;
}

bool GetProlog(){
    int c;
    c = getchar();
    if(c == '<'){
        c = getchar();
        if (c == '?'){
            c = getchar();
            if( c == 'p'){
                c = getchar();
                if (c == 'h'){
                    c = getchar();
                    if (c == 'p'){
                        c = getchar();
                        if (isspace(c)){
                            return true;
                        }
                        if (c == '/'){
                            c = getchar();
                            if(c == '*'){
                                bool end_cycle = false;
                                bool star = false;
                                while(!end_cycle){
                                    c = getchar();
                                    if (c == '*'){
                                        star = true;
                                    }
                                    else if (c == '/' && star){
                                        end_cycle = true;
                                    }
                                    else{
                                        star = false;
                                    }
                                }
                                return true;
                            }
                            else if (c == '/'){
                                while(c != '\n'){
                                    c = getchar();
                                }
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool GetToken(token_t *token){
    STATES state = S;
    token->string = NULL;
    int str_size = 64;
    char* str = malloc(str_size);
    int str_index = 0;
    int c;
    while(true){
        c = getchar();
        if (state == S){
            if(isspace(c)){
                continue;
            }
            if (c == '+'){
                token->type = ADD;
                return true;
            }
            if (c == '-'){
                token->type = SUB;
                return true;
            }
            if (c == '*'){
                token->type = MUL;
                return true;
            }
            if (c == '/'){
                state = DIV_S;
                continue;
            }
            if(c == '$'){
                token->type = DOLLAR;
                return true;
            }
            if(c == ','){ 
                token->type = COMMA;
                return true;
            }
            if(isalpha(c) || c == '_'){
                state = ID_S;
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(c == '?'){
                state = TYPE_S;
                continue;
            }
            if(c == EOF){
                token->type = EOF_T;
                return true;
            }
            if(c == '}'){
                token->type = R_BRAC;
                return true;
            }
            if(c == '{'){
                token->type = L_BRAC;
                return true;
            }
            if(c == ')'){
                token->type = R_PAR;
                return true;
            }
            if(c == '('){
                token->type = L_PAR;
                return true;
            }
            if(c == ';'){
                token->type = SEMICOL;
                return true;
            }
            if(c == '='){
                state = ASSIG_S;
                continue;
            }
            if(c == '<'){
                state = LESS_S;
                continue;
            }
            if(c == '>'){
                state = GREAT_S;
                continue;
            }
            if(c == '.'){
                token->type = KONKAT;
                return true;
            }
            if(c == '!'){
                state = NOT;
                continue;
            }
            if(c == ':'){
                token->type = COL;
                return true;
            }
            if(c == '"'){
                state = STRING_START;
                continue;
            }
            if(isdigit(c)){
                state = INT_S;
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == DIV_S){
            if (c == '*'){
                state = BLOCK_COM_S;
                continue;
            }
            if (c == '/'){
                state = LINE_COM_S;
                continue;
            }
            ungetc(c, stdin);
            token->type = DIV;
            return true;
        }
        if(state == LINE_COM_S){
            if (c == '\n' || c == EOF){
                state = S;
            }
            continue;
        }
        if(state == BLOCK_COM_S){
            if(c == EOF){
                return false;
            }
            if (c == '*'){
                state = BLOCK_COM;
            }
            continue;
        }
        if(state == BLOCK_COM){
            if (c == '/'){
                state = S;
            }
            else if(c == '*'){
                continue;
            }
            else{
                state = BLOCK_COM_S;
            }
            continue;
        }
        if(state == ID_S){
            if(c == '_' || isalpha(c) || isdigit(c)){
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            else{
                ungetc(c, stdin);
                str[str_index] = '\0';
                int keyword = isKeyword(str);
                if(keyword == -1){
                    token->type = ID;
                    token->string = str; 
                }
                else{
                    token->type = KEYWORD;
                    token->keyword = keyword;
                    token->string = str;
                }
                return true;
            }
        }
        if(state == TYPE_S){
            if(c == '>'){
                state = ENDMARK;  
                continue;
            }
            else{
                ungetc(c, stdin);
                token->type = TYPE;
                return true;
            }
        }
        if(state == ENDMARK){
            if(c == EOF){
                token->type = EOF_T;
                return true;
            }
        }
        if(state == ASSIG_S){
            if(c == '='){
                state = EQEQ;
                continue;
            }
            else{
                ungetc(c, stdin);
                token->type = ASSIG;
                return true;
            }
        }
        if(state == EQEQ){
            if(c == '='){
                token->type = EQ;
                return true;
            }
            return false;
        }
        if(state == LESS_S){
            if(c == '='){
                token->type = LESS_EQ;
                return true;
            }
            ungetc(c, stdin);
            token->type = LESS;
            return true;
        }
        if(state == GREAT_S){
            if(c == '='){
                token->type = GREAT_EQ;
                return true;
            }
            ungetc(c, stdin);
            token->type = GREAT;
            return true;
        }
        if(state == NOT){
            if(c == '='){
                state = NOT_EQ_S;
                continue;
            }
        }
        if(state == NOT_EQ_S){
            if(c == '='){
                token->type = NOT_EQ;
                return true;
            }
            return false;
        }
        if(state == STRING_START){
            if(c == '"'){
                str[str_index] = '\0';
                token->type = STRING_T;
                token->string = str;
                return true;
            }
            if(c == '\\'){
                state = ESCAPE_SEQ;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(32 <= c && c <= 127){
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == ESCAPE_SEQ){
            if(c == 'x'){
                state = SYM_HEX1;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if('0' <= c && c <= '7'){
                state = SYM_OCT1; 
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(c == 'n' || c == 't' || c == '"' || c == '\\' || c == '$'){
                state = STRING_START;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == SYM_HEX1){
            if(isdigit(c) || isalpha(c)){
                state = SYM_HEX2;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == SYM_HEX2){
            if(isdigit(c) || isalpha(c)){
                state = STRING_START;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == SYM_OCT1){
            if('0' <= c && c <= '7'){
                state = SYM_OCT2;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == SYM_OCT2){
            if('0' <= c && c <= '7'){
                state = STRING_START;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == INT_S){
            if(isdigit(c)){
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(c == 'E' || c == 'e'){
                state = EXPO;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(c == '.'){
                state = DECIMAL;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            str[str_index] = '\0';
            token->string = str;
            token->type = INT_T;
            
            ungetc(c, stdin);
            return true;
        }
        if(state == EXPO){
            if(c == '+' || c == '-'){
                state = EXPO_SIG;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(isdigit(c)){
                state = FLOAT_E_S;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == DECIMAL){
            if(isdigit(c)){
                state = FLOAT_S;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == FLOAT_S){
            if(isdigit(c)){
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            if(c == 'E' || c == 'e'){
                state = EXPO;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            else{
                str[str_index] = '\0';
                token->string = str;
                token->type = FLOAT_T;
                ungetc(c, stdin);
                return true;
            }
        }
        if(state == EXPO_SIG){
            if(isdigit(c)){
                state = FLOAT_E_S;
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
        }
        if(state == FLOAT_E_S){
            if(isdigit(c)){
                if(str_index >= str_size){
                    str_size += 64;
                    str = realloc(str, str_size);
                }
                str[str_index] = c;
                str_index++;
                continue;
            }
            else{
                str[str_index] = '\0';
                token->string = str;
                token->type = FLOAT_T;
                ungetc(c, stdin);
                return true;
            }
        }
        return false;
    }
}