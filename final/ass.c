/* ======================================================================
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
-----------------------------------------------------------------------
WELCOME!
PROGRAM DEVLOPED BY: Leandro Zlotnik & Shai Gotlieb
FOR: THE OPEN UNIVERSITY OF ISRAEL
CLASS: Programming in C Lab #20465
DATE: 20/08/2017
ABOUT:
        The following program was designed for special requierments of
        The Open University of Israel.
        This is an assembler program that will convert 'special' assembly
        language to 'wierd base 4' machine language.
        In order to use the program enter the following code into the terminal:

./<NAME OF EXE FILE> <NAME OF FILE NEEDED TO TRANSLATE>

Enjoy!
-----------------------------------------------------------------------
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
========================================================================= 
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

/*----main.c----*/

int main(int argc, char* argv[]){

    for(i = 1; i < argc; i++){
        printf("Opening file %s\n",argv[i]);
        f = fopen(argv[i], "r");
        if(!f){
            printf("Error: cannot open the file %s\n", argv[i]);
            break;
        }

        object = (char*)malloc(MAX_MEMORY);
        strcpy(object, argv[i]);
        strcat(object, ".ob");
        fas = fopen(object, "w");
        if(!fas){
            printf("Error: cannot open the file %s\n", object);
            break;
        }

        entry = (char*)malloc(MAX_MEMORY);
        strcpy(entry, argv[i]);
        strcat(entry, ".ent");
        fent = fopen(entry, "w");
        if(!fent){
            printf("Error: cannot open the file %s\n", entry);
            break;
        }

        extrn = (char*)malloc(MAX_MEMORY);
        strcpy(extrn, argv[i]);
        strcat(extrn, ".ext");
        fext = fopen(extrn, "w");
        if(!fext){
            printf("Error: cannot open the file %s\n", extrn);
            break;
        }

        IC = 0;
        DC = 0;
        sHead = (SplitLine*)malloc(sizeof(SplitLine));
        sHead = NULL;
        sHead = firstRun(f, sHead);
        secondRun(sHead, fent, fext);
        addBase4 = (char*)malloc(MAX_LINE);
        if(addBase4 == NULL)
        {
             printf("Unable to allocate memory for new node\n");
             exit(1);
        }
        for(j = 0; j < IC; j++){
            mItoa((START_ADD + j), addBase4);
            convert_base4(addBase4);
            address = convert_wierd4(addBase4);
            binaryToWierd4(Code[j], tmp);
            cleanArr(tmp, FOUR_BASE_SIZE);
            fprintf(fas, "\t%s\t%s\n", address, tmp);
        }
    }
    
    freeList(sHead);
    freeSList(syHead);
    free(object);
    free(entry);
    free(extrn);
    return 1;
}

/*-----run.c------*/

SplitLine* firstRun(FILE* f, SplitLine* sHead){
	SplitLine* p;
    char line1 [ MAX_LINE ];
    SymbolT* tmp = newSym();
    if(!tmp)
        return NULL;
   
    while(fgets(line1,sizeof(line1), f) != NULL){
        
        /*if (fgets(line1,MAX_LINE, f) == NULL) break;*/
        /*if () break;*/
        
        sLine = newSL();
        sLine = readline(sLine, line1);
        printf("Readline  ------ !!!%s \n", line1);
        if (sLine != NULL)
        {
            sHead = insertLineToTail(sHead, sLine);
        }   
    }

    p = sHead;
    syHead = (SymbolT*)malloc(sizeof(SymbolT));
    syHead = NULL;
    printf("%s\n", p->label);

    while(p){
        if(validLabel(p->label)){
            indx = validOpCode(p->opCode);
            if(indx == DATA || indx == STRING || indx == MAT){
                strcpy(tmp->label, p->label);
                tmp->addr = DC;
                L = memorySize(p);
                insertToDataT(p->vars, indx);
                DC += L;
                tmp->ope = false;
                tmp->ext = false;
                tmp->next = NULL;
                insertSymbolToTail(syHead, tmp);
                p = p->next;
            }
            else if(indx == EXTERN){
                strcpy(tmp->label, p->label);
                tmp->addr = 0;
                tmp->ext = true;
                tmp->next = NULL;
                insertSymbolToTail(syHead, tmp);
                p = p->next;
            }
            else if(indx <= OPERATIONS_NUM){
                strcpy(tmp->label, p->label);
                tmp->addr = IC;
                L = memorySize(p);
                IC += L;
                tmp->ext = false;
                tmp->ope = true;
                tmp->next = NULL;
                insertSymbolToTail(syHead, tmp);
                p = p->next;
            }
            else{
                p = p->next;
            }
        }
        else{
            p = p->next;
        }
    }

    while(sp){
        if(sp->ext == false && sp->ope == false){
            sp->addr += IC;
            sp = sp->next;
        }
        else{
            sp = sp->next;
        }
    }
    return sHead;
}

