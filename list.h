#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
 
SplitList makeSplitList();
int isEmptyList(const SplitList*);
void insertLineToTail(SplitList*, SplitLine*);
void freeList(SplitList*);
SymbolList makeSymbolList();
int isEmptysList(const SymbolList*);
void insertSymbolToTail(SymbolList*, SymbolTable*);
SymbolTable* searchSymbol(SymbolTable*, char*);
