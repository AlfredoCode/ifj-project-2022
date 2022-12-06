[1mdiff --git a/Makefile b/Makefile[m
[1mindex 4c0e9fe..aa51da7 100644[m
[1m--- a/Makefile[m
[1m+++ b/Makefile[m
[36m@@ -11,16 +11,22 @@[m [mpackageName = xmoskv00[m
 CC = gcc[m
 CFLAGS=-std=c99 -Wall -Wextra -pedantic -g[m
 [m
[31m-all: expr_stack.o expr_parser.o  error.o scanner.o generator.o parser.o symtable.o  main.o[m
[32m+[m[32mall: expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o generator.o[m
 	$(CC) $(CFLAGS) $^ -o ifj22 [m
 [m
 pack:[m
 	tar -cvzf $(packageName).tgz *.c *.h Makefile rozdeleni dokumentace.pdf[m
 [m
[32m+[m
 # TESTS[m
 test: $(testFile).o  expr_stack.o expr_parser.o  error.o scanner.o parser.o symtable.o  main.o[m
 	$(CC) $(CFLAGS) $^ -o $@ [m
 [m
[32m+[m
[32m+[m[32mtest: $(testFile).o symtable.o expr_stack.o error.o expr_parser.o scanner.o parser.o[m
[32m+[m
[32m+[m[32mgen: generator.o generator[m
[32m+[m
 test_run: test[m
 	./test 2>/dev/null[m
 [m
[36m@@ -46,6 +52,9 @@[m [mgenerator.o: generator.c[m
 [m
 main.o: main.c[m
 [m
[32m+[m[32mgenerator.o: generator.c[m
[32m+[m
[32m+[m
 # CLEANUP[m
 clean:[m
 	@rm -f $(testFile) *.o[m
[1mdiff --git a/expr_parser.c b/expr_parser.c[m
[1mindex 5a220c5..74f21c6 100644[m
[1m--- a/expr_parser.c[m
[1m+++ b/expr_parser.c[m
[36m@@ -33,7 +33,7 @@[m [mconst char prec_table [19][19] = {[m
 /* !== */{ '<', '<', '<', '<', '<', 'X', 'X', 'X', 'X', 'X', 'X', '<', '>', '<', '<', '<', '<', '<', '>'},[m
 /*  (  */{ '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', 'X'},[m
 /*  )  */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
[31m-/*  ID */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
[32m+[m[32m/*  ID */{ '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
 /* INT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
 /* FLT */{ '>', '>', '>', '>', 'X', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
 /* STR */{ 'X', 'X', 'X', 'X', '>', '>', '>', '>', '>', '>', '>', 'X', '>', 'X', 'X', 'X', 'X', 'X', '>'},[m
[36m@@ -89,7 +89,6 @@[m [mp_symbol tokenToTerminal(token_t *token)[m
                 return sym_null;[m
                 break;[m
             }[m
[31m-        // if not null[m
         // fallthru [m
 [m
         default:[m
[36m@@ -238,7 +237,7 @@[m [mint evaluate_brackets(stack_t *stack)[m
     return 0;[m
 }[m
 [m
[31m-int evaluate_concatenation(stack_t *stack)[m
[32m+[m[32mint evaluate_concatenation(stack_t *stack, instructList_T *iList)[m
 {[m
     stack_token_t *tok1 = stackPeek(stack, 0);[m
     stack_token_t *tok2 = stackPeek(stack, 2);[m
[36m@@ -266,7 +265,7 @@[m [mint evaluate_concatenation(stack_t *stack)[m
     return 0;[m
 }[m
 [m
[31m-int evaluate(stack_t *stack, htab_t *symtable)[m
[32m+[m[32mint evaluate(stack_t *stack, htab_t *symtable, instructList_T *iList)[m
 {   [m
     stack_token_t *top = nextNonTerm(stack);[m
     stat_t *id;[m
[36m@@ -282,7 +281,9 @@[m [mint evaluate(stack_t *stack, htab_t *symtable)[m
                errHandler(SEM_UNDEF_VAR_ERR, "Undefined variable in expression!");[m
                return 1;[m
             }[m
[31m-            [m
[32m+[m
[32m+[m[32m            insertInstruction(iList, PUSHS_ID_I, top->token->string, NULL, NULL);[m
[32m+[m[41m             [m
             switch (id->type){[m
                 case t_int:[m
                     top->symbol = term_int;[m
[36m@@ -306,39 +307,54 @@[m [mint evaluate(stack_t *stack, htab_t *symtable)[m
         [m
         // sym_int[m
         case sym_int:[m
[32m+[m[32m            insertInstruction(iList, PUSHS_INT_I, top->token->string, NULL, NULL);[m
             top->symbol = term_int;[m
             break;[m
          [m
         // sym_float[m
         case sym_float:[m
[32m+[m[32m            insertInstruction(iList, PUSHS_FLOAT_I, top->token->string, NULL, NULL);[m
             top->symbol = term_float;[m
             break;[m
         [m
         // sym_str[m
         case sym_str:[m
[32m+[m[32m            insertInstruction(iList, PUSHS_STRING_I, top->token->string, NULL, NULL);[m
             top->symbol = term_str;[m
             break;[m
         [m
         // My old friend NULL[m
         case sym_null:[m
[32m+[m[32m            insertInstruction(iList, PUSHS_NIL_I, NULL, NULL, NULL);[m
             top->symbol = term_null;[m
             break;[m
 [m
[31m-        // sym_[add|sub|mul][m
[32m+[m[32m        // sym_[add|sub|mul|div][m
         case sym_add:[m
[32m+[m[32m            insertInstruction(iList, ADDS_I, NULL, NULL, NULL);[m
[32m+[m[32m            arithmetic_check(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_sub:[m
[32m+[m[32m            insertInstruction(iList, SUBS_I, NULL, NULL, NULL);[m
[32m+[m[32m            arithmetic_check(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_mul:[m
[32m+[m[32m            insertInstruction(iList, MULS_I, NULL, NULL, NULL);[m
             arithmetic_check(stack);[m
             break;[m
[31m-        [m
[31m-        // sym_div[m
[32m+[m
         case sym_div:[m
[32m+[m[32m            //TODO DIVS[m
[32m+[m[32m            insertInstruction(iList, IDIVS_I, NULL, NULL, NULL);[m
             arithmetic_check(stack);[m
             break;[m
 [m
         // sym_con[m
         case sym_con:[m
[31m-            evaluate_concatenation(stack);[m
[32m+[m[32m            insertInstruction(iList, CONCAT_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_concatenation(stack, iList);[m
             break;[m
         [m
         // sym_rbr[m
[36m@@ -348,11 +364,37 @@[m [mint evaluate(stack_t *stack, htab_t *symtable)[m
         [m
         // sym_[lt|gt|lte|gte|eq|neq][m
         case sym_lt:[m
[32m+[m[32m            insertInstruction(iList, LTS_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_bool(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_gt:[m
[32m+[m[32m            insertInstruction(iList, GTS_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_bool(stack);[m
[32m+[m[32m            break;[m
[32m+[m[41m            [m
         case sym_lte:[m
[32m+[m[32m            insertInstruction(iList, LTS_I, NULL, NULL, NULL);[m
[32m+[m[32m            insertInstruction(iList, EQS_I, NULL, NULL, NULL);[m
[32m+[m[32m            insertInstruction(iList, ORS_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_bool(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_gte:[m
[32m+[m[32m            insertInstruction(iList, GTS_I, NULL, NULL, NULL);[m
[32m+[m[32m            insertInstruction(iList, EQS_I, NULL, NULL, NULL);[m
[32m+[m[32m            insertInstruction(iList, ORS_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_bool(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_eq:[m
[32m+[m[32m            insertInstruction(iList, EQS_I, NULL, NULL, NULL);[m
[32m+[m[32m            evaluate_bool(stack);[m
[32m+[m[32m            break;[m
[32m+[m
         case sym_neq:[m
[32m+[m[32m            insertInstruction(iList, LTS_I, NULL, NULL, NULL);[m
[32m+[m[32m            insertInstruction(iList, NOTS_I, NULL, NULL, NULL);[m
             evaluate_bool(stack);[m
             break;[m
 [m
[36m@@ -392,7 +434,7 @@[m [mp_return get_last(stack_t *stack)[m
     return ret_uhoh;[m
 }[m
 [m
[31m-p_return expr_parse(htab_t *symtable, expression_T *list)[m
[32m+[m[32mp_return expr_parse(htab_t *symtable, expression_T *list, instructList_T *iList)[m
 {[m
     // Stack init[m
     stack_t *stack = stackInit();[m
[36m@@ -441,7 +483,7 @@[m [mp_return expr_parse(htab_t *symtable, expression_T *list)[m
 [m
             // Reduce expression[m
             case '>':[m
[31m-                evaluate(stack, symtable);[m
[32m+[m[32m                evaluate(stack, symtable, iList);[m
                 break;[m
 [m
             // Non-rule[m
[1mdiff --git a/expr_parser.h b/expr_parser.h[m
[1mindex 69702e8..189c740 100644[m
[1m--- a/expr_parser.h[m
[1m+++ b/expr_parser.h[m
[36m@@ -13,6 +13,7 @@[m
 #include "expr_stack.h"[m
 #include "symtable.h"[m
 #include "parser.h"[m
[32m+[m[32m#include "generator.h"[m
 [m
 typedef enum {[m
     ret_bool,[m
[36m@@ -76,7 +77,7 @@[m [mint evaluate_brackets(stack_t *stack);[m
  * Evaluates if concatenation expression is correct.[m
  * Reduces stack.[m
  */[m
[31m-int evaluate_concatenation(stack_t *stack);[m
[32m+[m[32mint evaluate_concatenation(stack_t *stack, instructList_T *iList);[m
 [m
 /**[m
  * @brief   evaluates expression[m
[36m@@ -87,7 +88,7 @@[m [mint evaluate_concatenation(stack_t *stack);[m
  * Evaluates if expression is correct.[m
  * Reduces stack[m
  */[m
[31m-int evaluate(stack_t *stack, htab_t *symtable);[m
[32m+[m[32mint evaluate(stack_t *stack, htab_t *symtable, instructList_T *iList);[m
 [m
 /**[m
  * @brief   gets type of last token[m
[36m@@ -102,6 +103,6 @@[m [mp_return get_last(stack_t *stack);[m
  * @param   dll of tokens[m
  * @return  final datatype[m
  */[m
[31m-p_return expr_parse(htab_t *symTable, expression_T *list);[m
[32m+[m[32mp_return expr_parse(htab_t *symTable, expression_T *list, instructList_T *iList);[m
 [m
 #endif // __PREC_PARSER_H__[m
[1mdiff --git a/expr_table.txt b/expr_table.txt[m
[1mindex 965d1f4..c937d13 100644[m
[1m--- a/expr_table.txt[m
[1m+++ b/expr_table.txt[m
[36m@@ -1,4 +1,4 @@[m
[31m-:) | +   -   *   /   .   <   >   <=  >=  === !== (   )   ID INT FLT STR NUL  &[m
[32m+[m[32m:) | +   -   *   /   .   <   >  <=  >=  === !==  (   )  ID  INT FLT STR NUL  $[m
 ---+---------------------------------------------------------------------------[m
 +  | >   >   <   <   >   >   >   >   >   >   >   <   >   <   <   <   X   <   >[m
 -  | >   >   <   <   >   >   >   >   >   >   >   <   >   <   <   <   X   <   >[m
[36m@@ -17,4 +17,5 @@[m [mINT| >   >   >   >   X   >   >   >   >   >   >   X   >   X   X   X   X   X   >[m
 FLT| >   >   >   >   X   >   >   >   >   >   >   X   >   X   X   X   X   X   >[m
 STR| X   X   X   X   >   >   >   >   >   >   >   X   >   X   X   X   X   X   >[m
 NUL| >   >   >   >   >   >   >   >   >   >   >   X   >   X   X   X   X   X   >[m
[31m-&  | <   <   <   <   <   <   <   <   <   <   <   <   X   <   <   <   <   <   #[m
[32m+[m[32m$  | <   <   <   <   <   <   <   <   <   <   <   <   X   <   <   <   <   <   #[m
[41m+[m
[1mdiff --git a/generator.c b/generator.c[m
[1mindex 31c1bfe..a9fe6c4 100644[m
[1m--- a/generator.c[m
[1m+++ b/generator.c[m
[36m@@ -24,7 +24,18 @@[m [mvoid initInstList(instructList_T *instrList){[m
 [m
 }[m
 [m
[31m-int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest){ //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser[m
[32m+[m[32mvoid First(instructList_T *instrList ) {[m
[32m+[m	[32minstrList->activeElement = instrList->firstElement;[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid Next(instructList_T *instrList){[m
[32m+[m[32m    if(instrList->activeElement != NULL){[m
[32m+[m[32m        instrList->activeElement= instrList->activeElement->next;[m
[32m+[m[32m    }[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m//Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser[m
[32m+[m[32mint insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest){[m
     instructElem newElement = (instructElem) malloc(sizeof(*newElement));[m
 	if(newElement == NULL){[m
 		return INTERNAL_ERR; // return 99[m
[36m@@ -35,101 +46,327 @@[m [mint insertInstruction(instructList_T *instrList, TYPES operation, char* op1, cha[m
     newElement-> operation = operation;[m
 [m
 [m
[31m-	newElement->next = instrList->firstElement;	// Nastavení dat nového prvku seznamu[m
[31m-	newElement->previous = NULL;[m
[32m+[m	[32mnewElement->previous = instrList->lastElement;	// Nastavení dat nového prvku seznamu[m
[32m+[m	[32mnewElement->next = NULL;[m
 	if(instrList->lastElement != NULL){[m
[31m-		instrList->firstElement->previous = newElement;	[m
[31m-[m
[32m+[m		[32minstrList->lastElement->next = newElement;[m[41m	[m
 	}[m
 	else{[m
[31m-		instrList->lastElement = newElement;		// Navázání prvku do seznamu[m
[32m+[m		[32minstrList->firstElement = newElement;		// Navázání prvku do seznamu[m
 	}[m
[31m-	[m
[31m-	instrList->firstElement = newElement;	[m
[32m+[m	[32minstrList->lastElement = newElement;[m[41m	[m
     return SUCCESS_ERR;[m
 [m
 }[m
[32m+[m[32m// ======================== GENERATION ==========================[m
 /*Generating*/[m
[32m+[m
[32m+[m[32m// BUILT-INS TODO[m
[32m+[m[32mvoid generateRead(char *var, char *type);[m
[32m+[m[32mvoid generateWrite(char *symb);[m
[32m+[m[32mvoid generateStrlen(char *var, char *symb);[m
[32m+[m[32mvoid generateSubstring();[m
[32m+[m[32mvoid generateOrd();[m
[32m+[m[32mvoid generateChr();[m
[32m+[m
[32m+[m[32m// ARITHMETIC STACK[m
[32m+[m[32mvoid generateAdds()[m
[32m+[m[32m{[m
[32m+[m[32m    printf("ADDS\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateSubs()[m
[32m+[m[32m{[m
[32m+[m[32m    printf("SUBS\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateMuls()[m
[32m+[m[32m{[m
[32m+[m[32m    printf("MULS\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateDivs()[m
[32m+[m[32m{[m
[32m+[m[32m    printf("DIVS\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateIDivs()[m
[32m+[m[32m{[m
[32m+[m[32m    printf("IDIVS\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m// RELATION STACK TODO[m
[32m+[m
[32m+[m[32m// BOOL STACK TODO[m
[32m+[m
[32m+[m[32m// CONVERSION TODO[m[41m [m
[32m+[m
[32m+[m[32m// STRING TODO[m
[32m+[m
[32m+[m[32m// STACK TODO[m
[32m+[m
[32m+[m[32m// FRAME TODO[m
[32m+[m
[32m+[m[32m// DATAFLOW TODO[m
[32m+[m
[32m+[m[32m// MISC TODO[m
[32m+[m
 /*Program head*/[m
 void generateProgramHead(){[m
     printf(".IFJcode22\n");[m
 }[m
 [m
[32m+[m[32mvoid generateMove(char *var, char *symb){[m
[32m+[m[32m    printf("MOVE LF@%s\n", var);[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m/*Frames and function calls*/[m
[32m+[m[32mvoid generateCreateFrame(){[m
[32m+[m[32m    printf("CREATEFRAME\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generatePushFrame(){[m
[32m+[m[32m    printf("PUSHFRAME\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generatePopFrame(){[m
[32m+[m[32m    printf("POPFRAME\n");[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateDefvar(char *var){        //jak zjistim jestli ma byt lf, gf, nevbo tf[m
[32m+[m[32m    printf("DEFVAR LF@%s\n", var);[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32mvoid generateCall(char *label){[m
[32m+[m[32m    //fce pro vytvoreni unikatniho lablu[m
[32m+[m[32m    printf("CALL %s\n", label);[m
[32m+[m[32m}[m
[32m+[m
 /*Stack functions*/[m
[31m-//pridat do enumu a prekopat[m
[31m-void generatePushs(char *symb, TYPES type){[m
[32m+[m[32mvoid generatePushs(char *symb, INSTRUCTIONS type){[m
     switch(type){[m
[31m-        case INT_T:[m
[31m-            printf("PUSHS int@%a\n", atof(symb));[m
[32m+[m[32m        case PUSHS_INT_I:[m
[32m+[m[32m            printf("PUSHS int@%d\n", atoi(symb));[m
             break;[m
[31m-        case FLOAT_T:[m
[32m+[m
[32m+[m[32m        case PUSHS_FLOAT_I:[m
             printf("PUSHS float@%a\n", atof(symb));[m
             break;[m
[31m-        case STRING_T:[m
[31m-            //TODO fce na konvert stringu[m
[31m-            printf("PUSHS string@%s\n", symb); //symb zmenit na novy string pak[m
[32m+[m
[32m+[m[32m        case PUSHS_STRING_I:[m
[32m+[m[32m            printf("PUSHS string@%s\n", stringConvertor(symb));[m
[32m+[m[32m            break;[m
[32m+[m
[32m+[m[32m        case PUSHS_ID_I:[m
[32m+[m[32m            printf("PUSHS LF@%s\n", symb);[m
[32m+[m[32m            break;[m
[32m+[m
[32m+[m[32m        case PUSHS_NIL_I:[m
[32m+[m[32m            printf("PUSHS nil@nil\n");[m
             break;[m
[31m-        case ID:[m
[31m-            printf("PUSH LF@%s\n", symb);[m
[31m-        default: break;[m
[31m-    }[m
 [m
[32m+[m[32m        default:[m[41m [m
[32m+[m[32m            break;[m
[32m+[m[32m    }[m
 }[m
 //void generatePops(char *var);[m
 //void generateClears();[m
 [m
 /*String convertor*/[m
 char* stringConvertor(char* stringBefore){[m
[31m-    int stringSizeB = strlen(stringBefore);[m
[31m-    int stringSizeA = stringSizeB;[m
[31m-    char *stringAfter = (char *) malloc(stringSizeB * sizeof(char));[m
[31m-    char *add = (char *) malloc(sizeof(char) * 5);[m
[31m-    int i = 0;[m
[31m-    int k = 0;[m
[31m-    while(stringBefore[i] != '\0'){[m
[31m-        if(!isdigit(stringBefore[i])){[m
[31m-           stringSizeA += 5;[m
[31m-            stringAfter = (char *)realloc(stringAfter, stringSizeA * sizeof(char *));[m
[31m-            if((stringBefore[i] >= 0 && stringBefore[i] <= 32) || (stringBefore[i] == 35) || (stringBefore[i] == 92)){[m
[31m-                if(stringBefore[i] >= 0 && stringBefore[i] <= 9){[m
[31m-                    sprintf(add, "/00%d", stringBefore[i]);[m
[31m-                    strcat(stringAfter, add);[m
[31m-                    k += 4;[m
[31m-                }else{[m
[31m-                    sprintf(add, "/0%d", stringBefore[i]);[m
[31m-                    strcat(stringAfter, add);[m
[31m-                    k += 4;[m
[31m-                } [m
[31m-            }else{[m
[31m-                stringAfter[k] = stringBefore[i];[m
[31m-                k++;[m
[32m+[m[32m    size_t size = 16;[m
[32m+[m[32m    char* retstring = malloc(size);[m
[32m+[m[32m    strcpy(retstring, "");[m
[32m+[m[32m    char* helpString = malloc(5);[m
[32m+[m
[32m+[m[32m    for (size_t i = 0; i < strlen(stringBefore); i++){[m
[32m+[m[32m        if ([m
[32m+[m[32m                stringBefore[i] <= 32 ||[m
[32m+[m[32m                stringBefore[i] == 35[m
[32m+[m[32m           ) {[m
[32m+[m[32m            snprintf(helpString, 5, "\\0%d", stringBefore[i]);[m
[32m+[m[32m        } else if (stringBefore[i] == 92) {[m[41m [m
[32m+[m[32m            if (stringBefore[i + 1] == 110) {[m
[32m+[m[32m                // newline[m
[32m+[m[32m                snprintf(helpString, 5, "\\010");[m
[32m+[m[32m            } else if (stringBefore[i + 1] == 116) {[m
[32m+[m[32m                // tab[m
[32m+[m[32m                snprintf(helpString, 5, "\\009");[m
[32m+[m[32m            } else {[m
[32m+[m[32m                snprintf(helpString, 5, "\\0%d", stringBefore[i]);[m
             }[m
[31m-        }else{[m
[31m-            stringAfter[k] = stringBefore[i];[m
[31m-            k++;[m
[32m+[m[32m        } else {[m
[32m+[m[32m            snprintf(helpString, 5, "%c", stringBefore[i]);[m
         }[m
[31m-        i++;[m
[32m+[m
[32m+[m[32m        if (strlen(helpString) + strlen(retstring) + 1 > size) {[m
[32m+[m[32m            size = size * 2;[m
[32m+[m[32m            retstring = realloc(retstring, size);[m
[32m+[m[32m        }[m
[32m+[m[32m        strcat(retstring, helpString);[m
     }[m
[31m-    return stringAfter;[m
[32m+[m[32m    return retstring;[m
 }[m
[32m+[m[32m/*****************************Traverse through list of instructions*****************************/[m
[32m+[m[32mvoid generatorInit(instructList_T *instrList){[m
[32m+[m[32m    generateProgramHead();[m
[32m+[m[32m    First(instrList);[m
[32m+[m
[32m+[m[32m    while(instrList->activeElement != NULL){[m
[32m+[m[32m        switch(instrList->activeElement->operation){[m
[32m+[m[32m            case MOVE_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case CREATEFRAME_I:[m
[32m+[m[32m                generateCreateFrame();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHFRAME_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case POPFRAME_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case DEFVAR_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case CALL_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case RETURN_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHS_INT_I:[m
[32m+[m[32m                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHS_FLOAT_I:[m
[32m+[m[32m                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHS_STRING_I:[m
[32m+[m[32m                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHS_ID_I:[m
[32m+[m[32m                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case PUSHS_NIL_I:[m
[32m+[m[32m                generatePushs(instrList->activeElement->op1, instrList->activeElement->operation);[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case POPS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case CLEARS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case ADDS_I:[m
[32m+[m[32m                generateAdds();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case SUBS_I:[m
[32m+[m[32m                generateSubs();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case MULS_I:[m
[32m+[m[32m                generateMuls();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case DIVS_I:[m
[32m+[m[32m                generateDivs();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case IDIVS_I:[m
[32m+[m[32m                generateIDivs();[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case LTS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case GTS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case EQS_I:[m
[32m+[m[32m                break;[m
 [m
[32m+[m[32m            case ANDS_I:[m
[32m+[m[32m                break;[m
 [m
[32m+[m[32m            case ORS_I:[m
[32m+[m[32m                break;[m
 [m
[31m-// int main(){[m
[31m-//     instrList  = (instructList_T*)malloc(sizeof(*instrList));[m
[31m-//     if(instrList == NULL){[m
[31m-//         fprintf(stderr,"Malloc failure\n");[m
[31m-//         return INTERNAL_ERR;[m
[31m-//     }[m
[31m-//     initInstList(instrList);[m
[31m-//     insertInstruction(instrList, ADD, "x","y","z");[m
[31m-//     //generateProgramHead();[m
[31m-//     char *string = "\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?";[m
[31m-//     // char *string = "\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?";[m
[31m-//     char *string2 = stringConvertor(string);[m
[31m-//     printf("%s\n", string2);[m
[31m-    [m
[31m-    [m
[32m+[m[32m            case NOTS_I:[m
[32m+[m[32m                break;[m
 [m
[31m-//     return 0;[m
[31m-// }[m
[32m+[m[32m            case INT2FLOATS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case FLOAT2INTS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case INT2CHARS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case STRI2INTS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case READ_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case WRITE_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case CONCAT_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case STRLEN_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case GETCHAR_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case SETCHAR_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case TYPE_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case LABEL_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case JUMP_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case JUMPIFEQS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case JUMPIFNEQS_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            case EXIT_I:[m
[32m+[m[32m                break;[m
[32m+[m
[32m+[m[32m            default:[m[41m [m
[32m+[m[32m                break;[m
[32m+[m[32m        }[m
[32m+[m[32m        Next(instrList);[m
[32m+[m[32m    }[m
[32m+[m[32m}[m
[32m+[m
[32m+[m[32m/*[m
[32m+[m[32mint main(){[m
[32m+[m[32m    instrList  = (instructList_T*)malloc(sizeof(*instrList));[m
[32m+[m[32m    if(instrList == NULL){[m
[32m+[m[32m        fprintf(stderr,"Malloc failure\n");[m
[32m+[m[32m        return INTERNAL_ERR;[m
[32m+[m[32m    }[m
[32m+[m[32m    initInstList(instrList);[m
[32m+[m[32m    insertInstruction(instrList, PUSHS_INT_I, "125",NULL,NULL);[m
[32m+[m[32m    insertInstruction(instrList, PUSHS_FLOAT_I, "1.5",NULL,NULL);[m
[32m+[m[32m    insertInstruction(instrList, PUSHS_ID_I, "x",NULL,NULL);[m
[32m+[m[32m    generatorInit(instrList);[m
[32m+[m[32m    return 0;[m
[32m+[m[32m}[m
[32m+[m[32m*/[m
[1mdiff --git a/generator.h b/generator.h[m
[1mindex 3ebb9e8..0b0f835 100644[m
[1m--- a/generator.h[m
[1m+++ b/generator.h[m
[36m@@ -21,7 +21,11 @@[m [mtypedef enum{[m
     DEFVAR_I,[m
     CALL_I,[m
     RETURN_I,[m
[31m-    PUSHS_I,[m
[32m+[m[32m    PUSHS_INT_I,[m
[32m+[m[32m    PUSHS_FLOAT_I,[m
[32m+[m[32m    PUSHS_STRING_I,[m
[32m+[m[32m    PUSHS_ID_I,[m
[32m+[m[32m    PUSHS_NIL_I,[m
     POPS_I,[m
     CLEARS_I,[m
     ADDS_I,[m
[36m@@ -53,6 +57,7 @@[m [mtypedef enum{[m
     EXIT_I,[m
 }INSTRUCTIONS;[m
 [m
[32m+[m[32m// ============ INSTRUCTION LIST ============[m
 [m
 typedef struct instructionElement{[m
     char *op1;[m
[36m@@ -70,7 +75,10 @@[m [mtypedef struct instructionList{[m
 }instructList_T;[m
 [m
 void initInstList(instructList_T *instrList);[m
[31m-int insertInstruction(instructList_T *instrList, TYPES operation, char* op1, char* op2, char* dest); //Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser[m
[32m+[m[32m//Can possibly be type of instructList_T so we can modify the value in parser and in expr_parser[m
[32m+[m[32mint insertInstruction(instructList_T *instrList, INSTRUCTIONS operation, char* op1, char* op2, char* dest);[m[41m [m
[32m+[m
[32m+[m[32m// ============= GENERATION ==================[m
 [m
 void generatorInit(instructList_T *instrList);          //???????????????????[m
 void generateProgramHead();[m
[36m@@ -84,18 +92,13 @@[m [mvoid generateFuncStart(char *funcname);[m
 void generateFunctionEnd();[m
 [m
 /*Built-ins*/[m
[32m+[m[32m// Read[s|i|f], Write, Strlen, Substring, Ord, Chr[m
 void generateRead(char *var, char *type);[m
 void generateWrite(char *symb);[m
 void generateStrlen(char *var, char *symb);[m
[31m-// Generate built-ins[m
[31m-// Reads[m
[31m-// Readi[m
[31m-// Readf[m
[31m-// Write[m
[31m-// Strlen[m
[31m-// Substring[m
[31m-// Ord[m
[31m-// Chr[m
[32m+[m[32mvoid generateSubstring(); // TODO and next ones[m
[32m+[m[32mvoid generateOrd();[m
[32m+[m[32mvoid generateChr();[m
 [m
 /*Aritmetic operations on stack*/[m
 void generateAdds();[m
[36m@@ -130,7 +133,7 @@[m [mvoid generateGetchar(char *dest, char *op1, char *op2);[m
 void generateSetchar(char *dest, char *op1, char *op2);[m
 [m
 /*Stack operations*/[m
[31m-void generatePushs(char *symb, TYPES type);[m
[32m+[m[32mvoid generatePushs(char *symb, INSTRUCTIONS type);[m
 void generatePops(char *var);[m
 void generateClears();[m
 [m
[36m@@ -155,6 +158,12 @@[m [mvoid generateMove(char *var, char *symb);[m
 void generateDefvar(char *var);[m
 [m
 /*String convertor*/[m
[32m+[m
[32m+[m[32m/*[m
[32m+[m[32m * @brief   converts c str to ifjcode22 str[m
[32m+[m[32m * @param   string to convert[m
[32m+[m[32m * @return  converted string[m[41m   [m
[32m+[m[32m */[m
 char* stringConvertor(char* stringBefore);[m
 [m
 // Functions[m
[1mdiff --git a/parser.c b/parser.c[m
[1mindex c5c03ac..bc0dfa6 100644[m
[1m--- a/parser.c[m
[1m+++ b/parser.c[m
[36m@@ -15,7 +15,6 @@[m
 #include "symtable.h"[m
 #include "error.h"[m
 #include "expr_parser.h"[m
[31m-#include "generator.h"[m
 [m
 #define HTABSIZE 10[m
 [m
[36m@@ -33,20 +32,19 @@[m [mtoken_t token;[m
 //Statement struct[m
 stat_t *statement;[m
 [m
[31m-// Instruction list[m
[31m-instructList_T *instrList;[m
[31m-[m
 //Internal variables[m
 int currentToken = 0;[m
 p_return currentReturnType;[m
[31m-[m
 // Expressions[m
 expression_T *expression, *allTokens;[m
[31m-// expr_El expr; // FOR DEBUG PRINTS[m
[32m+[m[32mexpr_El expr; // FOR DEBUG PRINTS[m[41m[m
 [m
 // Symtable[m
 htab_t *symtable, *funTable;[m
 [m
[32m+[m[32m// Instruction list for generation[m[41m[m
[32m+[m[32minstructList_T *iList;[m[41m[m
[32m+[m[41m[m
 [m
 void expressionInit(expression_T *exprList){[m
     exprList->firstElement = NULL;[m
[36m@@ -56,7 +54,7 @@[m [mvoid expressionInit(expression_T *exprList){[m
 [m
 void insertExpr(expression_T *exprList, token_t *token){[m
 [m
[31m-    expr_El newElement = (expr_El ) malloc(sizeof(newElement));[m
[32m+[m[32m    expr_El newElement = (expr_El) malloc(sizeof(expr_El));[m[41m[m
 	if(newElement == NULL){[m
 		return; // return 99[m
 	}[m
[36m@@ -89,7 +87,7 @@[m [mvoid exprListDispose( expression_T *exprList ) {[m
 	firstEl = exprList->firstElement;[m
 	while(firstEl != NULL){[m
 		nextEl= firstEl->next; 	// Cyklus, který maže vždy první prvek seznamu, dokud tam nějaký prvek je[m
[31m-        free(firstEl->token->string);[m
[32m+[m[32m        //free(firstEl->token->string);[m[41m[m
         free(firstEl->token);[m
 		free(firstEl);[m
 		firstEl = nextEl;				// Rozhodně by to šlo vyřešit i jinak, nicméně tato možnost se mi jevila jako nejpříjemnější[m
[36m@@ -107,55 +105,71 @@[m [mint declareCheck(){[m
     // SEMANTIC  - Podívat se do tabulky symbolů, zda je ID rovno 'declare'[m
 token_res = GetToken(&token);       // levá závorka[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR, "Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_PAR){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m
[32m+[m[41m        [m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
 [m
     token_res = GetToken(&token);   // strict_types[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != ID){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU ID[m
 [m
 [m
     token_res = GetToken(&token);   // rovná se[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != ASSIG){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m
[32m+[m[41m        [m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
 [m
 [m
     token_res = GetToken(&token);   // int[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != INT_T){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m
[32m+[m[32m        // printf("SUP");[m[41m[m
[32m+[m[41m[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     // SEMANTIC - MRKNI DO SYMTABLE NA HODNOTU INT[m
 [m
 [m
     token_res = GetToken(&token);   // pravá závorka[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != R_PAR){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> WRONG DECLARE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
 [m
     token_res = GetToken(&token);   // středník[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != SEMICOL){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING SEMICOL <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> MISSING SEMICOL <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     return SUCCESS_ERR;[m
 }[m
[36m@@ -164,20 +178,17 @@[m [mtoken_res = GetToken(&token);       // levá závorka[m
 [m
 int parse(){[m
 [m
[32m+[m[32m    iList = malloc(sizeof(instructList_T));[m[41m[m
[32m+[m[32m    initInstList(iList);[m[41m[m
     int res = SUCCESS_ERR;[m
 [m
     expression  = (expression_T*)malloc(sizeof(*expression));[m
     if(expression == NULL){[m
[31m-        errHandler(INTERNAL_ERR,"Malloc failure\n");[m
[32m+[m[32m        fprintf(stderr,"Malloc failure\n");[m[41m[m
[32m+[m[32m        return INTERNAL_ERR;[m[41m[m
     }[m
     expressionInit(expression);[m
 [m
[31m-    instrList  = (instructList_T*)malloc(sizeof(*instrList));[m
[31m-    if(instrList == NULL){[m
[31m-        errHandler(INTERNAL_ERR,"Malloc failure\n");[m
[31m-    }[m
[31m-    initInstList(instrList);[m
[31m-[m
     symtable = htab_init(HTABSIZE); // Var symtable[m
     if(symtable == NULL){[m
         return INTERNAL_ERR;[m
[36m@@ -201,19 +212,26 @@[m [mint parse(){[m
 [m
     if(GetProlog()){    // modif was not here before[m
         res = prog();[m
[32m+[m[32m        generatorInit(iList);[m[41m  [m
         return res;[m
     }[m
     token_res = GetToken(&token);   [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        // printf("AHoj\n");[m[41m[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
 [m
     switch(token.type){[m
         case EOF_T:[m
[31m-            errHandler(SYNTAX_ERR,"Syntax error ---> EMPTY FILE <---\n");[m
[31m-            break;[m
[32m+[m[32m            fprintf(stderr,"Syntax error ---> EMPTY FILE <---\n");[m[41m[m
[32m+[m[32m            return SYNTAX_ERR;  // EMPTY FILE[m[41m [m
[32m+[m[32m        // case PHP:[m[41m[m
[32m+[m[32m        //     res = prog(); // First rule of LL[m[41m[m
[32m+[m[32m        //     return res;[m[41m[m
         default:[m
[31m-            errHandler(LEX_ERR,"Syntax error ---> UNKNOWN BEFORE PROLOG <---\n");[m
[32m+[m[32m            fprintf(stderr,"Syntax error ---> UNKNOWN BEFORE PROLOG <---\n");[m[41m[m
[32m+[m[32m            return LEX_ERR;  // MISSING PROLOG[m[41m[m
     }[m
     //GENERATE[m
     return SUCCESS_ERR;[m
[36m@@ -223,7 +241,8 @@[m [mint prog(){[m
     int res;[m
     token_res = GetToken(&token);   [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     switch(token.type){[m
         case ID:[m
[36m@@ -231,10 +250,11 @@[m [mint prog(){[m
             if(res == SUCCESS_ERR){[m
                 res = statement_list(symtable);[m
             }[m
[32m+[m[32m            // printf("%d\n",res); //DEBUG[m[41m[m
             return res; [m
         default:[m
[31m-            errHandler(SYNTAX_ERR,"Syntax error ---> MISSING DECLARE <---\n");[m
[31m-            return SYNTAX_ERR;[m
[32m+[m[32m            fprintf(stderr,"Syntax error ---> MISSING DECLARE <---\n");[m[41m[m
[32m+[m[32m            return SYNTAX_ERR;[m[41m   [m
     }[m
 }[m
 [m
[36m@@ -245,14 +265,16 @@[m [mint statement_list(htab_t *localTable){[m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
         if(insideIf || insideWhile || insideFunc){[m
[31m-            errHandler(SYNTAX_ERR,"Syntax error ---> EOF inside If statement <---\n");     // DONT ASK ME[m
[32m+[m[32m            fprintf(stderr,"Syntax error ---> EOF inside If statement <---\n");     // DONT ASK ME[m[41m[m
[32m+[m[32m            return SYNTAX_ERR;[m[41m[m
         }[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     token_t *expr_tok;[m
[31m-    stat_t *funcName;[m
     switch(token.type){[m
         case EOF_T: // FOUND ?>     [m
[32m+[m[32m            // printf("Found the end\n");   //DEBUG[m[41m[m
 [m
             free(expression);[m
             free(allTokens);[m
[36m@@ -270,7 +292,8 @@[m [mint statement_list(htab_t *localTable){[m
                     return res;[m
                 case FUNCTION:[m
                     if(insideFunc){ // Vnořené funkce nechceme[m
[31m-                        errHandler(SYNTAX_ERR,"Correct, we do not want nested functions\n"); //TODO WHICH ERROR IS THIS?[m
[32m+[m[32m                        fprintf(stderr,"Correct, we do not want nested functions\n"); //TODO WHICH ERROR IS THIS?[m[41m[m
[32m+[m[32m                        return SYNTAX_ERR;[m[41m[m
                     }[m
                     res = functionCheck();[m
                     return res;[m
[36m@@ -286,28 +309,29 @@[m [mint statement_list(htab_t *localTable){[m
                     return SYNTAX_ERR;[m
             }[m
         case ID:[m
[31m-           [m
[31m-            funcName = htab_find(funTable, token.string);   // Was function defined before??[m
[31m-            if(funcName == NULL){[m
[31m-                errHandler(SEM_FUNC_ERR, "Semantic Error ---> FUNCTION NOT DEFINED <---\n");[m
[31m-            }[m
[32m+[m[41m            [m
             token_res = GetToken(&token);  [m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             if(token.type == L_PAR){[m
                 res = builtinParams();[m
                 if(res != SUCCESS_ERR){[m
[32m+[m[41m                     [m
                     return SYNTAX_ERR;  // ADD SEMICOL TO LL ON ITS OWN!!![m
                 }[m
                 [m
[32m+[m[32m                // semicol();[m[41m[m
                 token_res = GetToken(&token);  [m
                 // printf("Token type is %d\n", token.type); // DEBUG [m
                 if(!token_res){[m
[31m-                    errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                    fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                    return LEX_ERR;[m[41m[m
                 }[m
                 if(token.type != SEMICOL){[m
[31m-                    errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER <---\n");[m
[32m+[m[32m                    fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER <---\n");[m[41m[m
[32m+[m[32m                    return SYNTAX_ERR;[m[41m[m
                 }[m
                 res = statement_list(localTable);[m
                 if(res != SUCCESS_ERR){[m
[36m@@ -343,7 +367,8 @@[m [mint builtinParams(){[m
     static bool multipleParams = false;[m
     token_res = GetToken(&token);   // bar( <PARAMS> )[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     switch(token.type){[m
         case R_PAR: // NO PARAM [m
[36m@@ -355,17 +380,20 @@[m [mint builtinParams(){[m
         case DOLLAR:[m
             token_res = GetToken(&token);   // type $ <ID>[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             if(token.type != ID){[m
[31m-                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER $ BUILTIN <---\n");[m
[32m+[m[32m                fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER AFTER $ BUILTIN <---\n");[m[41m[m
[32m+[m[32m                return SYNTAX_ERR;[m[41m[m
             }[m
             // SEMANTIC[m
            [m
 [m
             token_res = GetToken(&token);   // type <ID>,   OR type <ID>)[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             switch(token.type){[m
                 case R_PAR:[m
[36m@@ -381,7 +409,8 @@[m [mint builtinParams(){[m
             [m
             token_res = GetToken(&token);   // type <ID>,   OR type <ID>)[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             switch(token.type){[m
                 case R_PAR:[m
[36m@@ -405,10 +434,12 @@[m [mint checkWhile(htab_t *localTable){[m
     token_res = GetToken(&token);  [m
     token_t *expr_tok;[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_PAR){                   // Kontrola ([m
[31m-        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");[m[41m[m
[32m+[m[32m        return res;[m[41m[m
     }[m
     expr_tok = (token_t*) malloc(sizeof(*expr_tok));[m
     if(expr_tok == NULL){[m
[36m@@ -430,10 +461,12 @@[m [mint checkWhile(htab_t *localTable){[m
     [m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_BRAC){               // Kontrola {[m
[31m-        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     expr_tok = (token_t*) malloc(sizeof(*expr_tok));[m
     if(expr_tok == NULL){[m
[36m@@ -441,7 +474,7 @@[m [mint checkWhile(htab_t *localTable){[m
     }[m
     *expr_tok = token;[m
     insertExpr(expression, expr_tok);[m
[31m-    expr_parse(localTable, expression); // modif[m
[32m+[m[32m    expr_parse(localTable, expression, iList); // modif[m[41m[m
     exprListDispose(expression);[m
     insideWhile = true;[m
     res = statement_list(localTable);  // Kontrola vnitřku funkce[m
[36m@@ -471,15 +504,18 @@[m [mint functionCheck(){[m
 [m
     token_res = GetToken(&token);   // function <ID>[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != ID){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> FUNCTION IS MISSING IDENTIFIER <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> FUNCTION IS MISSING IDENTIFIER <---\n");[m[41m[m
[32m+[m[32m        return res;[m[41m[m
     }[m
     // INSERT ID INTO FUNCTION HTAB[m
     statementFun = htab_find(funTable, token.string);[m
     if(statementFun != NULL){[m
[31m-        errHandler(SEM_FUNC_ERR,"SEMANTIC ERROR ---> Function redefinition <---\n");[m
[32m+[m[32m        fprintf(stderr,"SEMANTIC ERROR ---> Function redefinition <---\n");[m[41m[m
[32m+[m[32m        return SEM_FUNC_ERR;[m[41m[m
     }[m
     statementFun = htab_lookup_add(funTable, token.string);   // add  func identifier to symtable[m
 [m
[36m@@ -495,32 +531,39 @@[m [mint functionCheck(){[m
     // -----------------------[m
     token_res = GetToken(&token);   // function <ID> ([m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_PAR){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING LEFT PARENTHESIS IN FUNCTION <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> MISSING LEFT PARENTHESIS IN FUNCTION <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     res = funcParams(localTable, statement); // Parametry funkce[m
     if(res != SUCCESS_ERR){[m
[31m-        errHandler(res, "Syntax error ---> Wrong parameters in function <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> Wrong parameters in function <---\n");[m[41m[m
[32m+[m[32m        return res;[m[41m[m
     }[m
 [m
     token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ):[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != COL){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING COLON IN FUNCTION <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> MISSING COLON IN FUNCTION <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
 [m
     if(token.type != KEYWORD){  // OR IT CAN BE TYPE TOKEN[m
         token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type[m
         if(!token_res){[m
[31m-            errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m            fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m            return LEX_ERR;[m[41m[m
         }[m
         if(token.type != KEYWORD){[m
             return SYNTAX_ERR;[m
[36m@@ -550,17 +593,16 @@[m [mint functionCheck(){[m
     //  int currentReturnType if token.type == TYPE TODO[m
     token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type {[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_BRAC){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING LEFT BRACKET IN FUNCTION <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> MISSING LEFT BRACKET IN FUNCTION <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     insideFunc = true;[m
     res = statement_list(localTable); // function <ID> ( <FUNC_PARAMS> ): type{ <ST_L>[m
     if(res != SUCCESS_ERR){[m
[31m-        if(res == SEM_FUNC_ERR){[m
[31m-            return res;[m
[31m-        }[m
         if(!(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string)){  // VOID FUNCTION WITHOUT RETURN STATEMENT[m
             return SUCCESS_ERR;[m
         }[m
[36m@@ -571,7 +613,8 @@[m [mint functionCheck(){[m
     }[m
     token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): type { <ST_L> return <>[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string){[m
         token_t *expr_tok;[m
[36m@@ -579,10 +622,12 @@[m [mint functionCheck(){[m
             case DOLLAR:[m
                 token_res = GetToken(&token);   // looking for ID[m
                 if(!token_res){[m
[31m-                    errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                    fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                    return LEX_ERR;[m[41m[m
                 }[m
                 if(token.type != ID){[m
[31m-                    errHandler(SYNTAX_ERR, "Syntax error ---> MISSING RETURN STATEMENT <---\n");   [m
[32m+[m[32m                    fprintf(stderr, "Syntax error ---> MISSING RETURN STATEMENT <---\n");[m[41m   [m
[32m+[m[32m                    return SYNTAX_ERR;[m[41m[m
                 }[m
                 expr_tok = (token_t*) malloc(sizeof(*expr_tok));[m
                 if(expr_tok == NULL){[m
[36m@@ -592,7 +637,8 @@[m [mint functionCheck(){[m
                 insertExpr(expression, expr_tok);[m
                 token_res = GetToken(&token);   // Looking for semicol;[m
                 if(!token_res){[m
[31m-                    errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                    fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                    return LEX_ERR;[m[41m[m
                 }[m
                 break;[m
             case INT_T: case FLOAT_T: case STRING_T:    // Teoreticky muzu vratit 5+5 apod, ale to ted neresme[m
[36m@@ -604,26 +650,30 @@[m [mint functionCheck(){[m
                 insertExpr(expression, expr_tok);[m
                 token_res = GetToken(&token);   // Looking for semicol;[m
                 if(!token_res){[m
[31m-                    errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                    fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                    return LEX_ERR;[m[41m[m
                 }[m
                 break;[m
             case KEYWORD:[m
                 if(token.keyword == NULL_K){[m
[31m-                    errHandler(SEM_PARAM_ERR, "null as a return type of function\n");[m
[32m+[m[32m                    fprintf(stderr, "null as a return type of function\n");[m[41m[m
[32m+[m[32m                    return SEM_PARAM_ERR;[m[41m[m
                 }[m
                 else{[m
[31m-                    errHandler(SYNTAX_ERR, "other keyword as a return type of function\n");[m
[32m+[m[32m                    fprintf(stderr, "other keyword as a return type of function\n");[m[41m[m
[32m+[m[32m                    return SYNTAX_ERR;[m[41m  [m
                 }[m
[31m-                break;[m
             default:[m
                 return SEM_RETURN_ERR;   // NO RETURN VALUE[m
         }[m
     }[m
     if(token.type != SEMICOL){[m
         if(currentReturnType == ret_float || currentReturnType == ret_int || currentReturnType == ret_string){[m
[31m-            errHandler(SYNTAX_ERR, "Syntax error ---> MISSING SEMICOL AFTER RETURN <---\n");[m
[32m+[m[32m            fprintf(stderr, "Syntax error ---> MISSING SEMICOL AFTER RETURN <---\n");[m[41m[m
[32m+[m[32m            return SYNTAX_ERR;[m[41m[m
         }[m
[31m-        errHandler(SEM_RETURN_ERR, "Syntax error ---> VOID FUNCTION RETURNING VALUE <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> VOID FUNCTION RETURNING VALUE <---\n");[m[41m[m
[32m+[m[32m        return SEM_RETURN_ERR;[m[41m[m
     }[m
     token_t *expr_tok_semicol;[m
     expr_tok_semicol = (token_t*) malloc(sizeof(*expr_tok_semicol));[m
[36m@@ -633,18 +683,24 @@[m [mint functionCheck(){[m
     *expr_tok_semicol = token;[m
     insertExpr(expression, expr_tok_semicol);[m
     if(currentReturnType == ret_string || currentReturnType == ret_int || currentReturnType == ret_float){[m
[31m-        if(expr_parse(localTable, expression) != currentReturnType){   // sending return expression to expr_parser[m
[31m-            errHandler(SEM_PARAM_ERR, "Wrong return type\n");[m
[32m+[m[32m        // statement = htab_find(localTable,statement->name);    // DEBUG[m[41m[m
[32m+[m[32m        //     printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG[m[41m[m
[32m+[m[32m        if(expr_parse(localTable, expression, iList) != currentReturnType){   // sending return expression to expr_parser[m[41m[m
[32m+[m[41m[m
[32m+[m[32m            fprintf(stderr, "Wrong return type\n");[m[41m[m
[32m+[m[32m            return SEM_PARAM_ERR;[m[41m[m
         }[m
         [m
     }[m
     exprListDispose(expression);[m
     token_res = GetToken(&token);   // Looking for R_BRAC;[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != R_BRAC){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> MISSING RIGHT BRACKET AFTER RETURN STATEMENT <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> MISSING RIGHT BRACKET AFTER RETURN STATEMENT <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     // printf("name is %s, value is %s\n",statement->name, statement->value);                      // DEBUG[m
     insideFunc = false;[m
[36m@@ -657,7 +713,8 @@[m [mint funcParams(htab_t *localTable, stat_t *statementIn){[m
     [m
     token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> )[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     switch(token.type){[m
         case R_PAR: // NO PARAM [m
[36m@@ -670,7 +727,8 @@[m [mint funcParams(htab_t *localTable, stat_t *statementIn){[m
             if(token.type == TYPE){[m
                 token_res = GetToken(&token);   // function <ID> ( <FUNC_PARAMS> ): ?type[m
                 if(!token_res){[m
[31m-                    errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                    fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                    return LEX_ERR;[m[41m[m
                 }[m
                 if(token.type != KEYWORD){[m
                     return SYNTAX_ERR;[m
[36m@@ -695,18 +753,22 @@[m [mint funcParams(htab_t *localTable, stat_t *statementIn){[m
             [m
             token_res = GetToken(&token);   // type $[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             if(token.type != DOLLAR){[m
[31m-                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED $ <---\n");[m
[32m+[m[32m                fprintf(stderr, "Syntax error ---> EXPECTED $ <---\n");[m[41m[m
[32m+[m[32m                return SYNTAX_ERR;[m[41m[m
             }[m
 [m
             token_res = GetToken(&token);   // type $ <ID>[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             if(token.type != ID){[m
[31m-                errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER AFTER TYPE IN FUCNTION <---\n");[m
[32m+[m[32m                fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER AFTER TYPE IN FUCNTION <---\n");[m[41m[m
[32m+[m[32m                return SYNTAX_ERR;[m[41m[m
             }[m
             // SEMANTIC[m
             statementIn = htab_lookup_add(localTable, token.string);  // ADD PARAM TO LOCAL VAR TABLE[m
[36m@@ -715,7 +777,8 @@[m [mint funcParams(htab_t *localTable, stat_t *statementIn){[m
             [m
             token_res = GetToken(&token);   // type <ID>,   OR type <ID>)[m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");  // Hledáme ID[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");  // Hledáme ID[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
             }[m
             switch(token.type){[m
                 case R_PAR:[m
[36m@@ -762,7 +825,8 @@[m [mint checkIfStat(htab_t *table){[m
 [m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));[m
     if(expr_tok == NULL){[m
[36m@@ -773,7 +837,9 @@[m [mint checkIfStat(htab_t *table){[m
         case DOLLAR:    // $ <ID> <SEPARATOR>  [m
             token_res = GetToken(&token);  [m
             if(!token_res){[m
[31m-                errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m                fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m                return LEX_ERR;[m[41m[m
[32m+[m[41m            [m
             }[m
             if(token.type != ID){[m
                 return SYNTAX_ERR;[m
[36m@@ -803,6 +869,7 @@[m [mint checkIfStat(htab_t *table){[m
     }[m
     res = checkIfOperators();[m
     if(res == SUCCESS_ERR){[m
[32m+[m[32m        // printf("Found the triple EQ\n");[m[41m[m
         return SUCCESS_ERR;[m
     }[m
     else if(res == LEX_ERR){[m
[36m@@ -811,8 +878,10 @@[m [mint checkIfStat(htab_t *table){[m
     [m
     res = separators_if(table);[m
     if(res != SUCCESS_ERR){[m
[32m+[m[41m        [m
         return res;[m
     }[m
[32m+[m[41m    [m
     return SUCCESS_ERR;[m
 }[m
 [m
[36m@@ -821,7 +890,8 @@[m [mint checkIfOperators(){[m
     int res = SYNTAX_ERR;[m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     token_t *expr_tok = (token_t*) malloc(sizeof(*expr_tok));[m
     if(expr_tok == NULL){[m
[36m@@ -851,10 +921,12 @@[m [mint condiCheck(htab_t *table){[m
         return INTERNAL_ERR;[m
     }[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_PAR){                   // Kontrola ([m
[31m-        errHandler(res,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING LEFT PARENTHESIS <---\n");[m[41m[m
[32m+[m[32m        return res;[m[41m[m
     }[m
     *expr_tok = token;[m
     insertExpr(expression, expr_tok);[m
[36m@@ -862,17 +934,22 @@[m [mint condiCheck(htab_t *table){[m
     if(res != SUCCESS_ERR){[m
         return res;[m
     }[m
[32m+[m[41m    [m
[32m+[m[41m[m
     res = checkIfStat(table);    // Zkontroluj pravou stranu vyrazu v ifu[m
     if(res != SUCCESS_ERR){[m
         return res;[m
     }[m
 [m
[32m+[m[41m[m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_BRAC){               // Kontrola {[m
[31m-        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     token_t *expr_tok2 = (token_t*) malloc(sizeof(*expr_tok2));[m
     if(expr_tok2 == NULL){[m
[36m@@ -886,7 +963,7 @@[m [mint condiCheck(htab_t *table){[m
     //     expr = expr->previous;[m
     // }[m
     // putchar('\n');[m
[31m-    expr_parse(table, expression);[m
[32m+[m[32m    expr_parse(table, expression, iList);[m[41m[m
     [m
     exprListDispose(expression);[m
     insideIf = true;[m
[36m@@ -898,7 +975,8 @@[m [mint condiCheck(htab_t *table){[m
     insideIf = false;[m
     res = elseCheck(table);[m
     if(res != SUCCESS_ERR){[m
[31m-        errHandler(res,"Syntax error ---> WRONG ELSE FORMAT <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> WRONG ELSE FORMAT <---\n");[m[41m[m
[32m+[m[32m        return res;[m[41m[m
     }[m
 [m
     return statement_list(table);[m
[36m@@ -909,18 +987,22 @@[m [mint elseCheck(htab_t *localTable){[m
     int res = SYNTAX_ERR;[m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.keyword != ELSE){               // Kontrola tokenu else[m
[31m-        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING ELSE <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING ELSE <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
 [m
     token_res = GetToken(&token);  [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type != L_BRAC){               // Kontrola {[m
[31m-        errHandler(SYNTAX_ERR,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m
[32m+[m[32m        fprintf(stderr,"Syntax error ---> MISSING LEFT BRACKET <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
     insideIf = true;[m
     res = statement_list(localTable); // Kontrola těla else[m
[36m@@ -938,17 +1020,22 @@[m [mint expression_check(htab_t *table){[m
     token_res = GetToken(&token);  [m
     // printf("Token type is %d\n", token.type); // DEBUG [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(!((token.type == ID) || (token.type == KEYWORD))){[m
[31m-        errHandler(SYNTAX_ERR, "Syntax error ---> EXPECTED IDENTIFIER <---\n");[m
[32m+[m[32m        fprintf(stderr, "Syntax error ---> EXPECTED IDENTIFIER <---\n");[m[41m[m
[32m+[m[32m        return SYNTAX_ERR;[m[41m[m
     }[m
 [m
[32m+[m[41m   [m
     statement = htab_lookup_add(table, token.string);   // add  identifier to symtable[m
 [m
[32m+[m[41m[m
     token_res = GetToken(&token);[m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     if(token.type == ASSIG){[m
         // ZAVOLAT EXPRESSION PARSER[m
[36m@@ -969,19 +1056,25 @@[m [mint expression_check_inside(htab_t *table){[m
     int res = SUCCESS_ERR;[m
 [m
     token_res = GetToken(&token);  [m
[32m+[m[32m    // printf("Token type is %d\n", token.type); // DEBUG[m[41m [m
     if(!token_res){[m
[31m-        errHandler(LEX_ERR,"Lexical error\n");[m
[32m+[m[32m        fprintf(stderr,"Lexical error\n");[m[41m[m
[32m+[m[32m        return LEX_ERR;[m[41m[m
     }[m
     switch(token.type){[m
         case ID:[m
         case INT_T:[m
         case FLOAT_T:[m
         case STRING_T:[m
[32m+[m[41m            [m
             res = separators(table);[m
[32m+[m[41m            [m
             return res;[m
         default:[m
             return SYNTAX_ERR;[m
     }[m
[32m+[m[41m[m
[32m+[m[41m[m
     return res;[m
 }[m
 [m
[36m@@ -992,19 +1085,21 @@[m [mint statement_list_inside(htab_t *table){[m
     token_res = GetToken(&token);  [m
     // printf("Token type is %d\n", t