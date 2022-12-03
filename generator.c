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

/*Stack functions*/
//pridat do enumu a prekopat
void generatePushs(char *symb, TYPES type){
    switch(type){
        case INT_T:
            printf("PUSHS int@%a\n", atof(symb));
            break;
        case FLOAT_T:
            printf("PUSHS float@%a\n", atof(symb));
            break;
        case STRING_T:
            //TODO fce na konvert stringu
            printf("PUSHS string@%s\n", symb); //symb zmenit na novy string pak
            break;
        case ID:
            printf("PUSH LF@%s\n", symb);
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



// int main(){
//     instrList  = (instructList_T*)malloc(sizeof(*instrList));
//     if(instrList == NULL){
//         fprintf(stderr,"Malloc failure\n");
//         return INTERNAL_ERR;
//     }
//     initInstList(instrList);
//     insertInstruction(instrList, ADD, "x","y","z");
//     //generateProgramHead();
//     char *string = "\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?";
//     // char *string = "\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?\\neviem uz .#.\n.\t kde jsou ty otazniky blby co to vypisovalo?";
//     char *string2 = stringConvertor(string);
//     printf("%s\n", string2);
    
    

//     return 0;
// }
