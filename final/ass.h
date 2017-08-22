#ifndef ASSEMBLER_HEADER_H
#define ASSEMBLER_HEADER_H

#define START_ADD 100
#define FOUR_BASE_SIZE 5
/* validate.h */
#define IMMEDIATE_ADRESS 0
#define DIRECT_ADRESS 1
#define MAT_ADRESS 2
#define DIRECT_REG 3
#define MAX_CMD 16
#define MAX_OPERAND 3
#define MAX_LABEL 30
#define REG_SIZE 2
/* run.h */
#define OPERATIONS_NUM 16
#define DATA 17
#define STRING 18
#define MAT 19
#define ENTRY 20
#define EXTERN 21
/* utils.h */
#define OPSIZE 21
#define MAX_MEMORY 256
#define MAX_LINE 81
#define ONE_BYTE 10
#define LABEL_SIZE 30
#define DECIMAL 10
#define BASE 4

/* list.h */


typedef enum {false, true} boolean;

/*Symbol Table*/
typedef struct SymbolT
{
    char label[30];
    int addr;
    boolean ext;
    boolean ope;
    struct SymbolT* next;
}SymbolT;


/*SplitLine hold the separate words of the line and validate some of them*/
typedef struct SplitLine
{
    char label[30];
    char* opCode;
    char* vars[MAX_MEMORY];
    struct SplitLine* next;
}SplitLine;


FILE* f;
FILE* fas;
FILE* fent;
FILE* fext;
int i, j;
char* object;
char* entry;
char* extrn;
char* tempStr="";
char* addBase4 = NULL;
char* address = NULL;
char base4Code[6];
char wierd4Code[6];
char tmp[FOUR_BASE_SIZE];

char* line;
SplitLine* sHead;
SplitLine* sLine;
SymbolT* syHead;
SymbolT* symbol;
SymbolT* sp;
int L, indx, r = 0;



enum states{START, SPACE, COMM_ZERO, COMM_ONE, COMM_TWO_A, COMM_TWO_B, INST_TWO, INST, LABEL, MAKE, END};


/*Data Table, Data runs with DC*/
char DataT[MAX_MEMORY][ONE_BYTE];

/*Code Table, Binaric form of the code, receives the Data table at the end of the program, runs with IC*/
char Code[MAX_MEMORY][ONE_BYTE];



int IC, DC;
char name[LABEL_SIZE];
char R1[3] = "\0";
char R2[3] = "\0";


char r0[4] = "0000";
char r1[4] = "0001";
char r2[4] = "0010";
char r3[4] = "0011";
char r4[4] = "0100";
char r5[4] = "0101";
char r6[4] = "0110";
char r7[4] = "0111";


/* firstRun: Runs through the file for the first time.
   This method will read a file and call other fuctions in order to parse the file's lines.
   FirstRun will validate words, create list, parse each line and insert data to the right data types.*/ 
SplitLine* firstRun(FILE*, SplitLine*);
/* secondRun: Runs through the file for the second time.
   This method will read a file and call other fuctions in order to parse and finish the program.
   secondRun will create the right tables in order to insert them into the right files.
   It will also call other functions which will convert the code to the right code as mentioned in the instructions*/ 
void secondRun(SplitLine*, FILE*, FILE*);
int countList(SplitLine* curr);
/* typeAdress: getting a structure wich contains data from line that already has been parsed.
    it gets the index of the "vars" array in order to determain the kind of the type address.
    It returns 0 for immidiate address, 1 for direct address, 2 for matrix address, 3 for register address.*/ 
int typeAdress(SplitLine*, int);
/* isNumber: Function that determain if a single char is a number. Number can start with '#' sign, '-' or '+'.
    returns 1 if true and 0 if false.*/
int isNumber(const char*);
/* isOpenBracket: Helping method that decides if a specific character is an open square bracket: '['
    returns 1 if true and 0 if false*/
int isOpenBracket(char);
/* isLetter: Helping method that decides if a specific character is a lligal character.
    returns 1 if true and 0 if false*/
int isLetter(char*);
/* isRegister: Function that check if a given word is a lligal register: r0 - r7
    return 1 if true and 0 if false*/
int isRegister(char*);
/* isMatrix: Function that check if a given word is a lligal matrix: starts has two pairs of square bracket '[]'.
    inside should contain register. return 1 if true and 0 if false */
int isMatrix(char*);
/* validLabel: check if a label is lligal label as asked in the instructions.
    return 1 if true and 0 if false*/
int validLabel(char*);
/* isCloseBracket: Helping method that decides if a specific character is a close square bracket: ']'
    returns 1 if true and 0 if false*/
