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
#include "symtable.h"

#ifndef __GENERATOR_H__
#define __GENERATOR_H__

typedef enum{
    MOVEI2TF_I,
    MOVEF2TF_I,
    MOVES2TF_I,
    MOVENIL2TF_I,
    MOVELF2TF_I,
    MOVETF2TF_I,
    MOVEI2LF_I,
    MOVEF2LF_I,
    MOVES2LF_I,
    MOVENIL2LF_I,
    MOVETF2LF_I,
    MOVELF2LF_I,
    CREATEFRAME_I,
    PUSHFRAME_I,
    POPFRAME_I,
    DEFVAR_I,
    DEFVAR_LOC_I,
    FUNC_S_I,
    FUNC_E_I,
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
    READI_I,
    READS_I,
    READF_I,
    WRITE_I,
    ORD_I,
    CONCAT_I,
    STRLENVAR_I,
    STRLENKONST_I,
    GETCHAR_I,
    SETCHAR_I,
    TYPE_I,
    LABEL_I,
    JUMP_I,
    JUMPIFEQS_I,
    JUMPIFNEQS_I,
    EXIT_I,
}INSTRUCTIONS;

// ============ INSTRUCTION LIST ============

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
void First(instructList_T *instrList );
void Next(instructList_T *instrList);
void Dispose(instructList_T *instrList);

//Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser
int insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest); 

// ============= GENERATION ==================
// BUILT-INS
// Read[s|i|f], Write, Substring, Ord, Chr
void generateRead(char *var, INSTRUCTIONS type);
void generateWrite();
void generateSubstring(); // TODO and next ones
void generateOrd();
void generateChr();

// ARITHMETIC ON STACK
// Non-stack are not used
// + - * /
void generateAdds();
void generateSubs();
void generateMuls();
void generateDivs();
void generateIDivs(); // Kinda obsolete?

// RELATION
// < > ===
// <= >= !== need to be generated using these and bool
void generateLts();
void generateGts();
void generateEqs();

// BOOL
// && || ! 
void generateAnds();
void generateOrs();
void generateNots();

// CONVERSIONS
// Only between int and float
// Rests are strval which is not supported
void generateInt2Floats();
void generateFloat2Ints();

// STRING
void generateConcat(); // Made to work like *S functions
void generateStrlen(char *dest, char *op, INSTRUCTIONS type);
void generateGetchar(char *dest, char *op1, char *op2);
void generateSetchar(char *dest, char *op1, char *op2);

// STACK
void generatePushs(char *symb, INSTRUCTIONS type);
void generatePops(char *var);
void generateClears();

// FRAME
void generateCreateFrame();
void generatePushFrame();
void generatePopFrame();
void generateCall(char *label);
void generateReturn();

// DATAFLOW
void generateLabel(char *label);
void generateLabelEnd(char *label);
void generateJump(char *label);
void generateJumpEnd(char *label);
void generateJumpIfEqs(char *label);
void generateJumpIfNEqs(char *label);
void generateExit(char *number);        // 0 <= number <= 49

// MISC
void generateProgramHead();
void generateMove(char *var, char *symb, INSTRUCTIONS type);

void generateDefvar(char *var);
void generateLocDefVar(stat_t *data);
void generateType(char *var, char *symb);
void generateMainStart();
void generateMainEnd();
void generateFuncStart(char *funcname);
void generateFunctionEnd();

// MAIN GENERATE FUNCTION
void generatorInit(instructList_T *instrList, htab_list *symList);


// HELPER FUNCTIONS
char *UniqueLabel(char *labelbefore);

/*
 * @brief   converts c str to ifjcode22 str
 * @param   string to convert
 * @return  converted string   
 */
char* stringConvertor(char* stringBefore);
#endif // __GENERATOR_H__

