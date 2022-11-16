#include "parser.h"
#include "scanner.h"
#include "error.h"


int main(){
    int err_code = 0;
    err_code = parse();
    if(err_code == SUCCESS_ERR){
        // SUCCESSFUL PARSE
        return 0;
    }
    fprintf(stderr,"Could not compile the code\n");
    return err_code;


}