void secondRun(SplitLine* sHead, FILE* fent, FILE* fext){
	SplitLine* p;
    int k, n, ind;
    /*sList is empty should verify insertion in first run*/
    p = sHead;
    for (k = 0; k < MAX_MEMORY; k++){
        for(n = 0; n < ONE_BYTE; n++){
            Code[k][n] = '\0';
        }
    }

    ind = validOpCode(p->opCode);

    while(p){
        if(ind == DATA || ind == STRING || ind == MAT){
            p = p->next;
            break;
        }
        else if(ind == ENTRY){
            sp = searchSymbol(syHead, p->label);
            mItoa((sp->addr), addBase4);
            convert_base4(addBase4);
            address = convert_wierd4(addBase4);
            fprintf(fent, "\t%s\t%s\n", sp->label, address);
            p = p->next;
            break;
        }
        else if(ind <= OPERATIONS_NUM){
            insertToCode(p, &r);
            p = p->next;
            break;
        }
        else if(ind == EXTERN){
            sp = searchSymbol(syHead, p->label);
            mItoa(sp->addr, addBase4);
            convert_base4(addBase4);
            address = convert_wierd4(addBase4);
            fprintf(fext, "\t%s\t%s\n", sp->label, address);
            p = p->next;
            break;
        }
        else{
            p = p->next;
            break;
        }
    }
}

/*-----utils.c----*/

SplitLine* readline(SplitLine* data, char* line){
    int size = strlen(line);
    int i, j=0, indx, t=0, k;
    int status = SPACE;
    int mainStatus = START;
    int commaflag = 0;
    int labelflag = 0;
    int lastRun =0;
    char temp[30];
    memset(temp, '\0', sizeof(temp));
    if(line[0] == ';')
        {
            printf("Ignoring Comment line\n");
            return NULL;
        }
    else if(isEmpty(line))
        {
            printf("Ignoring empty line\n");
            return NULL;
        }
    else{
        printf("Handling line with size of %d.\n", size);
        for(i = 0; i < size; i++){
            switch(status){
                case SPACE:
                    if(isspace(line[i])){
                        break;
                    }
                    else{
                    	memset(temp, '\0', sizeof(temp));
                        status = MAKE;
                    	--i;
                        j = 0;
                    	break;
                    }

                case MAKE:
                printf("START %c %d %d \n", line[i], i, mainStatus);
                    if(!isspace(line[i]) && line[i] != ','){
                    	commaflag = 0;
                        temp[j] = line[i];
                        j++;
                        if(line[i] == ':'){
                    		labelflag = 1;
                    	}
                        break;
                    }
                    else{
                        if(line[i] == ',' || i ==size-1){
                            if(commaflag == 1){
                                printf("ERROR: 2 commas consecutive\n");
                                break;
                            }
                        }
                        else{
                            commaflag = 1;
                            status = SPACE;
                        }                       
                    }
                    if(mainStatus == START){
                        if(labelflag == 1){
                            labelflag = 0;
                            status = LABEL;
                            break;
                        }
                        else{

                            indx = validOpCode(temp);
                            printf("%d\n", indx);
                            switch(indx){
                                case 0:
                                		printf("ERROR: invalid command\n");
                				break;
                                case 1: case 2:
                                        status = mainStatus = COMM_ZERO;
                                        data->opCode = (char*)malloc(strlen(temp));
                                        if(!(data->opCode)) return NULL;
                                        strcpy(data->opCode, temp);
                                        printf("opCode = %s\n", data->opCode);
                                        j = 0;
                                break;
                                case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
                                        status = SPACE;
                                        mainStatus = COMM_ONE;
                                        data->opCode = (char*)malloc(strlen(temp));
                                        if(!(data->opCode)) return NULL;
                                        strcpy(data->opCode, temp);
                                        printf("opCode = %s\n", data->opCode);
                                        j = 0;
                                break;
                                case 12: case 13: case 14: case 15: case 16:
                                        status = SPACE;
                                        mainStatus = COMM_TWO_A;
                                        data->opCode = (char*)malloc(strlen(temp));
                                        if(!(data->opCode)) return NULL;
                                        strcpy(data->opCode, temp);
                                        printf("opCode = %s\n", data->opCode);
                                        j = 0;
                                break;
                                case 17: case 18: case 19: case 20: case 21:
                                /* if its string all between "" enter togheter - to fix*/
                                        status = SPACE;
                                        mainStatus = INST;
                                        data->opCode = (char*)malloc(strlen(temp));
                                        if(!(data->opCode)) return NULL;
                                        strcpy(data->opCode, temp);
                                        printf("opCode = %s\n", data->opCode);
                                        j = 0;
                                break;
                            }
                        }
                    }
                    else{
                        if(i == size-1 && lastRun !=1 )
                        {
                                i--;
                                lastRun = 1;
                        }
                        status = mainStatus;
                        break;
                    }


                case LABEL:
                    if(validOpCode(temp) != 0 && labelflag == 1){
                        printf("ERROR: invalid symbol name\n");
                        break;;
                    }
                    else{
                        strcpy(data->label, temp);
                        j = 0;
                        status = SPACE;
                        labelflag = 0;
                        break;
                    }

                case COMM_ZERO:
                    status = END;
                    break;

                case COMM_ONE:
                    k = validOpCode(data->opCode);
                    
                    if( data && validOperand(k, data)){
                        data->vars[t] = (char*)malloc(strlen(temp)+1);
                        strcpy(data->vars[t], temp);
                        printf("vars4 = %s\n", data->vars[t]);
                        status = END;
                        i--;
                        t += 1;
                    }
                    else{
                        printf("Error: invalid operand for %s\n", data->opCode);
                        return NULL;
                    }
                    break;
                        
                case COMM_TWO_A:
                    data->vars[t] = (char*)malloc(strlen(temp)+1);
                    strcpy(data->vars[t], temp);
                    printf("vars3 = %s\n", data->vars[t]);
                    status = SPACE;
                    mainStatus = COMM_TWO_B;
                    t += 1;
                    i--;
                    break;

                case COMM_TWO_B:
                    data->vars[t] = (char*)malloc(strlen(temp)+1);
                    strcpy(data->vars[t], temp);
                    printf("vars2 = %s\n", data->vars[t]);
                    i--;
                    status = END;
                    t += 1;
                    break;

                case INST:
                    data->vars[t] = (char*)malloc(strlen(temp)+1);
                    strcpy(data->vars[t], temp);
                    printf("vars1 = %s\n", data->vars[t]);
                    status = SPACE;
                    commaflag = 0;
                    i--;
                    t += 1;
                    break;

                case END:
                    if(isspace(line[i])){
                        break;
                    }
                    else{
                        printf("The %s command have excesive arguments\n", data->opCode);
                        return NULL;
                    }
                break;
            }
        }

    }
    data->next = NULL;
    return data;
}

