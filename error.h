/*
 * =================================================== *
 * Name:       error.h                                 *
 * Authors:    xhofma11                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */


#ifndef ERROR_H_
#define ERROR_H_

typedef enum{ 
    SUCCESS_ERR,
    LEX_ERR,
    SYNTAX_ERR,
    SEM_FUNC_ERR,
    SEM_PARAM_ERR,
    SEM_UNDEF_VAR_ERR,
    SEM_RETURN_ERR,
    SEM_TYPE_ERR,
    SEM_OTHERS_ERR,
    INTERNAL_ERR = 99
}err_code;

void errHandler(err_code result, char *msg);

#endif // ERROR_H_

