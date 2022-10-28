#ifndef __PARSER_H__
#define __PARSER_H__

enum stat_type {
    t_fun,
    t_int,
    t_float,
    t_str,
};

typedef struct statement {
    stat_type type;
    char* name;
    char* value;
} stat_t;
#endif // __PARSER_H__