int validOpCode(char* op){
    int i;
    char* opCodes[OPSIZE] = {"rts", "stop", "not", "clr", "inc", "dec", "jmp", "bne", "red", "prn", "jsr",
                         	"mov", "cmp", "add", "sub", "lea", ".data", ".string", ".mat", ".entry", ".extern"};

 	printf("op: %s\n", op);
    for(i = 0; i < OPSIZE; i++){
        if((strstr(opCodes[i], op)) != NULL){
            return i+1;
        }
    }
    return 0;
}

int validOperand(int op, SplitLine* spl){
    int ind, i;
    int vSize = sizeof(spl->vars) / sizeof(spl->vars[0]);
    static int secondVarFlag = 0;
    if (spl->vars[0]=='\0')
        return 0;
    for(i = 0; i < vSize; i++){
        ind = typeAdress(spl, i);
        switch(op){
            case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 11:
                if(ind == 1 || ind == 2 || ind == 3)
                        return 1;
                return 0;
            case 10: case 13:
                if(ind == 0 || ind == 1 || ind == 2 || ind == 3)
                    return 1;
                return 0;
            case 12: case 14: case 15:
                if(secondVarFlag == 0){
                    if(ind == 0 || ind == 1 || ind == 2 || ind == 3){
                        secondVarFlag = 1;
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    if(ind == 1 || ind == 2 || ind == 3){
                        secondVarFlag = 0;
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
            case 16:
                if(secondVarFlag == 0){
                    if(ind == 1 || ind == 2){
                        secondVarFlag = 1;
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
                else{
                    if(ind == 1 || ind == 2 || ind == 3){
                        secondVarFlag = 0;
                        return 1;
                    }
                    else{
                        return 0;
                    }
                }
            default: return 0;
        }
    }
    return 0;
}

void insertToDataT(char** vars, int ind){
    int i, j;
    int size;
    char* tmp = (char*)malloc(sizeof(char)*DECIMAL);

    switch(ind){
        case 17:
            size = sizeof(vars[0])/sizeof(vars[0][0]);
            for(i = DC, j = 0; i < MAX_MEMORY || j < size; i++, j++){
                tmp = convertToBinary(vars[j]);
                strcpy(DataT[i], tmp);
                memset(tmp, '\0', strlen(tmp));
            }
            DC = i;
            break;

        case 18:
            size = strlen(vars[0]);
            for(i = DC, j = 0; i < MAX_MEMORY && j < size; i++, j++){
                tmp = convertToBinary(vars[j]);
                strcpy(DataT[i], tmp);
                memset(tmp, '\0', strlen(tmp));
            }
            strcpy(DataT[++i], convertToBinary(0));
            DC = i;
            break;

        case 19:
            size = ((sizeof(*vars))/(sizeof(*vars[0])));
            for(i = DC, j = 1; i < MAX_MEMORY || j < size; i++, j++){
                tmp = convertToBinary(vars[j]);
                strcpy(DataT[i], tmp);
                memset(tmp, '\0', strlen(tmp));
            }
            DC = i;
            break;
    }
    free(tmp);
}


char* convert_base4(char* inputNum)
{
    int num = atoi(inputNum), ind;
    char* result[DECIMAL];
    memset(result, '\0', sizeof(result));
    ind = 9;
    while(num > 0)
    {
        if(num % BASE != 0)
        {
            *result[ind] = '1';
        }
        else{
            *result[ind] = '0';
        }
        ind--;
        num /= BASE;
    }
    return *result;
}

char* convert_wierd4(char* str)
{
    int r, k = BASE;
    char temp, *wierdStr;
    int num = atoi(str);
    int tempNum = num;
    wierdStr = (char*)malloc(MAX_LINE);
    while(tempNum > 0)
    {
        if((tempNum % 10 ) == 0)
            wierdStr = strcat(wierdStr, "a");
        if((tempNum % 10 ) == 1)
            wierdStr = strcat(wierdStr, "b");
        if((tempNum % 10 ) == 2)
            wierdStr = strcat(wierdStr, "c");
        if((tempNum % 10 ) == 3)
            wierdStr = strcat(wierdStr, "d");
        tempNum /= 10;
    }
    /* reverse the string*/
    for(r = 0; r < BASE; r++)
    {
        temp = wierdStr[k];/* make it array with malloc??*/
        wierdStr[k] = wierdStr[r];
        wierdStr[r] = temp;
        k--;
    }
    return wierdStr;
}

void binaryToWierd4(char* source, char* tmp){
    int i, j, k = 0;
    for(i = 0, j = 1; i < strlen(source); i+=2, j+=2){
        if(source[i] == '0'){
            if(source[j] == '0'){
                tmp[k] = 'a';
                k++;
                continue;
            }
            else{
                tmp[k] = 'b';
                k++;
                continue;
            }
        }
        else{
            if(source[j] == '0'){
                tmp[k] = 'c';
                k++;
                continue;
            }
            else{
                tmp[k] = 'd';
                k++;
                continue;
            }
        }
    }
    return;
}

void cleanArr(char* arr, int size){
    int i;
    for(i = 0; i < size; i++){
        arr[i] = 0;
    }
}

char* convertToBinary(char* inputNum)
{
    int num = atoi(inputNum), ind;
    char* result = (char*)malloc(sizeof(char)*DECIMAL);
    memset(result, '\0', strlen(result));
    ind = 9;
    while(num > 0)
    {
        if(num % 2 != 0)
        {
            result[ind] = '1';
        }
        else{
            result[ind] = '0';
        }
        ind--;
        num /= 2;
    }
    return result;
}

int isEmpty(char* str){
   int i;
   int size = strlen(str);
   for(i = 0; i < size; i++){
       if(isspace(str[i])){
           continue;
       }
       else{
           return 0;
       }
   }
   return 1;
}

void insertToCode(SplitLine* p, int* r)
{
    int indx = 0, type = 0, regFlag = 0, NullFlag = 0, NumFlag=0, *row = r, tmpR = 1;
    char* temp, *str = NULL, *s, *binary = (char*)malloc(sizeof(char)*DECIMAL);
    SymbolT* ptr;
    char* tempArr[3];
    temp = cmdToCode(p->opCode);
    strcpy(Code[*row], temp);
        while(*row < MAX_MEMORY && p->vars[indx] != NULL)
        {
            type = typeAdress(p, indx);
            switch(type)
            {
                case IMMEDIATE_ADRESS:   strcat(Code[*row], "00");
                                         s = strtok(p->vars[indx], "#-+");
                                         binary = convertToBinary(s);
                                         binary = (char*)realloc(binary, sizeof(binary)+2);
                                         strcat(binary, "00\0");
                                         strcat(Code[*row+tmpR], binary);
                                         NumFlag = 1;
                                         indx++;
                                         tmpR++;
                                         free(binary);
                                         break;

                case DIRECT_ADRESS:     strcat(Code[*row], "01");
                                        ptr = searchSymbol(syHead, p->label);
                                        if(ptr == NULL)
                                        {
                                            if(isRegister(p->vars[indx])==1)
                                            {
                                                strcat(Code[*row+tmpR], getRegister(p->vars[indx]));/* dest register is in bytes 2-5*/
                                                strcat(Code[*row+tmpR], "0000");
                                            }
                                        }
                                        mItoa(ptr->addr, str);
                                        str = convertToBinary(str);
                                        strcat(Code[*row+tmpR], str);
                                        if(ptr->ext == 0){
                                            strcat(Code[*row+tmpR], "10\0");
                                        }
                                        else{
                                            strcat(Code[*row+tmpR], "01\0");
                                        }
                                        indx++;
                                        tmpR++;
                                        break;

                case MAT_ADRESS:        strcat(Code[*row], "10");
                                        parseMat(p->vars[indx]);/* malloc*/
                                        ptr = searchSymbol(syHead, tempArr[0]);
                                        mItoa(ptr->addr, temp);
                                        temp = convertToBinary(temp);
                                        strcpy(Code[*row+tmpR], temp);
                                        tmpR++;
                                        strcat(Code[*row+tmpR], tempArr[2]);/* dest register is in bytes 2-5*/
                                        strcat(Code[*row+tmpR], tempArr[1]);/* source register is in bytes 6-9*/
                                        if(ptr->ext == 0){
                                            strcat(Code[*row+tmpR], "10\0");
                                        }
                                        else{
                                            strcat(Code[*row+tmpR], "01\0");
                                        }
                                        indx++;
                                        tmpR++;
                                        break;

                case DIRECT_REG:        strcat(Code[*row], "11");
                                        if(NullFlag == 1)
                                        {
                                            strcat(Code[*row+tmpR], getRegister(p->vars[indx]));
                                            strcat(Code[*row+tmpR], "00\0");
                                        }
                                        else if(regFlag == 1)
                                        {
                                            tmpR--;
                                            strcat(Code[*row+tmpR], getRegister(p->vars[indx]));/* dest register is in bytes 2-5*/
                                            strcat(Code[*row+tmpR], getRegister(p->vars[indx-1]));/* source register is in bytes 6-9*/
                                            strcat(Code[*row+tmpR], "00\0");
                                        }
                                        if(NumFlag == 1)
                                        {
                                            strcat(Code[*row+tmpR], getRegister(p->vars[indx]));
                                            strcat(Code[*row+tmpR], "0000");
                                            strcat(Code[*row+tmpR], "00\0");
                                        }

                                        regFlag = 1;
                                        indx++;
                                        tmpR++;
                                        break;

                 default:               strcat(Code[*row], "00");
                                        NullFlag = 1;
                                        if(regFlag == 1)
                                        {
                                           tmpR--;
                                           strcat(Code[*row+tmpR], "0000");
                                           strcat(Code[*row+tmpR], getRegister(p->vars[indx]));
                                        }
                                        indx++;
                                        tmpR++;
            }
            p = p->next;
        }
    strcat(Code[*row], "00\0");
    r += tmpR;
}

void mItoa(int n, char s[])
 {
     int i, sign;
     s= malloc(200* sizeof(char));
     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0 && i < strlen(s));     /* delete it */
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

  void reverse(char s[])
 {
     int i, j;
     char c;

     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

char* cmdToCode(char* currcmd)
{
    char* cmdArr[MAX_CMD][MAX_CMD] = {
     { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec",
    "jmp", "bne", "red", "prn", "jsr", "rts", "stop" },
     { "0000", "0001", "0010", "0011", "0100", "0101", "0110", "0111",
    "1000", "1001", "1010", "1011", "1100", "1101", "1110",
       "1111" }
 };
/* concatenate the binary "base 4" number of the right command to a temporary string*/
    int i, j;
    char* tempStr = "";
    for(i=0, j=0; j < MAX_CMD; j++)
    {
     if(strcmp(currcmd, cmdArr[i][j]))
     {
      i += 1;
      strcpy(tempStr, cmdArr[i][j]);
      break;
     }
    }
    return tempStr;
}

void parseMat(char* mat){

    int i, j = 0;
    int statFlag = 0;
    int size = strlen(mat);
    
    for(i = 0; i < size; i++){
        switch(statFlag){
            case 0:
                    if(mat[i] != '[' && mat[i] != ']'){
                        name[j] = mat[i];
                        j++;
                        break;
                    }
                    else{
                        if(mat[i] == '['){
                            statFlag++;
                            j = 0;
                            break;
                        }
                        else{
                            break;
                        }
                    }
            case 1:
                    if(mat[i] != '[' && mat[i] != ']'){
                        R1[j] = mat[i];
                        j++;
                        break;
                    }
                    else{
                        if(mat[i] == ']'){
                            statFlag++;
                            j = 0;
                            break;
                        }
                        else{
                            j = 0;
                            break;
                        }
                    }
            case 2:
                    if(mat[i] != '[' && mat[i] != ']'){
                        R2[j] = mat[i];
                        j++;
                        break;
                    }
                    else{
                        break;
                    }
            default: break;
        }
    }
    return;
}

/*-----validate.c----*/

int typeAdress(SplitLine* sl, int index)
{
    if(isNumber(sl->vars[index]) != 0 ){
        return IMMEDIATE_ADRESS;/*"00" */
    }
    if(isLetter(sl->vars[index]) != 0){
        return DIRECT_ADRESS;/*"01" */
    }
    if (isMatrix(sl->vars[index]) != 0){
        return MAT_ADRESS; /*"10" */
    }
    if(isRegister(sl->vars[index]) != 0){
        return DIRECT_REG;/* "11" */
    }
    return 4;
}

int isNumber(char* str)
{
    char c;
    if (str != NULL) {
/* minus and plus is  allowed, number must start with # */
        if((*str == '-' || *str == '+') && *str == '#')
            str++;
        while ((c=*str++))
        {
            if(!isdigit(c))
                return 0;
        }
        return 1;
    }
    return 0;
}

/* returns ture if the specifed char is an english letter a-z or A-Z */
int isLetter(char* word)
{
    int i, size = strlen(word);
    
    for(i = 1; i < size; i++){
        if(!isalpha(word[0])){
            return 0;
        }
        if(!isdigit(word[i]) && !isalpha(word[i])){
            return 0;
        }
        else{
            continue;
        }
    }
    return 1;
}

/* cheack if a word is a valid register */
int isRegister(char* word)
{
    if((word[0] == 'r') && (word[1] >= 0) && (word[1] <= 7) &&
(strlen(word) == 2))
return 1; /* success */
        else
return 0; /* not valid register - print error outside of the function */
}

int validLabel(char* word)
{
    int i = 0;
    int labLength = 0;
    SymbolT* head = syHead;
    if(isEmpty(word) == 1)
    {
        printf("ERROR! Label is empty!\n");
        return 0;
    }

    if(!isalpha(word[0]))
    {
        printf("ERROR! Label must start with letters.\n");
        return 0;
    }
/* check the length of the label until ':'*/
    while(word[i] != ':' && word[i] != '\0')
    {
        labLength++;
        i++;
    }

    if(labLength > MAX_LABEL)
    {
        printf("ERROR! Lable is too long.");
        return 0;
    }
    if(head == NULL)
    {
        return 1;
    }
    /* check if label is already exist in the list*/
    while(head != NULL)
    {
        if(strcmp(head->label, word) == 0)
        {
            printf("ERROR! Label: [%s] already exist in symbol list\n", word);
            return 0;
        }
        head = head->next;
    }
    if(validOpCode(word) != 0){
            printf("ERROR! Label [%s] is a saved word!\n", word);
            return 0;
        }
    else{
    	return 1;
	}
}

/* isMatrix get a word and validate if it's a llegal matrix - return 1
if success*/
int isMatrix(char* word)
{
    int i = 0, j, flagOpen, flagClose;
    char tempStr[REG_SIZE];
    if(isOpenBracket(word[i]) == 1)
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with alphabetic character.\n");
        return 0;
    }
    if(isdigit(word[i]))
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with alphabetic character.\n");
        return 0;
    }
    if(!(isalpha(word[i])))
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with alphabetic character.\n");
        return 0;
    }
/*Matrix name must contain either alphabetic characters or digits characters */
    while(isalpha(word[i]) || isdigit(word[i]))
    {
        i++;
    }
    if(isOpenBracket(word[i]))
    {
        flagOpen = 1;
        i++;
    }
    else
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with alphabetic character.\n");
        return 0;
    }
    if(flagOpen == 1)
    {
        j = 0;
/* sepperate the words after a llegal matrix in order to check the next word*/
        breakMat(word);

        if(isRegister(tempStr) != 1)
        {
            printf("ERROR! Invalid use of matrix. Matrix must contain llegal register.\n");
            return 0;
        }
        else
        {
            i++;
            if(isCloseBracket(word[i]) != 1)
            {
                printf("ERROR! Invalid use of matrix. Matrix must contain llegal register.\n");
                return 0;
            }
            else
            {
                flagClose = 1;
                i++;
            }
            if(isOpenBracket(word[i]) != 1)
            {
                printf("ERROR! Invalid use of matrix. second matrix must start with square bracket and must be beside first matrix.\n");
                return 0;
            }
            else
            {
                flagOpen += 1;
                i++;
            }
            j = 0;
/* sepperate the words after a llegal matrix in order to check the next word*/
            breakMat(word);

            if(isCloseBracket(word[i]) == 1)
            {
                flagClose += 1;
            }
            if(isRegister(tempStr) != 1)
            {
                printf("ERROR! Invalid use of matrix. second matrix must contain llegal register.\n");
                return 0;
            }
            else
            {
                i++;
    /* copy the line untill the end to tempStr*/
                while(word[i] != '\n' && i < strlen(word))
                {
                    j=0;
                    tempStr[j] = word[i];
                    i++;
                    j++;
                }
    /* the rest of the line after a llegal matrix must be empty */
                if(isEmpty(tempStr) != 1)
                {
                    printf("ERROR! Invalid use of matrix. After declaration of matrix line must be empty.\n");
                    return 0;
                }
            }
        }
    }
/* last check to see that we really got lleagal matrix by matching
closing and opening square brackets */
    if(flagOpen == 2 && flagClose == 2){
        return 1;
    }
    else{
        return 0;
    }
}


/* isMatrix get a word and validate if it's a llegal matrix - return 1
if success*/
int isMatrixObject(char* word)
{
    int i = 0, firstNumber, secondNumber;
    if(isOpenBracket(word[i]) != 1 && isdigit(word[i+1]) == 1 )
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
        return 0;
    }
    firstNumber = 0;
    secondNumber = 0;
    i++;
/*extracting the full number as int from the word string*/
    while (isdigit(word[i]) == 1)
    {
        firstNumber = (firstNumber * 10) + word[i];
        i++;
    }
    if (isCloseBracket(word[i]) !=1)
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
        return 0;
    }
    i++;
    if(isOpenBracket(word[i]) != 1 && isdigit(word[i+1]) == 1 )
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
        return 0;
    }
    i++;
    while (isdigit(word[i]) == 1)
    {
        secondNumber = (secondNumber * 10) + word[i];
        i++;   }
        if (isCloseBracket(word[i]) !=1)
        {
            printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
            return 0;
        }
        i++;
        if(word[i] != '\0')/* different from NULL*/
        {
            printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
            return 0;
        }
        return firstNumber*secondNumber;
}


