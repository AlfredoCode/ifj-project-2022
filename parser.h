/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 * Last modif: 10/29/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__

// Enum for statement types
typedef enum {
    t_fun,
    t_int,
    t_float,
    t_str,
}stat_type;

// Internal structure used to store variables and functions inside htab
typedef struct statement {
    stat_type type;
    char* name;
    char* value;
} stat_t;




// ACTUAL FUNCTIONS USED IN PARSER


int prog(); // Entry point to LL grammar rules
int prologCheck(); // First token has to be prolog


#endif  // __PARSER_H__