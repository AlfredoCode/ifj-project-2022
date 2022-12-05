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

//HELPFUL VARIABLES
int labelcnt = 0;
instructList_T *instrList;

// ============ INSTRUCTION LIST ============
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

void generateWrite()
{
    printf("POPS GF@temp0\n");
    printf("WRITE GF@temp0\n");
}

//TODO
void generateSubstring();
void generateOrd();
void generateChr();

// ARITHMETIC ON STACK
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

// RELATION
void generateLts(){
    printf("LTS\n");
}
void generateGts(){
    printf("GTS\n");
}
void generateEqs(){
    printf("EQS\n");
}

// BOOL
void generateAnds(){
    printf("ANDS\n");
}
void generateOrs(){
    printf("ORS\n");
}
void generateNots(){
    printf("NOTS\n");
}

// COVERSIONS
void generateInt2Floats(){
    printf("INT2FLOATS\n");
}
void generateFloat2Ints(){
    printf("FLOAT2INTS\n");
}

// STRING TODO
void generateConcat()
{
    printf("# STACK CONCAT\n");
    printf("POPS GF@temp0\n");
    printf("POPS GF@temp1\n");
    printf("CONCAT GF@temp2 GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp2\n");
}

void generateStrlen(char *var, char *symb, INSTRUCTIONS type){
    if(type == STRLENVAR_I){
        printf("STRLEN LF@%s LF@%s", var, symb); 
    }else if(type == STRLENKONST_I){
        printf("STRLEN LF@%s string@%s", var, stringConvertor(symb));
    }
}

void generateGetchar(char *dest, char *op1, char *op2);
void generateSetchar(char *dest, char *op1, char *op2);

// STACK
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

// FRAME
void generateCreateFrame(){
    printf("CREATEFRAME\n");
}

void generatePushFrame(){
    printf("PUSHFRAME\n");
}

void generatePopFrame(){
    printf("POPFRAME\n");
}

void generateCall(char *label){
    //fce pro vytvoreni unikatniho lablu
    printf("CALL %s\n", label);
}

void generateReturn(){
    printf("RETURN\n");
}

// DATAFLOW TODO
void generateLabel(char *label)
{
    printf("LABEL ??%s\n", label);
}
void UniqueLabel(char *label);

void generateJump(char *label);
void generateJumpIfEqs(char *label);


void generateJumpIfNEqs(char *label){
    printf("JUMPIFNEQS %s\n",label);
}
void generateExit(char *number);        // 0 <= number <= 49

// MISC TODO
void generateProgramHead(){
    printf(".IFJcode22\n");
    printf("DEFVAR GF@temp0\n");
    printf("DEFVAR GF@temp1\n");
    printf("DEFVAR GF@temp2\n");
    printf("JUMP ??main\n");
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

void generateDefvar(char *var){        //jak zjistim jestli ma byt lf, gf, nevbo tf
    printf("DEFVAR LF@%s\n", var);
}

void generateLocDefVar(stat_t *data){
    // printf("DEFVAR LF@%s\n", data->name); 
    generateDefvar(data->name);
}

void generateType(char *var, char *symb);

void generateMainStart()
{
    printf("# START OF MAIN\n");
    printf("LABEL ??main\n");
    generateCreateFrame();
    generatePushFrame();
}

void generateMainEnd()
{
    generatePopFrame();
    printf("# END OF PROGRAM\n");
}

void generateFuncStart(char *funcname){
     generateLabel(funcname);
     generateCreateFrame();
     generatePushFrame();

}
void generateFunctionEnd(){
    generatePopFrame();
    generateReturn();
}

// HELPER FUNCTIONS
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

void UniqueLabel(char *labelbefore){
    printf("LABEL ??%s%d\n", labelbefore, labelcnt);
    labelcnt++;
}

/*****************************Traverse through list of instructions*****************************/
void generatorInit(instructList_T *instrList, htab_list *symList){
    generateProgramHead();
    generateMainStart();
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
                generatePushFrame();
                break;

            case POPFRAME_I:
                generatePopFrame();
                break;

            case DEFVAR_I:
                break;

            case DEFVAR_LOC_I:
                symList->activeElement = symList->activeElement->next;
                if(symList->activeElement != NULL){
                    htab_for_each(symList->activeElement, generateLocDefVar);    // We have to start from index 1, because on index 0 is funTable
                }
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
                generateConcat();
                break;

            case STRLENVAR_I:
                generateStrlen(instrList->activeElement->op1, instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case STRLENKONST_I:
                generateStrlen(instrList->activeElement->op1, instrList->activeElement->op2, instrList->activeElement->operation);
                break;

            case GETCHAR_I:
                break;

            case SETCHAR_I:
                break;

            case TYPE_I:
                break;

            case LABEL_I:
                generateLabel(instrList->activeElement->dest);
                break;
            case FUNC_S_I:
                generateFuncStart(instrList->activeElement->dest);
                break;
            
            case FUNC_E_I:
                generateFunctionEnd();
                break;

            case JUMP_I:
                break;

            case JUMPIFEQS_I:

                
                break;

            case JUMPIFNEQS_I:
                generateJumpIfNEqs("else");  // TODO UNIQUE LABEL ELSE
                break;

            case EXIT_I:
                break;

            default: 
                break;
        }
        Next(instrList);
    }
    generateMainEnd();
}
