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

//Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser
int insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest){
    instructElem newElement = (instructElem) malloc(sizeof(*newElement));
	if(newElement == NULL){
		return INTERNAL_ERR; // return 99
	}
	newElement->op1 = op1;
    newElement->op2 = op2;
    newElement->dest = dest;
    newElement-> operation = operation;


	newElement->previous = instrList->lastElement;	// Nastavení dat nového prvku seznamu
	newElement->next = NULL;
	if(instrList->lastElement != NULL){
		instrList->lastElement->next = newElement;	
	}
	else{
		instrList->firstElement = newElement;		// Navázání prvku do seznamu
	}
	instrList->lastElement = newElement;	
    return SUCCESS_ERR;

}
// ======================== GENERATION ==========================

// BUILT-INS TODO
void generateRead(char *var, INSTRUCTIONS type){
    switch (type){
        case READI_I:
            printf("READ LF@%s int\n", var);             
            break;
        case READF_I:
            printf("READ LF@%s float\n", var);
            break;
        case READS_I:
            printf("READ LF%s string\n", var);
            break;
        default: break;
    }
}

void generateWrite(char *symb){
    printf("WRITE LF@%s\n", symb);
}

void generateStrlen(char *var, char *symb){
    printf("STRLEN LF@%s LF@%s", var, symb);        //může být symb aji string@algo? --> přidat instrukce strlen_id, strlen_konst
}

//TODO
void generateSubstring();
void generateOrd();
void generateChr();

/*Aritmetic operations on stack*/
void generateAdds()
{
    printf("ADDS\n");
}

void generateSubs()
{
    printf("SUBS\n");
}

void generateMuls()
{
    printf("MULS\n");
}

void generateDivs()
{
    printf("DIVS\n");
}

void generateIDivs()
{
    printf("IDIVS\n");
}

/*Relation operations*/
void generateLts(){
    printf("LTS\n");
}
void generateGts(){
    printf("GTS\n");
}
void generateEqs(){
    printf("EQS\n");
}

/*Bool operations*/
void generateAnds(){
    printf("ANDS\n");
}
void generateOrs(){
    printf("ORS\n");
}
void generateNots(){
    printf("NOTS\n");
}

/*Conversion*/
void generateInt2Floats(){
    printf("INT2FLOATS\n");
}
void generateFloat2Ints(){
    printf("FLOAT2INTS\n");
}
void generateInt2Chars(){
    printf("INT2CHARS\n");
}
void generateStri2Ints(){
    printf("STRING2INTS\n");
}

// STRING TODO

// STACK TODO

// FRAME TODO

// DATAFLOW TODO

// MISC TODO

/*Program head*/
void generateProgramHead(){
    printf(".IFJcode22\n");
}

void generateMove(char *var, char *symb, INSTRUCTIONS type){
    switch(type){
        case MOVEI2LF_I:
            printf("MOVE LF@%s int@%d\n", var, atoi(symb));
            break;
        case MOVEF2LF_I:
            printf("MOVE LF@%s float@%a\n", var, atof(symb));
            break;
        case MOVES2LF_I:
            printf("MOVE LF@%s string@%s\n", var, stringConvertor(symb));
            break;
        case MOVENIL2LF_I:
            printf("MOVE LF@%s nil@nil\n", var);
            break;
        case MOVETF2LF_I:
            printf("MOVE LF@%s TF@%s\n", var, symb);
            break;
        case MOVEI2TF_I:
            printf("MOVE TF@%s int@%d\n", var, atoi(symb));
            break;
        case MOVEF2TF_I:
            printf("MOVE TF@%s float@%a\n", var, atof(symb));
            break;
        case MOVES2TF_I:
            printf("MOVE TF@%s string@%s\n", var, stringConvertor(symb));
            break;
        case MOVENIL2TF_I:
            printf("MOVE TF@%s nil@nil\n", var);
            break;
        case MOVELF2TF_I:
            printf("MOVE TF@%s LF@%s\n", var, symb);
            break;
        case MOVELF2LF_I:
            printf("MOVE LF@%s LF@%s\n", var, symb);
            break;
        case MOVETF2TF_I:
            printf("MOVE TF@%s TF@%s\n", var, symb);
            break;
        default: break;
    }
}

/*Frames and function calls*/
void generateCreateFrame(){
    printf("CREATEFRAME\n");
}

void generatePushFrame(){
    printf("PUSHFRAME\n");
}

void generatePopFrame(){
    printf("POPFRAME\n");
}

void generateDefvar(char *var){        //jak zjistim jestli ma byt lf, gf, nevbo tf
    printf("DEFVAR LF@%s\n", var);
}

void generateCall(char *label){
    //fce pro vytvoreni unikatniho lablu
    printf("CALL %s\n", label);
}