int isCloseBracket(char);
/* breakMat: Method that will get a word and save the name of the matrix before the brasckets*/
void breakMat(char*);
/* isMatrixInputValid: function that validate the matrix lligalization. 
    return 1 if matrix is lligal and 0 if illigal */
int isMatrixInputValid(char*, int);
/* isMatrixObject: check valditation of a matrix after '.mat' appears. return the size of the matrix*/
int isMatrixObject(char*);
/* validateMatCommandObject: function that call the other functions in order to validate the full matrix line.
    return the size of the matrix after validation. Validate the elemnts of the matrix as well.
    if faild - will print error message*/
int validateMatCommandObject(char*);
/* validData: validate the number and the lligalization of elements in array 'vars' that located inside the structure
    will return the size of elements in the array (data)*/
int validData(char*);
/* checkConsecutiveSigns: Method that check if consecutive signs: '-' , '+'  appears in a line
    return 1 if consecutive signs appears and 0 if not*/
int checkConsecutiveSigns(char*);
/* checkConsecutiveCommas: Method that check if consecutive commas: ',,' appears in a line
    return 1 if consecutive commas appears and 0 if not*/
int checkConsecutiveCommas(char*);
/* validExtern: method that check the validation of variable after '.extern'
    name of a variable can't appear twice. return 1 if true and 0 if false*/
int validExtern(char*, SymbolT*);
/* validEntry: method that check the validation of variable after '.entry'
    name of a variable must exist in symbol list. return 1 if true and 0 if false*/
int validEntry(char*, SymbolT*);
/* validString: method that check the validation of string after '.string'
    return size of a string in order to increase 'DC' and 0 if false*/
int validString(char*);
/* memorySize: function that check how many memory spaces should be added to IC (by L).
    return the size of the memory should be opened*/
int memorySize(SplitLine*);
/* isVar: helping method that check the correction of a word in array 'vars'*/
int isVar(char*);
/* getRegister: function that get a register name and return the code for the right register: 0000 - 1111*/
char* getRegister(char*);
/* parseMat: function that parse the matrix it gets and seperate the name of the matrix, the variables inside.*/
void parseMat(char*);
/* mItoa: function that change the type of a variable from int to char.*/
void mItoa(int, char[]);
/* reverse: helping function that get a word and reverse it*/
void reverse(char[]);

/* list.h */
/* isEmptyList: function that check if a given list is empty
    return 1 if empty and 0 if not empty*/
int isEmptyList(SplitLine*);
/* insertLineToTail: function that adding a structure to the end of the list*/
SplitLine* insertLineToTail(SplitLine*, SplitLine*);
/* freeList: function that realease a list from a memory*/
void freeList(SplitLine*);

void freeSList(SymbolT*);
/* isEmptyList: function that check if a given list is empty (symbol list)
    return 1 if empty and 0 if not empty*/
int isEmptysList(SymbolT*);
/* insertSymbolToTail: function that adding a symbol structure to the end of the list*/
SymbolT* insertSymbolToTail(SymbolT*, SymbolT*);
/* searchSymbol: is getting a symbol table and a word in order to search if it exist
    if exist it will return a pointer to the 'object'*/
SymbolT* searchSymbol(SymbolT*, char*);

SplitLine* newSL();
SymbolT* newSym();

/* readline: function that getting a line in order to read it and parse it.*/
SplitLine* readline(SplitLine*, char*);
/* convertToBinary: method that getting a string and convert it to binary as characters and return it*/
char* convertToBinary(char*);
/* validOpCode: function that check the validation of the command and return the number of the command*/
int validOpCode(char*);
/* validOperand: function that check the correction of the operands using wtich cases.
    if the operand exist - return 1 if not it will return 0*/
int validOperand(int, SplitLine*);
/* insertToDataT: function that adding data to the right table*/
void insertToDataT(char**, int);
/* convert_base4: method that gets a string and converts it to base 4 and return the string */
char* convert_base4(char*);
/* convert_wierd4: method that gets a string and converts it to 'wierd base 4': 'a', 'b', 'c', 'd' and return the string */
char* convert_wierd4(char*);
/* convertToBinary: method that get a string and convert it to binary. returns the result as a char type*/
char* convertToBinary(char*);
/* isEmpty: function that decide if a given strng is empty or not (could be empty line)
    return 1 if empty and 0 if not empty*/
int isEmpty(char*);
/* insertToCode: function that get the right structure and insert it to the array 'Code' according to the right row*/
void insertToCode(SplitLine*, int*);
/* cmdToCode: function that gets the command name and return the number it represents*/
char* cmdToCode(char*);
/* binaryToWierd4: convert a string of characters to 'wierd 4 base'*/
void binaryToWierd4(char*, char*);
/* cleanArr: method that designed to clear a given array*/
void cleanArr(char*, int);

#endif
