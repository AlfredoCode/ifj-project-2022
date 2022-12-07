/*
 * =================================================== *
 * Name:       generator.c                             *
 * Authors:    xsafar27                                * 
 *             xhofma11                                *
 *             xjadrn03                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "generator.h"
#include "error.h"
#include "ins_list.h"

//HELPFUL VARIABLES
int labelcnt = 0;
instructList_T *instrList;

// BUILT-INS 
void generateRead(){
    printf("\n#READI\n");
    printf("LABEL readi\n");
    printf("READ GF@temp0 int\n");
    printf("PUSHS GF@temp0\n"); 
    printf("RETURN\n\n");

    printf("\n#READS\n");
    printf("LABEL reads\n");
    printf("READ GF@temp0 string\n");
    printf("PUSHS GF@temp0\n"); 
    printf("RETURN\n\n");

    printf("\n#READF\n");
    printf("LABEL readf\n");
    printf("READ GF@temp0 float\n");
    printf("PUSHS GF@temp0\n"); 
    printf("RETURN\n\n");

    printf("\n#READB\n");
    printf("LABEL readb\n");
    printf("READ GF@temp0 bool\n");
    printf("PUSHS GF@temp0\n"); 
    printf("RETURN\n\n");
}

void generateWrite()
{
    printf("\n#WRITE\n");
    printf("LABEL write\n");
    printf("LABEL write?while\n");
    printf("POPS GF@temp0\n");
    printf("JUMPIFEQ write?whileEnd GF@temp0 nil@nil\n");
    printf("WRITE GF@temp0\n");
    printf("JUMP write?while\n");
    printf("LABEL write?whileEnd\n");
    printf("RETURN\n\n");
}

void generateWriteCall()
{
    printf("CALL write\n");
}

void generateOrd(){
    printf("\n#ORD\n");
    printf("LABEL ord\n");
    printf("POPS GF@temp0\n");
    printf("JUMPIFEQ ordEmpty GF@temp0 string@\n"); 
    printf("PUSHS GF@temp0\n");
    printf("PUSHS int@0\n");
    printf("STRI2INTS\n");
    printf("JUMP ordEnd\n");
    printf("LABEL ordEmpty\n");
    printf("PUSHS int@0\n");
    printf("LABEL ordEnd\n");
    printf("RETURN\n\n");
}

void generateChr(){
    printf("\n#CHR\n");
    printf("LABEL chr\n");
    printf("INT2CHARS\n");
    printf("RETURN\n\n");
}

// ARITHMETIC ON STACK
void generateAdds()
{
    printf("ADDS\n");
}

void callAdds()
{
    printf("CALL ?adds\n");
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
void generateInt2Floats()
{
    printf("\n#INT2FLOATS\n");
    printf("#FLOATVAL\n");
    printf("LABEL floatval\n");
    printf("LABEL int2floats\n");
    printf("POPS GF@temp0\n");
    printf("TYPE GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp0\n");
    printf("JUMPIFNEQ ?i2fend GF@temp1 string@int\n");
    printf("INT2FLOATS\n");
    printf("LABEL ?i2fend\n");
    printf("RETURN\n");
}

void callInt2Floats()
{
    printf("CALL int2floats\n");
}

void generateFloat2Ints()
{
    printf("\n#FLOAT2INTS\n");
    printf("#INTVAL\n");
    printf("LABEL intval\n");
    printf("LABEL float2ints\n");
    printf("POPS GF@temp0\n");
    printf("TYPE GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp0\n");
    printf("JUMPIFNEQ ?f2iend GF@temp1 string@float\n");
    printf("FLOAT2INTS\n");
    printf("LABEL ?f2iend\n");
    printf("RETURN\n");
}

void callFloat2Ints()
{
    printf("CALL float2ints\n");
}

void generateStrval()
{
    printf("\n#STRVAL\n");
    printf("LABEL strval\n");
    printf("POPS GF@temp0\n");
    printf("TYPE GF@temp1 GF@temp0\n");
    printf("JUMPIFNEQ strval?notnil GF@temp1 string@nil\n");
    printf("PUSHS string@\n");
    printf("JUMP strval?end\n");
    printf("LABEL strval?notnil\n");
    printf("PUSHS GF@temp0\n");
    printf("LABEL strval?end\n");
    printf("RETURN\n");
}

// STRING
void generateConcat()
{
    printf("# STACK CONCAT\n");
    printf("POPS GF@temp0\n");
    printf("POPS GF@temp1\n");
    printf("CONCAT GF@temp2 GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp2\n");
}

void generateStrlen()
{
    printf("\n#STRLEN\n");
    printf("LABEL strlen\n");
    printf("POPS GF@temp0\n");
    printf("STRLEN GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp1\n");
    printf("RETURN\n");
}

void generateSubstr()
{
    printf("\n#SUBSTR\n");
    printf("LABEL substring\n");
    printf("CREATEFRAME\n");
    printf("PUSHFRAME\n");

    printf("DEFVAR LF@j\n");
    printf("DEFVAR LF@i\n");
    printf("DEFVAR LF@str\n");

    printf("POPS LF@j\n");
    printf("POPS LF@i\n");
    printf("POPS LF@str\n");

    printf("DEFVAR LF@amn\n");
    printf("DEFVAR LF@out\n");

    // !(i > 0)
    printf("LT GF@temp0 LF@i INT@0\n");
    printf("JUMPIFEQ ?substr_nul GF@temp0 bool@true\n");

    // i < strlen(str)
    printf("STRLEN GF@temp1 LF@str\n");
    printf("LT GF@temp0 GF@temp1 LF@i\n");
    printf("JUMPIFEQ ?substr_nul GF@temp0 bool@true\n");

    // i != strlen(str)
    printf("EQ GF@temp0 GF@temp1 LF@i\n");
    printf("JUMPIFEQ ?substr_nul GF@temp0 bool@true\n");

    // j < strlen
    printf("LT GF@temp0 GF@temp1 LF@j\n");
    printf("JUMPIFEQ ?substr_nul GF@temp0 bool@true\n");

    // i == j
    printf("JUMPIFEQ ?substr_empty LF@i LF@j\n");

    // j > i && j > 0
    printf("GT GF@temp0 LF@j LF@i\n");
    printf("JUMPIFNEQ ?substr_nul GF@temp0 bool@true\n");


    printf("SUB LF@amn LF@j LF@i\n");

    printf("MOVE GF@temp2 string@\n");

    printf("LABEL ?substr_while\n");
    printf("JUMPIFEQ ?substr_whileend LF@amn int@0\n");

    printf("SUB LF@amn LF@amn int@1\n");
    printf("ADD GF@temp0 LF@amn LF@i\n");
    printf("GETCHAR GF@temp1 LF@str GF@temp0\n");
    printf("CONCAT GF@temp2 GF@temp1 GF@temp2\n");
    printf("JUMP ?substr_while\n");

    printf("LABEL ?substr_whileend\n");
    printf("PUSHS GF@temp2\n");
    printf("JUMP ?substr_end\n");
    
    printf("LABEL ?substr_nul\n");
    printf("PUSHS nil@nil\n");
    printf("JUMP ?substr_end\n");

    printf("LABEL ?substr_empty\n");
    printf("PUSHS string@\n");
    printf("JUMP ?substr_end\n");

    printf("LABEL ?substr_end\n");
    printf("POPFRAME\n");
    printf("RETURN\n");
}

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

void generateCall(char *funcname){
    printf("CALL %s\n", funcname);
}

void generateReturn(){
    printf("RETURN\n");
}

// DATAFLOW 
void generateLabel(char *label)
{
    printf("LABEL %s\n", label);
}
void generateLabelEnd(char *label){
    printf("LABEL %s_end\n", label);
}


// UNIQUE LABEL FOR IF-ELSE AND WHILE
char *UniqueLabel(char *labelbefore){
    char *labelafter = (char *) malloc(strlen(labelbefore) + sizeof(labelcnt) + 3);
    sprintf(labelafter, "%s%d", labelbefore, labelcnt);
    labelcnt++;
    return labelafter;
}

// JUMPS
void generateJump(char *label){
    printf("JUMP %s\n", label);
}
void generateJumpEnd(char *label){
    printf("JUMP %s_end\n", label);
}
void generateJumpIfEqs(char *label){
    printf("JUMPIFEQS %s\n",label);
}


void generateJumpIfNEqs(char *label){
    printf("JUMPIFNEQS %s\n",label);
}
void generateExit(char *number){
    printf("EXIT int@%d\n", atoi(number));
}

// MISC
void generateProgramHead(){
    printf(".IFJcode22\n");
    printf("DEFVAR GF@temp0\n");
    printf("DEFVAR GF@temp1\n");
    printf("DEFVAR GF@temp2\n");
    printf("JUMP main\n");
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

void generateDefvar(char *var){
    printf("DEFVAR LF@%s\n", var);
}

void generateCondiCheck(){
    printf("LABEL ??condiCheck\n");
    printf("POPS GF@temp0\n");
    printf("TYPE GF@temp1 GF@temp0\n");
    printf("PUSHS GF@temp0\n");
    // SWITCH TEMP_1

    // INT -> INT@0 ---> EQS, NOTS

    printf("JUMPIFNEQ ??condiFloat GF@temp1 string@int\n");
    printf("PUSHS int@0\n");
    printf("EQS\n");
    printf("NOTS\n");
    printf("JUMP ??EndcondiCheck\n");
    // FLOAT -> FLOAT@0.0 EQS NOTS
    printf("LABEL ??condiFloat\n");
    printf("JUMPIFNEQ ??condiString GF@temp1 string@float\n");
    printf("PUSHS float@0x0.0p+0\n");
    printf("EQS\n");
    printf("NOTS\n");
    printf("JUMP ??EndcondiCheck\n");
    
    // STRING -> STRING@ -> EQS NOTS 
    printf("LABEL ??condiString\n");
    printf("JUMPIFNEQ ??condiNil GF@temp1 string@string\n");
    printf("PUSHS string@\n");
    printf("EQS\n");
    printf("NOTS\n");
    printf("JUMP ??EndcondiCheck\n");

    //NIL
    printf("LABEL ??condiNil\n");
    printf("JUMPIFNEQ ??EndcondiCheck GF@temp1 string@nil\n");
    printf("CLEARS\n");
    printf("PUSHS bool@false\n");
    // BOOL --> ret
    printf("LABEL ??EndcondiCheck\n");
    printf("RETURN\n");

}

void generateConditions(char *label){
    printf("CALL ??condiCheck\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS %s\n",label);
}
void generateConditionsW(char *label){
    printf("CALL ??condiCheck\n");
    printf("PUSHS bool@true\n");
    printf("JUMPIFNEQS %s_end\n",label);
}
void generateLocDefVar(stat_t *data){
    generateDefvar(data->name);
}

void generateMainStart()
{
    printf("# START OF MAIN\n");
    printf("LABEL main\n");
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
                stringBefore[i] <= 32
           ) {
            snprintf(helpString, 5, "\\0%d", stringBefore[i]);
        } else if (stringBefore[i] == 92) { 
            if (stringBefore[i + 1] == 110) {
                // newline
                snprintf(helpString, 5, "\\010");
                // skip next char
                i++;
            } else if (stringBefore[i + 1] == 116) {
                // tab
                snprintf(helpString, 5, "\\009");
                // skip next char
                i++;
            } else {
                snprintf(helpString, 5, "\\0%d", stringBefore[++i]);
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

void insertFunctionCall(instructList_T *iList, char* funName)
{
    if (strcmp(funName, "write")){
        // I have gotten something thats not write
        insertInstruction(iList, CALL_I, funName, NULL, NULL);
        return;
    }

    // I have gotten the write call, so I must flip the argx
    // so I do not print in reverse order.
    instructList_T *tmp = malloc (sizeof(instructList_T));
    initInstList(tmp);
    
    // PUSHS nil@nil is the "last" arg
    while(iList->lastElement->operation != PUSHS_NIL_I){
        // I save the instructions in different stack
        insertInstructionT(tmp, popInstruction(iList));
    } 

    // Push the instructions back on stack in reverse order
    instructElem ptr = tmp->firstElement;
    while (ptr){
        insertInstruction(iList, ptr->operation, ptr->op1, ptr->op2, ptr->dest);
        ptr = ptr->next;
    }

    // All good, we can finally call write
    insertInstruction(iList, CALL_I, "write", NULL, NULL);
    // Not leaving garbage behind
    while(tmp->lastElement){
        free(popInstruction(tmp));
    }
    free(tmp);
}

void insertArg(stat_t **arr, size_t arrSize, stat_t *stat)
{
    for(size_t i = 0; i < arrSize; i++){
        if (!arr[i]) {
            arr[i] = stat;
            return;
        }
        if (arr[i]->index < stat->index){
            for (size_t j = arrSize; j --> i;){
                arr[j] = arr[j-1];
            }
            arr[i] = stat;
            return;
        }
    }
}

void popInstructions(htab_t *htab)
{
    // get arr for all args
    size_t arrSize = htab->size;
    stat_t **argArr = calloc(arrSize, sizeof(stat_t*));

    // For each arg in htab
    for(int i = 0; i < htab->arr_size; i++){
        if (htab->arr_ptr[i] == NULL){
            continue;
        }

        htab_item_t *current_ptr = htab->arr_ptr[i];

        // insert it sorted into the arr
        while(current_ptr){
            insertArg(argArr, arrSize, current_ptr->statement);
            current_ptr = current_ptr->next;
        }
    }
    
    // pop all items from back to front
    // tim --> zdravim pana doktora Smrcku o/
    for (size_t i = arrSize; i --> 0;){
        printf("POPS LF@%s\n", argArr[i]->name);
    }
}



/*****************************Traverse through list of instructions*****************************/
void generatorInit(instructList_T *instrList, htab_list *symList){
    generateProgramHead();
    generateRead();
    generateWrite();
    generateInt2Floats();
    generateFloat2Ints();
    generateStrval();
    generateOrd();
    generateChr();
    generateStrlen();
    generateCondiCheck();
    generateSubstr();
    generateMainStart();
    First(instrList);
    while(instrList->activeElement != NULL){
        switch(instrList->activeElement->operation){
            case MOVEI2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVEF2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVES2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVENIL2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVELF2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVEI2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVEF2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVES2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVENIL2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVETF2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVELF2LF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
                break;

            case MOVETF2TF_I:
                generateMove(instrList->activeElement->dest,instrList->activeElement->op1, instrList->activeElement->operation);
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
                    // We have to start from index 1, because on index 0 is funTable
                    htab_for_each(symList->activeElement, generateLocDefVar);
                }
                if(strcmp(instrList->activeElement->dest, "main")) 
                    popInstructions(symList->activeElement);
                break;

            case CALL_I:
                generateCall(instrList->activeElement->op1);
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
                callInt2Floats();
                break;

            case FLOAT2INTS_I:
                callFloat2Ints();
                break;

            case WRITE_I:
                generateWriteCall();
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
            case LABEL_END_I:
                generateLabelEnd(instrList->activeElement->dest);
                break;
            case FUNC_S_I:
                printf("# ----FUNCTION START\n");
                generateJumpEnd(instrList->activeElement->dest);
                generateFuncStart(instrList->activeElement->dest);
                break;
            
            case FUNC_E_I:
                generateFunctionEnd();
                generateLabelEnd(instrList->activeElement->dest);
                printf("# ----FUNCTION END\n");
                break;

            case JUMP_I:
                generateJump(instrList->activeElement->dest);
                break;
            
            case JUMP_END_I:
                generateJumpEnd(instrList->activeElement->dest);
                break;

            case JUMPIFEQS_I:
                generateJumpIfEqs(instrList->activeElement->dest);
                break;

            case JUMPIFNEQS_I:
                generateJumpIfNEqs(instrList->activeElement->dest);  
                break;

            case GENERATE_IF_I:
                generateConditions(instrList->activeElement->dest);
                break;

            case GENERATE_WHILE_I:
                generateConditionsW(instrList->activeElement->dest);
                break;

            case EXIT_I:
                generateExit(instrList->activeElement->op1);
                break;

            default: 
                break;
        }
        Next(instrList);
    }
    if(instrList->activeElement == NULL){
        Dispose(instrList);
    }
    generateMainEnd();
}
