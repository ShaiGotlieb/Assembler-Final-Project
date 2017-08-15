#ifndef _VALIDATE_H
#define _VALIDATE_H

#define IMMEDIATE_ADRESS 0
#define DIRECT_ADRESS 1
#define MAT_ADRESS 2
#define DIRECT_REG 3
#define MAX_CMD 16
#define MAX_OPERAND 3

char r0[4] = "0000";
char r1[4] = "0001";
char r2[4] = "0010";
char r3[4] = "0011";
char r4[4] = "0100";
char r5[4] = "0101";
char r6[4] = "0110";
char r7[4] = "0111";
 
 
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

#endif
