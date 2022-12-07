/*
 * =================================================== *
 * Name:       generator.h                             *
 * Authors:    xsafar27                                *
 *             xhofma11                                *
 *             xjadrn03                                *
 * Last modif: 12/07/2022                              *
 * =================================================== *
 */

#include "scanner.h"    // TYPES
#include "symtable.h"
#include "ins_list.h"

#ifndef __GENERATOR_H__
#define __GENERATOR_H__
// BUILT-INS
// Read[s|i|f], Write, Substring, Ord, Chr
void generateRead();
void generateWrite();
void generateSubstring();
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
void generateStrlen();

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
void generateElseEnd(char *label);
void generateJump(char *label);
void generateJumpEnd(char *label);
void generateElseJumpEnd(char *label);
void generateJumpIfEqs(char *label);
void generateJumpIfNEqs(char *label);
void generateExit(char *number);        // 0 <= number <= 49
void generateConditions(char *label);

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

/**
 * @brief   
 * @param   
 * @param   
 */
void generatorInit(instructList_T *instrList, htab_list *symList);


// HELPER FUNCTIONS

/**
 * @brief   
 * @param   
 * @return  
 */
char *UniqueLabel(char *labelbefore);

/**
 * @brief   inserts function call to iList
 * @param   iList to insert to
 * @param   function name
 *
 * Inserts call label to instruction funName
 * If funName is write, it reverses the arguments on stack then
 * calls write.
 */
void insertFunctionCall(instructList_T *iList, char* funName);

/*
 * @brief   converts c str to ifjcode22 str
 * @param   string to convert
 * @return  converted string   
 */
char* stringConvertor(char* stringBefore);
#endif // __GENERATOR_H__