int validateMatCommandObject(char* word)
{
/*split word by space*/
    char *array = (char*)malloc(strlen(word));
    int NumberOfElements = 0, matrixSize, result;
    int i=0;
    char* str = strtok(word," ");
    strcpy(&array[i], str);
    while(array[i]!= '\0')/* different from NULL('\0')*/
    {
        strcpy(&array[++i], strtok(NULL," "));
    }
/*validate matrix size*/
    NumberOfElements = sizeof(array)/sizeof(array[0]);

    if (NumberOfElements >2)
    {
        printf("ERROR! Invalid use of matrix. Too many spaces in word.\n");
        return 0 ;
    }
    matrixSize = isMatrixObject(&array[0]);
    if (matrixSize == 0)
    {
        printf("ERROR! Invalid use of matrix.\n");
        return 0;
    }
    result = isMatrixInputValid(&array[1], matrixSize);
    if ( result == 0)
    {
/*error with matrix elements input*/
        printf("ERROR! Invalid use of matrix. Invalid matrix element input.\n");
        return 0;
    }

    return matrixSize;

}
/* isMatrixInputValid*/
int isMatrixInputValid(char* word, int matrixSize)
{

/*split word by commas*/
    char *array = (char*)malloc(matrixSize);
    int i=0;
    int NumberOfElements = 0;
    strcpy(&array[i], strtok(word,","));
    while(array[i]!= '\0')
    {
        strcpy(&array[++i], strtok(word,","));
    }

/*validate matrix size*/
/*check if number of elements is less or equal to matrixSize*/
    NumberOfElements = sizeof(array)/sizeof(array[0]);
    if ( NumberOfElements > matrixSize)
    {
        printf("ERROR! Invalid declaration of Matrix elements - number of elements [%d] should match matrix size [%d].\n", NumberOfElements, matrixSize);
        return 0;
    }

/*check if each element is a positive or negetive number*/
    i = 0;
    while ( array[i] != '\0')/* different from NULL*/
    {
/*0 is a valid number but atoi returns 0 on error so we need to check
it seperatly */
        if ( array[i] != '0' && atoi(&array[i]) == 0)
        {
            printf("ERROR! Invalid declaration of Matrix elements. elements should be only numbers. failed on element [%d] \n", array[i]);
            return 0;
        }
        i++;
    }
    return 1;
}

