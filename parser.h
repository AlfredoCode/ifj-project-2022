/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 *             xlogin00
 * Last modif: 10/24/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__

typedef struct var {
    int type; //enum
    char* name;
    void* val;
} var_t;

typedef struct function {
    char* name;
    int ret; //enum
    var_t *args;
} function_t;

#endif
