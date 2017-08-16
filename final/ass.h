#define toStr(x) #x
#define START_ADD 100
#define FOUR_BASE_SIZE 5
/* validate.h */
#define IMMEDIATE_ADRESS 0
#define DIRECT_ADRESS 1
#define MAT_ADRESS 2
#define DIRECT_REG 3
#define MAX_CMD 16
#define MAX_OPERAND 3
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
#define ONE_BYTE 10
#define LABEL_SIZE 30
   

typedef enum {false, true} boolean;
 
/*Symbol Table*/
typedef struct SymbolTable
{
    char* label;
    int addr;
    boolean ext;
    boolean ope;
    struct SymbolTable* next;
}SymbolTable;
 
typedef struct SymbolList
{
    struct SymbolTable* head;
    struct SymbolTable* tail;
}SymbolList;
 
/*SplitLines hold the separate words of the line and validate some of them*/
typedef struct SplitLine
{
    char* label;
    char* opCode;
    char* vars[MAX_MEMORY];
    struct SplitLine* next;
}SplitLine;

typedef struct SplitList
{
    struct SplitLines* head;
    struct SplitLines* tail;
}SplitList;
 

int i, j;
char* fname;
char* as;
char* ob;
char* ext;
char* addBase4;
char* address;
char base4Code[6];
char wierd4Code[6];
char tmp[FOUR_BASE_SIZE];
 

char* line;
SplitList* sList;
SplitLine* sLine;
SplitLine* p;
SymbolList* symbList;
SymbolTable* symbol;
SymbolTable* sp;
int L, index, r = 0;
 

enum states{START, SPACE, COMM_ZERO, COMM_ONE, COMM_TWO_A, COMM_TWO_B, INST_TWO, INST_EXTERN, LABEL, MAKE, END};
 
char* opCodes[OPSIZE] = {"rts", "stop", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                         "mov", "cmp", "add", "sub", "lea", ".data", ".string", ".mat", ".entry", ".extern"};
 
 
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
 
int typeAdress(SplitLine*, int);
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
int validExtern(char*, SymbolList*);
int validEntry(char*, SymbolList*);
int validString(char*);
int memorySize(SplitLine*);
int isVar(char*);
char* getRegister(char*);
void parseMat(char*);
 
/* list.h */
SplitList* makeSplitList();
int isEmptyList(const SplitList*);
void insertLineToTail(SplitList*, SplitLine*);
void freeList(SplitList*);
SymbolList* makeSymbolList();
int isEmptysList(const SymbolList*);
void insertSymbolToTail(SymbolList*, SymbolTable*);
SymbolTable* searchSymbol(SymbolTable*, char*);

void readline(SplitLine*, char*);
char* convertToBinary(char*);
int validOpCode(char*);
int validOperand(int, char*);
void insertToDataT(char*[], int);
void copyBinarStr(char*, char*);
char* convert_base4(char*);
char* convert_wierd4(char*);
char* convertToBinary(char*);
int isEmpty(const char*);
void insertToCode(SplitLine*, int*);
char* cmdToCode(char*);
void binaryToWierd4(char*, char*);
void cleanArr(char*, int);
void parceMat(char*);
 

void firstRun(FILE*);
void secondRun(const SplitList*, FILE*, FILE*);
 
