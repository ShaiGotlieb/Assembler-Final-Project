#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define tostr(a) #a

void createFileName(const char*, char);
void firstRun(File*);
void secondRun(File*);
int readline(char*, command, instruction);
void addSymbol(symboltable*, char*, int, boolean, boolean);
void dataencode(char*, char*);
int extentFunc(instruction*);
int cmdsizealloc(command*);

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

int extentFunc(instruction* inst){
    /*returns 1 if is .extern, 2 if is .entry*/
    /*enter symbol to the table if is a symbol, number 11 in the book*/
}

int cmdsizealloc(command* tmpcmd){
	/*calculates L*/
	/*build the encoded first line of the command*/
}

void updateDataT(){
	/*update the data table with the final addresses, address + IC*/
}

void cmdToCode(command*){
	/*turns the command to code and insert them in the code table*/
}
