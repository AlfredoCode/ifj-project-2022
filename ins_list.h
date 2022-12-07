/*
 * =================================================== *
 * Name:       ins_list.h                              *
 * Authors:    xhofma11                                *
 *             xjadrn03                                *
 *             xsafar27                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#ifndef __INS_LIST_H__
#define __INS_LIST_H__

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
    LABEL_END_I,
    JUMP_I,
    JUMPIFEQS_I,
    JUMP_END_I,
    JUMPIFNEQS_I,
    GENERATE_IF_I,
    GENERATE_WHILE_I,
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

/**
 * @brief   
 * @param
 */
void initInstList(instructList_T *instrList);

/**
 * @brief   
 * @param   
 */
void First(instructList_T *instrList);

/**
 * @brief   
 * @param
 */
void Next(instructList_T *instrList);

/**
 * @brief   
 * @param   
 */
void Dispose(instructList_T *instrList);

/**
 * @brief   
 * @param   
 * @param
 * @param   
 * @param   
 * @param
 * @return  
 */
int insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest); 

/**
 * @brief   inserts instructElem into iList
 * @param   iList to insert to
 * @param   instruction to insert
 *
 * Alternative to insertInstruction
 */
int insertInstructionT(instructList_T *iList, instructElem instruction);

/**
 * @brief   pops last instruction from iList
 * @param   iList to pop from
 * @return  poped instruction
 */
instructElem popInstruction(instructList_T *iList);


#endif
