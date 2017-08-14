#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#define OPSIZE 21
 
 
enum states{START, SPACE, COMM_ZERO, COMM_ONE, COMM_TWO_A, COMM_TWO_B, INST_TWO, INST_EXTERN, LABEL, MAKE, END};
 
char* opCodes[OPSIZE] = {"rts", "stop", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                         "mov", "cmp", "add", "sub", "lea", ".data", ".string", ".mat", ".entry", ".extern"};
 
 
void readline(SplitLine*, char*);
char* convertToBinary(char*);
int validOpCode(char*);
int validOperand(int, char*);
void insertToDataT(char, int);
void copyBinarStr(char*, char*);
void createFileName(const char*, char*, char*);
char* convert_base4(char*);
char* convert_wierd4(char*);
char* convertToBinary(char*);