/* Function that checks if a square bracket is an opening kind */
int isOpenBracket(char c)
{
    if(c == '[')
        return 1;
    else
        return 0;
}

/* Function that checks if a square bracket is a closing kind */
int isCloseBracket(char c)
{
    if(c== ']' )
        return 1;
    else
        return 0;
}

/* Function that breaks the words after a matrix*/
void breakMat(char* word)
{
    tempStr = (char*)malloc(MAX_LINE);
    if(tempStr == NULL)
        {
             printf("Unable to allocate memory!\n");
             exit(1);
        }
    while(word[i] != ']' && i <= strlen(word))
    {
        tempStr[j] = word[i];
        i++;
        j++;
    }
}

int validData(char* var)
{
    int i = 0, size = 0;
    if(checkConsecutiveSigns(var) == 1)
    {
        printf("ERROR! elements in .data contain consequtive signs!\n");
        return 0;
    }
    while(isdigit(var[i]) || var[i] == '-' || var[i] == '+' || var[i] == ',')
    {
        /* count number of commas in order to know the number of elements*/
        if(var[i] == ',')
            size++;
        i++;
    }
    if(var[i] == '\0' && var[i-1] == ',')
    {
        printf("ERROR! line can't end with a comma!\n");
        return 0;
    }
    if( i < strlen(var))
    {
        printf("ERROR! elemnt: [%d] in .data must be a valid number!\n", var[i]);
        return 0;
    }
    if(var[i] == '\0' && var[i-1] == ',')
    {
        printf("ERROR! line can't end with a comma!\n");
        return 0;
    }

    return size + 1; /* size was increased by the number of commas*/
}

