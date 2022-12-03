/*
 * =================================================== *
 * Name:       generator.c                             *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 *             xjadrn03                                *
 * Last modif: 11/28/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "generator.h"
#include "error.h"

instructList_T *instrList;

/*List functions*/
void initInstList(instructList_T *instrList){
    instrList->activeElement = NULL;
    instrList->firstElement = NULL;
    instrList->lastElement = NULL;

}

void First(instructList_T *instrList ) {
	instrList->activeElement = instrList->firstElement;
}

void Next(instructList_T *instrList){
    if(instrList->activeElement != NULL){
        instrList->activeElement= instrList->activeElement->next;
    }
}

int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest){ //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser
    instructElem newElement = (instructElem) malloc(sizeof(*newElement));
	if(newElement == NULL){
		return INTERNAL_ERR; // return 99
	}
	newElement->op1 = op1;
    newElement->op2 = op2;
    newElement->dest = dest;
    newElement-> operation = operation;


	newElement->next = instrList->firstElement;	// Nastavení dat nového prvku seznamu
	newElement->previous = NULL;
	if(instrList->lastElement != NULL){
		instrList->firstElement->previous = newElement;	

	}
	else{
		instrList->lastElement = newElement;		// Navázání prvku do seznamu
	}
	
	instrList->firstElement = newElement;	
    return SUCCESS_ERR;

}
/*Generating*/
/*Program head*/
void generateProgramHead(){
    printf(".IFJcode22\n");
}

/*void generateMove(char *var, char *symb){
    printf("MOVE LF@%s ????", var);
}*/

/*Frames and function calls*/
void generateCreateFrame(){
    printf("CREATEFRAME");
}

void generatePushFrame(){
    printf("PUSHFRAME");
}

void generatePopFrame(){
    printf("POPFRAME");
}

void generateDefvar(char *var){        //jak zjistim jestli ma byt lf, gf, nevbo tf
    printf("DEFVAR LF@%s", var);
}

void generateCall(char *label){
    //fce pro vytvoreni unikatniho lablu
    printf("CALL %s", label);
}

/*Stack functions*/
void generatePushs(char *symb, INSTRUCTIONS type){
    switch(type){
        case PUSHS_INT_I:
            printf("PUSHS int@%a\n", atof(symb));
            break;
        case PUSHS_FLOAT_I:
            printf("PUSHS float@%a\n", atof(symb));
            break;
        case PUSHS_STRING_I:
            printf("PUSHS string@%s\n", stringConvertor(symb));
            break;
        case PUSHS_ID_I:
            printf("PUSHS LF@%s\n", symb);
            break;
        case PUSHS_NIL_I:
            printf("PUSHS nil@nil\n");         //?????????????
            break;
        default: break;
    }

}
//void generatePops(char *var);
//void generateClears();

/*String convertor*/
char* stringConvertor(char* stringBefore){
    int stringSizeB = strlen(stringBefore);
    int stringSizeA = stringSizeB;
    char *stringAfter = (char *) malloc(stringSizeB * sizeof(char));
    char *add = (char *) malloc(sizeof(char) * 5);
    int i = 0;
    int k = 0;
    while(stringBefore[i] != '\0'){
        if(!isdigit(stringBefore[i])){
           stringSizeA += 5;
            stringAfter = (char *)realloc(stringAfter, stringSizeA * sizeof(char *));
            if((stringBefore[i] >= 0 && stringBefore[i] <= 32) || (stringBefore[i] == 35) || (stringBefore[i] == 92)){
                if(stringBefore[i] >= 0 && stringBefore[i] <= 9){
                    sprintf(add, "/00%d", stringBefore[i]);
                    strcat(stringAfter, add);
                    k += 4;
                }else{
                    sprintf(add, "/0%d", stringBefore[i]);
                    strcat(stringAfter, add);
                    k += 4;
                } 
            }else{
                stringAfter[k] = stringBefore[i];
                k++;
            }
        }else{
            stringAfter[k] = stringBefore[i];
            k++;
        }
        i++;
    }
    return stringAfter;
}

void generatorInit(instructList_T *instrList){
    generateProgramHead();
    First(instrList);
    while(instrList->activeElement != NULL){
        switch(instrList->activeElement->operation){
            case MOVE_I:
                break;
            case CREATEFRAME_I:
                generateCreateFrame();
                break;
            case PUSHFRAME_I:
                break;
            case POPFRAME_I:
                break;
            case DEFVAR_I:
                break;
            case CALL_I:
                break;
            case RETURN_I:
                break;
            case PUSHS_INT_I:
                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);
                break;
            case PUSHS_FLOAT_I:
                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);
                break;
            case PUSHS_STRING_I:
                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);
                break;
            case PUSHS_ID_I:
                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);
                break;
            case PUSHS_NIL_I:
                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);
                break;
            case POPS_I:
                break;
            case CLEARS_I:
                break;
            case ADDS_I:
                break;
            case SUBS_I:
                break;
            case MULS_I:
                break;
            case DIVS_I:
                break;
            case IDIVS_I:
                break;
            case LTS_I:
                break;
            case GTS_I:
                break;
            case EQS_I:
                break;
            case ANDS_I:
                break;
            case ORS_I:
                break;
            case NOTS_I:
                break;
            case INT2FLOATS_I:
                break;
            case FLOAT2INTS_I:
                break;
            case INT2CHARS_I:
                break;
            case STRI2INTS_I:
                break;
            case READ_I:
                break;
            case WRITE_I:
                break;
            case CONCAT_I:
                break;
            case STRLEN_I:
                break;
            case GETCHAR_I:
                break;
            case SETCHAR_I:
                break;
            case TYPE_I:
                break;
            case LABEL_I:
                break;
            case JUMP_I:
                break;
            case JUMPIFEQS_I:
                break;
            case JUMPIFNEQS_I:
                break;
            case EXIT_I:
                break;
            default: break;
        }
        Next(instrList);
    }
}


int main(){
    instrList  = (instructList_T*)malloc(sizeof(*instrList));
    if(instrList == NULL){
        fprintf(stderr,"Malloc failure\n");
        return INTERNAL_ERR;
    }
    initInstList(instrList);
    insertInstruction(instrList, PUSHS_INT_I, "125",NULL,NULL);
    insertInstruction(instrList, PUSHS_FLOAT_I, "1.5",NULL,NULL);
    insertInstruction(instrList, PUSHS_ID_I, "x",NULL,NULL);
    generatorInit(instrList);
    //char *string = "h123456798ello wor00122032658 01ld#";
    // char *string = "\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?";
    //char *string2 = stringConvertor(string);
    //printf("%s\n", string2);
    //char *string = "Hello world<>#";
    /*char *id = "x";
    char *intliteral = "123";
    char *floatliteral = "125.478";
    char *nil = "nil";
    generatePushs(string2, PUSHS_STRING_I);
    generatePushs(id, PUSHS_ID_I);
    generatePushs(intliteral, PUSHS_INT_I);
    generatePushs(floatliteral, PUSHS_FLOAT_I);
    generatePushs(nil, PUSHS_NIL_I);*/
    

    return 0;
}
