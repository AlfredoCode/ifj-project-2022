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

int errHandler(int err){
    switch(err){
        case SUCCESS_ERR:
            return SUCCESS_ERR;

        case LEX_ERR:
            return LEX_ERR;

        case SYNTAX_ERR:
            return SYNTAX_ERR;

        case SEM_FUNC_ERR:
            return SEM_FUNC_ERR;

        case SEM_PARAM_ERR:
            return SEM_PARAM_ERR;

        case SEM_UNDEF_VAR_ERR:
            return SEM_UNDEF_VAR_ERR;

        case SEM_RETURN_ERR:
            return SUCCESS_ERR;

        case SEM_TYPE_ERR:
            return SEM_TYPE_ERR;

        case SEM_OTHERS_ERR:
            return SEM_OTHERS_ERR;

        default:
            return INTERNAL_ERR;
        
            
    }

}


#endif