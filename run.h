#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
#define OPERATIONS_NUM 16
#define DATA 17
#define STRING 18
#define MAT 19
#define ENTRY 20
#define EXTERN 21
 
char* line;
SplitList sList;
SplitLine sLine;
SplitLine* p;
SymbolList symbList;
SymbolTable symbol;
SymbolTable sp;
int L, index;
 
void firstRun(FILE*);
void secondRun(const SplitList*, FILE*, FILE*);