/*check if there are consecutive signs: '-' or '+' in the user input */
int checkConsecutiveSigns(char* input)
{
    int i;
    char last = 'a';
    for(i=0; i<strlen(input); i++)
    {
        if((input[i] == '-' || input[i] == '+') && (last == '-' || last == '+' )){
            return 1;
        }
        if(input[i] != ' ')
            last = input[i];
    }

    return 0;
}

/*check if there are consecutive commas in the user input */
int checkConsecutiveCommas(char* input)
{

    int i;
    char last='a';
    for(i=0; i<strlen(input) ;i++)
    {
        if(input[i]==',' && last==','){
            return 1;
        }
        if(input[i]!=' ')
            last=input[i];
    }

    return 0;
}

/* validString check if a string after ".string" is lligal and return
the size of the string (in order to increase DC)
    if string is not valid - return 0 */
int validString(char* var)
{   int i = 0, size = 0;
    if(isEmpty(var) == 1)
    {
        printf("ERROR! Empty string after .string\n");
        return 0;
    }
    while(isspace(var[i]))
    {
        i++;
    }
    if(var[i] != '\"')
    {
        printf("ERROR! string must start with \" ! Instead it starts with: [%d]\n", var[i]);
        return 0;
    }
    else
    {
        i++;
        while(var[i] != '\"')
        {
            i++;
            size++;
        }
/* check if there are more characters after the end of the string*/
        if(strlen(var) > size+2)
        {
            printf("ERROR! string must end after '\"' \n");
            return 0;
        }

        if(var[i] == '\"' && var[i+1] == '\0')
        {
            return size;
        }
    }
    return 0;
}

