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
int isEmpty(char*);
void insertToCode(SplitLine, int*);
char* cmdToCode(char*);


#define MAX_MEMORY 256
#define ONE_BYTE 10

typedef enum {false, true} boolean;

/Symbol Table/
typedef struct SymbolTable
{
	char* label;
	int addr;
	boolean ext;
	boolean ope;
	SymbolTable* next;
}SymbolTable;

typedef struct SymbolList
{
	SymbolTable* head;
	SymbolTable* tail;
}SymbolList;

/Data Table, Data runs with DC/
char DataT[MAX_MEMORY][ONE_BYTE];

/Code Table, Binaric form of the code, receives the Data table at the end of the program, runs with IC/
char Code[MAX_MEMORY][ONE_BYTE];

/SplitLines hold the separate words of the line and validate some of them/
typedef struct SplitLine
{
	char* label;
	char* opCode;
	char* vars[MAX_MEMORY];
	SplitLine* next;
}SplitLine;

typedef struct SplitList
{
	SplitLines* head;
	SplitLines* tail;
}SplitList;

int IC, DC;
