/*
 * =================================================== *
 * Name:       error.c                                 *
 * Authors:    xsafar27                                * 
 * Last modif: 11/14/2022                              *
 * =================================================== *
 */

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void err_msg(err_code err, char* message)
{
    fprintf(stderr, "Exit code (%i): %s\n", err, message);
    exit(err);
}