int validEntry(char* word, SymbolT* head)
{
    if(isEmpty(word) == 1)
    {
        printf("ERROR! line after '.entry' is empty!\n");
        return 0;
    }

    if(head == NULL)
    {
        printf("ERROR! Symbol after .entry must exist in symbol list! %s does'nt exist!\n", word);
        return 0;
    }
    while(head != NULL)
    {
        if(strcmp(head->label, word) == 0)
        {
            return 1;
        }
        head = head->next;
    }
    printf("ERROR! The symbol doesn't match after .entry\n");
    return 0;
}

int validExtern(char* word, SymbolT* head)
{
    if(isEmpty(word) == 1)
    {
        printf("ERROR! line after '.extern' is empty!\n");
        return 0;
    }
    if(head == NULL)
    {
        return 1;
    }
    while(head != NULL)
    {
        if(strcmp(head->label, word) == 0)
        {
            printf("ERROR! Local label cannot be declared as external.\n");
            return 0;
        }
        head = head->next;
    }
    return 1;
}

int memorySize(SplitLine* sl)
{
    int regFlag = 0;
/* check if it's one of the commands*/
    if(validOpCode(sl->opCode) > MAX_CMD)
    {
        return 0;
    }
    /* if it's empty*/
    if(sl->opCode == NULL)
    {
        return 0;
    }
    else
    {
        /* run through the array and increase L by the correct type adress*/
        for(i = 0; i < strlen(*(sl->vars)); i++)
        {
            if(typeAdress(sl, i) == IMMEDIATE_ADRESS || typeAdress(sl, i) == DIRECT_ADRESS || (isVar(sl->vars[i]) == 1))
            {
                L++;
            }
            if(typeAdress(sl, i) == MAT_ADRESS)
            {
                L += 2;
            }
            if(typeAdress(sl, i) == DIRECT_REG)
            {
                if(regFlag == 0)
                {
                    L++;
                    regFlag = 1;
                }
                else
                    break;
            }
        }
    }
        return L;
}

