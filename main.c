/*
 * =================================================== *
 * Name:       main.c                                  *
 * Authors:    xhofma11                                * 
 * Last modif: 11/30/2022                              *
 * =================================================== *
 */

#include "scanner.h"
#include "parser.h"
#include "error.h"

int main(){
    int err_code = 0;
    err_code = parse();

    if(err_code == SUCCESS_ERR){
        // SUCCESSFUL PARSE
        fprintf(stderr,"Compiled successfully\n");
        return err_code;
    }
    fprintf(stderr,"Could not compile the code\n");
    return err_code;
}
