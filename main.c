#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define tostr(a) #a
#define MAX_CMD 16
#define BASE 4
#define DECIMAL 10

void createFileName(const char*, char);
void firstRun(File*);
void secondRun(File*);
int readline(char*, command, instruction);
void addSymbol(symboltable*, char*, int, boolean, boolean);
void dataencode(char*, char*);
int extentFunc(instruction*);
int cmdsizealloc(command*);
void cmdToCode(command*);
void insertToTable(char*);
char* convert_base4(char*, char*);

char* base4_str = ""; /* for converting decimal number to base 4 */
char* wierdStr[BASE];

File* f;
symboltable* symbolT = (symboltable*)malloc(sizeof(symboltable));
if(!symbolT){
	printf("Error: allocation memmory failed\n");
	exit(0);
}
char* dataT = (char*)malloc(sizeof(char)*10);
if(!dataT){
	printf("Error: allocation memmory failed\n");
	exit(0);
}
int IC = 0;
int DC = 0;
int i, sycount = 0, extentint, L;
char* line;
char filename[30];
int symbolflag = 0;
int linetype;
command tmpcmd;
instruction tmpinst;
void* currcmd;

int main(int argc, char* argv[]){
	for(i = 1; i < argc; i++){
		createFileName(argv[i], fileName);
		f = fopen(tostr(filename), "r");
		if(!f){
			printf("Error: cannot open the file %s\n", filename);
			break;
		}
		else{
			firstRun(f);
			updateDataT();
			rewind(f);
			secondRun(f);
			exportToFiles(symbolT);
		}
	free(symbolT);
	free(dataT);
	}
}

void firstRun(File* f){
	while(feof(f)){
		fgets(line, 80, f);
		linetype = readline(line, tmpcmd, tmpinst);
		switch(linetype){
			case 0:
				break;
			case 1:
				currcmd = tmpcmd;
				break;
			case 2: case 3:
				currcmd = tmpinst;
				break;
		}
		if(currcmd.lableName != NULL)
			symbolflag = 1;
		if(linetype == 2){
			if(symbolflag == 1){
				addSymbol(symbolT, currcmd.lableName, DC, NO, NO);
			}
			dataencode(dataT, currcmd.data);
			break;
		}
		if(linetype == 3){
			extentint = extentFunc(currcmd);
			if(extentint == 1)
				addSymbol(inst.lableName, 0, YES, NO);/* need to be completed!!!!!!*/
				/*enter symbol table*/
				break;
		}
		L = cmdsizealloc(currcmd);
		IC += L;
		break;
	}
	break;
}

void secondRun(f){
	IC = 0;
	while(feof(f)){
		fgets(line, 80, f);
		linetype = readline(line, tmpcmd, tmpinst);
		switch(linetype){
			case 0:
				break;
			case 1:
				currcmd = tmpcmd;
				break;
			case 2: case 3:
				currcmd = tmpinst;
				break;
		}
		if(linetype == 2)
			break;
		if(linetype == 3){
			extentint = extentFunc(currcmd);
			if(extentint == 2){
				/*number 6 in the book*/

				break;
			}
			cmdToCode(currcmd);
			IC += L;
			break;
		}
	}
	break;
}


void createFileName(const char* origName, char* fName)
{
	strcpy(fName, origName);
	strcat(fName, ".as'\0");
}

void addSymbol(symboltable* symbolT, char* lable, int addr, boolean ext, boolean action){
	symboltable* p = symbolT;
	p[sycount]->lablename = lable;
	p[sycount]->address = addr;
	p[sycount]->ext = ext;
	p[sycount]->act = action;
	p = (symboltable*)realloc(p, sizeof(p)+1);
	if(!p){
		printf("Error: allocation memmory failed\n");
		exit(0);
	}
	else{
		symbolT = p;
		sycount++;
		symbolflag = 0;
	}
}

void dataencode(char* dataT, char* data){
	/*number 7 in the book*/
	/*realloc the dataT array*/

}

int extentFunc(instruction* inst, int IC){
    /*returns 1 if is .extern, 2 if is .entry*/
    /*enter symbol to the table if is a symbol, number 11 in the book*/
    if(inst->instract == ".extern")
    {	
    	return 1;
    }
    if(inst->instract == ".entry")
    {
    	return 2;
}


int cmdsizealloc(command* tmpcmd){
	/*calculates L*/
	/*build the encoded first line of the command*/
}

void updateDataT(){
	/*update the data table with the final addresses, address + IC*/
}

void cmdToCode(command* currcmd){
	/*turns the command to code and insert them in the code table*/
	char* cmdArr[MAX_CMD][MAX_CMD] = { {"mov"}, {"cmp"}, {"add"}, {"sub"}, {"not"}, {"clr"}, {"lea"}, {"inc"}, {"dec"}, {"jmp"}, {"bne"}, {"red"}, {"prn"}, {"jsr"}, {"rts"}, {"stop"},
							 {"0000"}, {"0001"}, {"0010"}, {"0011"}, {"0100"}, {"0101"}, {"0110"}, {"0111"}, {"1000"}, {"1001"}, {"1010"}, {"1011"}, {"1100"}, {"1101"}, {"1110"}, 
							 {"1111"} };

    int i, j;
    char* tempStr = "";
	for(i=0, j=0; j < MAX_CMD; j++)
	{						 
	 if(currcmd->opCode == cmdArr[i][j])
	 {
	  i += 1;
	  tempStr = strcat(tempStr, cmdArr[i][j]); /* concatenate the binary "base 4" number of the right command to a temporary string*/
     }
      tempStr = strcat(tempStr, currcmd->srcOp);
      tempStr = strcat(tempStr, currcmd->dstOp);
      if(currcmd->encoding == 0)
      	tempStr = strcat(tempStr, "00");
      	else if(currcmd->encoding == 1)
      		tempStr = strcat(tempStr, "01");
      		else if(currcmd->encoding == 2)
      			tempStr = strcat(tempStr, "10");
      insertToTable(tempStr);
	}
}
void insertToTable(char* str)
{
	static int k = 0;
	char* codeTable[IC+DC] = NULL;
	if(codeTable == NULL)
	    codeTable[k] = str;
		k++;
	else{
		codeTable[k] = str;
	}

/* Function to convert a given decimal number to base 4*/
char* convert_base4(char* inputNum, char* base4_str)
{
int num = atoi(inputNum);
char* result[DECIMAL-1] = '\0';
int index = 9;
while(num > 0)
{
	if(num % BASE != 0)
	{
		result[index] = 1;
	}
	else{
		result[index] = 0;

	}
	index--;
	num /= BASE;
}
result = base4_str;	
return base4_str;
}

/* function to convert base 4 number to "wierd base 4" symbols: 'a','b','c','d'*/
char* convert_wierd4(char* str)
{
	int r, k = BASE;
	char* temp;
	int num = atoi(str);
	int tempNum = num;
	while(tempNum > 0)
	{
		if((tempNum % 10 ) == 0)
			wierdStr = strcat(wierdStr, 'a');
		if((tempNum % 10 ) == 1)
			wierdStr = strcat(wierdStr, 'b');
		if((tempNum % 10 ) == 2)
			wierdStr = strcat(wierdStr, 'c');
		if((tempNum % 10 ) == 3)
			wierdStr = strcat(wierdStr, 'd');
		tempNum /= 10;
	}
	/* reverse the string*/
	for(r = 0; r < BASE; r++)
    {
        temp = wierdStr[k];
        wierdStr[k] = wierdStr[r];
        wierdStr[r] = temp;
        k--;
    }
    return wierdStr;

}	