char* getRegister(char* r){
    switch(r[1]){
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        default: return NULL;
    }
}

int isVar(char* word)
{
    int i = 0;
    char c;
    while(i < strlen(word))
    {
        c = word[i];
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || isdigit(c) != 0)
                i++;
                else return 0;
    }
    return 1;
}

/*-----list.c-------*/
/*
SplitList* makeSplitList(){
    SplitList* splitList = NULL;
    splitList->head = (SplitLine*)malloc(sizeof(SplitLine));
    return splitList;
}
int isEmptyList(SplitList* lst){
    return lst->head == NULL;
}
*/
SplitLine* insertLineToTail(SplitLine* head, SplitLine* line){
    SplitLine *p = (SplitLine*)malloc(sizeof(SplitLine));
    printf("++++++++++++++++++++++++\n");  
    if(p == NULL){
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }
    p = line;
    p->next = NULL;  

    if(head == NULL){
        head = p;
        head->next = NULL;
        printf("added at beginning\n");
    }
    else{
        SplitLine* curr = head;
        /*while(1) {*/ 
            while(curr->next != NULL)
            {   
                curr = curr->next;
            }
                /*curr->next = (SplitLine*)malloc(sizeof(SplitLine));*/
                curr->next = p;
                curr->next->next = NULL;
                printf("added later\n");
        }
    /*free(p);*/
    return head;
}

int countList(SplitLine* curr)
{
    int i = 1;
    while(curr->next != NULL)
            {   
                curr = curr->next;
                i++;
            }
            return i;
}

void freeList(SplitLine* curr){
    SplitLine* tmp;
    while (curr != NULL)
    {
       tmp = curr;
       curr = curr->next;
       free(tmp);
    }

}

void freeSList(SymbolT* syHead){
    SymbolT* curr = syHead;
    SymbolT* next;
    while(curr){
        next = curr->next;
        free(curr);
        curr = next;
    }
    syHead = NULL;
}

SymbolT* insertSymbolToTail(SymbolT* head, SymbolT* symbol){
    SymbolT *p = (SymbolT*)malloc(sizeof(SymbolT));
    printf("++++++++++++++++++++++++\n");  
    if(p == NULL){
        fprintf(stderr, "Unable to allocate memory for new node\n");
        exit(-1);
    }

    p = symbol;
    p->next = NULL;  

    if(head == NULL){
        head = p;
        head->next = NULL;
        printf("added at beginning\n");
    }
    else{
        SymbolT* curr = head;
        while(1) { 
            if(curr->next == NULL)
            {   
                curr->next = (SymbolT*)malloc(sizeof(SymbolT));
                curr->next = p;
                curr->next->next = NULL;
                printf("added later\n");
                break; 
            }
            else{
                curr = curr->next;
            }
        }
    }
    free(p);
    return head;
}

SplitLine* newSL(){
    SplitLine* n = (SplitLine*)malloc(sizeof(SplitLine));
    n->next = NULL;
    return n;
}

SymbolT* newSym(){
    SymbolT* n = (SymbolT*)malloc(sizeof(SymbolT));
    n->next = NULL;
    return n;
}




SymbolT* searchSymbol(SymbolT* head, char* symbol){
    SymbolT* p = head;
    while(p){
        if((strcmp(p->label, symbol)) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}
