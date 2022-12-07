/*
 * =================================================== *
 * Name:       error.c                                 *
 * Authors:    xhofma11                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#include <stdio.h>
#include <stdlib.h>

#include "error.h"

void errHandler(err_code err, char* message)
{
    fprintf(stderr, "Exit code (%i): %s\n", err, message);
    exit(err);
}

