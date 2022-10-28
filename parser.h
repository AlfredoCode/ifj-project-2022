/*
 * =================================================== *
 * Name:       parser.h                                *
 * Authors:    xsafar27                                * 
 * Last modif: 10/24/2022                              *
 * =================================================== *
 */

#ifndef __PARSER_H__
#define __PARSER_H__

// Enum for statement types
enum stat_type {
    t_fun,
    t_int,
    t_float,
    t_str,
};

// Internal structure used to store variables and functions inside htab
typedef struct statement {
    stat_type type;
    char* name;
    char* value;
} stat_t;

#endif  // __PARSER_H__

