#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#define IMMEDIATE_ADRESS 0
#define DIRECT_ADRESS 1
#define MAT_ADRESS 2
#define DIRECT_REG 3
 
 
int typeAdress(splitLines*, int);
int isNumber(const char*);
int isOpenBracket(char*);
int isLetter(char*);
int isRegister(char*);
int isMatrix(char*);
int validLabel(char*);
int isOpenBracket(char*);
int isCloseBracket(char*);
void breakMat(void);
int isMatrixInputValid(char*, int);
int isMatrixObject(char*);
int validateMatCommandObject(char*);
int validData(char*);
int checkConsecutiveSigns(char*);
int checkConsecutiveCommas(char*);
int validExtern(char*, Symblist*);
int validEntry(char*, Symblist*);
int validString(char*);
int memorySize(splitLines*);
int isVar(char*);