/*Stack functions*/
void generatePushs(char *symb, INSTRUCTIONS type){
    switch(type){
        case PUSHS_INT_I:
            printf("PUSHS int@%d\n", atoi(symb));
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
            printf("PUSHS nil@nil\n");
            break;

        default: 
            break;
    }
}
void generatePops(char *var){
    printf("POPS LF@%s\n", var);
}

void generateClears(){
    printf("CLEARS\n");
}

/*String convertor*/
char* stringConvertor(char* stringBefore){
    size_t size = 16;
    char* retstring = malloc(size);
    strcpy(retstring, "");
    char* helpString = malloc(5);

    for (size_t i = 0; i < strlen(stringBefore); i++){
        if (
                stringBefore[i] <= 32 ||
                stringBefore[i] == 35
           ) {
            snprintf(helpString, 5, "\\0%d", stringBefore[i]);
        } else if (stringBefore[i] == 92) { 
            if (stringBefore[i + 1] == 110) {
                // newline
                snprintf(helpString, 5, "\\010");
            } else if (stringBefore[i + 1] == 116) {
                // tab
                snprintf(helpString, 5, "\\009");
            } else {
                snprintf(helpString, 5, "\\0%d", stringBefore[i]);
            }
        } else {
            snprintf(helpString, 5, "%c", stringBefore[i]);
        }

        if (strlen(helpString) + strlen(retstring) + 1 > size) {
            size = size * 2;
            retstring = realloc(retstring, size);
        }
        strcat(retstring, helpString);
    }
    return retstring;
}
/*****************************Traverse through list of instructions*****************************/
void generatorInit(instructList_T *instrList){
    generateProgramHead();
    First(instrList);

    while(instrList->activeElement != NULL){
        switch(instrList->activeElement->operation){
            case MOVEI2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVEF2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVES2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVENIL2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVELF2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVEI2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVEF2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVES2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVENIL2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVETF2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVELF2LF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case MOVETF2TF_I:
                generateMove(instrList->activeElement->op1,instrList->activeElement->op2, instrList->activeElement->operation);
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
                generatePops(instrList->activeElement->op1);
                break;

            case CLEARS_I:
                generateClears();
                break;

            case ADDS_I:
                generateAdds();
                break;

            case SUBS_I:
                generateSubs();
                break;

            case MULS_I:
                generateMuls();
                break;

            case DIVS_I:
                generateDivs();
                break;

            case IDIVS_I:
                generateIDivs();
                break;

            case LTS_I:
                generateLts();
                break;

            case GTS_I:
                generateGts();
                break;

            case EQS_I:
                generateEqs();
                break;

            case ANDS_I:
                generateAnds();
                break;

            case ORS_I:
                generateOrs();
                break;

            case NOTS_I:
                generateNots();
                break;

            case INT2FLOATS_I:
                generateInt2Floats();
                break;

            case FLOAT2INTS_I:
                generateFloat2Ints();
                break;

            case INT2CHARS_I:
                generateInt2Chars();
                break;

            case STRI2INTS_I:
                generateStri2Ints();
                break;

            case READI_I:
                generateRead(instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case READS_I:
                generateRead(instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case READF_I:
                generateRead(instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case WRITE_I:
                generateWrite(instrList->activeElement->op1);
                break;

            case CONCAT_I:
                break;

            case STRLEN_I:
                generateStrlen(instrList->activeElement->op1, instrList->activeElement->op2);
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

            default: 
                break;
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
    /*insertInstruction(instrList, PUSHS_STRING_I, "hello worl",NULL,NULL);
    insertInstruction(instrList, PUSHS_FLOAT_I, "1.5",NULL,NULL);
    insertInstruction(instrList, PUSHS_ID_I, "2",NULL,NULL);
    insertInstruction(instrList, ADDS_I, NULL,NULL,NULL);
    insertInstruction(instrList, POPS_I, "y",NULL,NULL);
    insertInstruction(instrList, ANDS_I, NULL,NULL,NULL);
    insertInstruction(instrList, READI_I, "kdfh",NULL,NULL);
    */
    insertInstruction(instrList, MOVEF2LF_I, "x","1.5",NULL);
    insertInstruction(instrList, MOVEF2TF_I, "y","4.0",NULL);
    insertInstruction(instrList, MOVEI2LF_I, "o","-56",NULL);
    insertInstruction(instrList, MOVENIL2TF_I, "k",NULL,NULL);
    insertInstruction(instrList, MOVETF2LF_I, "f","g",NULL);
    insertInstruction(instrList, MOVES2LF_I, "d","hello world",NULL);
    insertInstruction(instrList, MOVES2TF_I, "h","uplne genialni123",NULL);
    
    generatorInit(instrList);
    return 0;
}

