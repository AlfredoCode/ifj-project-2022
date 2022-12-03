/*
 * =================================================== *
 * Name:       generator.h                             *
 * Authors:    xsafar27                                *
 *             xhofma11                                *
 *             xjadrn03                                *
 * Last modif: 11/28/2022                              *
 * =================================================== *
 */

#include "scanner.h"    // TYPES

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

typedef enum{
    MOVE_I,
    CREATEFRAME_I,
    PUSHFRAME_I,
    POPFRAME_I,
    DEFVAR_I,
    CALL_I,
    RETURN_I,
    PUSHS_INT_I,
    PUSHS_FLOAT_I,
    PUSHS_STRING_I,
    PUSHS_ID_I,
    PUSHS_NIL_I,
    POPS_I,
    CLEARS_I,
    ADDS_I,
    SUBS_I,
    MULS_I,
    DIVS_I,
    IDIVS_I,
    LTS_I,
    GTS_I,
    EQS_I,
    ANDS_I,
    ORS_I,
    NOTS_I,
    INT2FLOATS_I,
    FLOAT2INTS_I,
    INT2CHARS_I,
    STRI2INTS_I,
    READ_I,
    WRITE_I,
    CONCAT_I,
    STRLEN_I,
    GETCHAR_I,
    SETCHAR_I,
    TYPE_I,
    LABEL_I,
    JUMP_I,
    JUMPIFEQS_I,
    JUMPIFNEQS_I,
    EXIT_I,
}INSTRUCTIONS;


typedef struct instructionElement{
    char *op1;
    char *op2;
    char *dest;
    INSTRUCTIONS operation;               
    struct instructionElement *next;
    struct instructionElement *previous;
}*instructElem;

typedef struct instructionList{
    instructElem firstElement;
    instructElem lastElement;
    instructElem activeElement;
}instructList_T;

void initInstList(instructList_T *instrList);
int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest); //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser

void generatorInit(instructList_T *instrList);          //???????????????????
void generateProgramHead();

/*Main function*/
void generateMainStart();
void generateMainEnd();

/*Other functions*/
void generateFuncStart(char *funcname);
void generateFunctionEnd();

/*Built-ins*/
void generateRead(char *var, char *type);
void generateWrite(char *symb);
void generateStrlen(char *var, char *symb);
// Generate built-ins
// Reads
// Readi
// Readf
// Write
// Strlen
// Substring
// Ord
// Chr

/*Aritmetic operations on stack*/
void generateAdds();
void generateSubs();
void generateMuls();
void generateDivs();
void generateIDivs();

/*Relation operations*/
void generateLts();
void generateGts();
void generateEqs();

/*Bool operations*/
void generateAnds();
void generateOrs();
void generateNots();

/*Conversion*/
void generateInt2Floats();
void generateFloat2Ints();
void generateInt2Chars();
void generateStri2Ints();

/*Type of symb stores to var*/
void generateType(char *var, char *symb);

/*String operations*/
void generateConcat(char *dest, char *op1, char *op2);
void generateStrlen(char *dest, char *op);
void generateGetchar(char *dest, char *op1, char *op2);
void generateSetchar(char *dest, char *op1, char *op2);

/*Stack operations*/
void generatePushs(char *symb, INSTRUCTIONS type);
void generatePops(char *var);
void generateClears();

/*Frames and function calls*/
void generateCreateFrame();
void generatePushFrame();
void generatePopFrame();
void generateCall(char *label);
void generateReturn();

/*Dataflow*/
void generateLabel(char *label);
void generateJump(char *label);
void generateJumpIfEqs(char *label);
void generateJumpIfNEqs(char *label);
void generateExit(char *number);        // 0 <= number <= 49

void UniqueLabel(int labelnmbr,char *idk); //co s tim


void generateMove(char *var, char *symb);
void generateDefvar(char *var);

/*String convertor*/
char* stringConvertor(char* stringBefore);

// Functions

// Gen init

// ...

#endif // __GENERATOR_H__
