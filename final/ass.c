#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "header.h"

/*----main.c----*/

int main(int argc, char* argv[]){

    for(i = 1; i < argc; i++){
        f = fopen(argv[i], "r");
        if(!f){
            printf("Error: cannot open the file %s\n", argv[i]);
            break;
        }

        object = strcat(argv[i], ".ob");
        fas = fopen(object, "w");
        if(!fas){
            printf("Error: cannot open the file %s\n", object);
            break;
        }

        entry = strcat(argv[i], ".ent");
        fent = fopen(entry, "w");
        if(!fent){
            printf("Error: cannot open the file %s\n", entry);
            break;
        }

        extrn = strcat(argv[i], ".ext");
        fext = fopen(extrn, "w");
        if(!f){
            printf("Error: cannot open the file %s\n", extrn);
            break;
        }

        IC = 0;
        DC = 0;
        firstRun(f);
        secondRun(sList, fent, fext);
        for(j = 0; j < IC; j++){

            mItoa((START_ADD + j), addBase4);
            convert_base4(addBase4);
            address = convert_wierd4(addBase4);
            binaryToWierd4(Code[j], tmp);
            cleanArr(tmp, FOUR_BASE_SIZE);
            fprintf(fas, "\t%s\t%s\n", address, tmp);
        }
    }
}

/*-----run.c------*/

void firstRun(FILE* f){
    int symbolflag = 0;
    SymbolTable* tmp = (SymbolTable*)malloc(sizeof(SymbolTable));
    while(feof(f)){
        fgets(line, 80, f);
        sList = makeSplitList();
        readline(sLine, line);
        insertLineToTail(sList, sLine);
    }
    symbList = makeSymbolList();
    p = sList->head;
    sp = symbList->head;

    while(p){
        if(validLabel(p->label)){
            symbolflag = 1;
            indx = validOpCode(p->opCode);
            if(indx == DATA || indx == STRING || indx == MAT){
                tmp->label = (char*)malloc(sizeof(p->label));
                if(!(tmp->label)) return;
                strcat(tmp->label, p->label);
                tmp->addr = DC;
                L = memorySize(p);
                insertToDataT(p->vars, indx);
                DC += L;
                tmp->ope = false;
                tmp->ext = false;
                tmp->next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else if(indx == EXTERN){
                tmp->label = (char*)malloc(sizeof(p->label));
                if(!(tmp->label)) return;
                strcat(tmp->label, p->label);
                tmp->addr = 0;
                tmp->ext = true;
                tmp->next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else if(indx <= OPERATIONS_NUM){
                tmp->label = (char*)malloc(sizeof(p->label));
                if(!(tmp->label)) return;
                strcat(tmp->label, p->label);
                tmp->addr = IC;
                L = memorySize(p);
                IC += L;
                tmp->ext = false;
                tmp->ope = true;
                tmp->next = NULL;
                insertSymbolToTail(symbList, tmp);
                p = p->next;
            }
            else{
                p = p->next;
                break;
            }
        }
        else{
            p = p->next;
            break;
        }
    }
    free(tmp);

    while(sp){
        if(sp->ext == false && sp->ope == false){
            sp->addr += IC;
            sp = sp->next;
        }
        else{
            sp = sp->next;
        }
    }
}

void secondRun(const SplitList* sList, FILE* fent, FILE* fext){
    int k, n, ind;
    p = sList->head;
    for (k = 0; k < MAX_MEMORY; k++){
        for( n = 0; n < ONE_BYTE; n++){
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
            sp = searchSymbol(symbList->head, p->label);
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
            sp = searchSymbol(symbList->head, p->label);
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

void readLine(SplitLine* data, char* line){
    int size = strlen(line);
    int i, j=0, indx, t=0, k;
    int status = SPACE;
    int mainStatus = START;
    int symbolflag = 0;
    int commaflag = 0;
    char* temp = (char*)malloc(sizeof(char));
    if(!(temp)) return;

    if(line[0] == ';')
        return;
    else if(isEmpty(line))
        return;
    else{
        for(i = 0; i < size; i++){
            switch(status){
                case SPACE:
                    if(isspace(line[i]))
                        break;
                    status = MAKE;
                    --i;
                    break;

                case MAKE:
                    if(!isspace(line[i]) && line[i] != ','){
                        commaflag = 0;
                        temp[j] = line[i];
                        j++;
                        temp = (char*)realloc(temp, sizeof(temp)+1);
                        if(!(temp)) return;
                        break;
                    }
                    else{
                        if(line[i] == ','){
                            if(commaflag == 0){
                                commaflag = 1;
                            }
                            else{
                                printf("ERROR: 2 commas consecutive\n");
                                return;
                            }
                        }
                    }
                    temp = (char*)realloc(temp, sizeof(temp)+1);
                    if(!(temp)) return;
                    temp[j+1] = '\0';

                    if(mainStatus == START){
                        if(temp[j] == ':'){
                            temp[j] = '\0';

                            status = LABEL;
                            break;
                        }
                        else{
                            indx = validOpCode(temp);
                            switch(indx){
                                case 0: printf("ERROR: invalid command\n");
                                return;
                                case 1: case 2:
                                        status = mainStatus = COMM_ZERO;
                                        data->opCode =
(char*)malloc(sizeof(temp));
                                        if(!(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 3: case 4: case 5: case 6: case
7: case 8: case 9: case 10: case 11:
                                        status = SPACE;
                                        mainStatus = COMM_ONE;
                                        data->opCode =
(char*)malloc(sizeof(temp));
                                        if(!(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 12: case 13: case 14: case 15: case 16:
                                        status = SPACE;
                                        mainStatus = COMM_TWO_A;
                                        data->opCode =
(char*)malloc(sizeof(temp));
                                        if(!(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 17: case 18: case 19: case 20: case 21:
                                /* if its string all between "" enter
togheter - to fix*/
                                        status = SPACE;
                                        mainStatus = INST;
                                        data->opCode =
(char*)malloc(sizeof(temp));
                                        if(!(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                            }
                        }
                    }
                    else{
                        status = mainStatus;
                        break;
                    }


                case LABEL:
                    if(validOpCode(temp) != 0){ /*shai code instead
validOpCode*/
                        printf("ERROR: invalid symbol name\n");
                        return;
                    }
                    else{
                        data->label = (char*)malloc(sizeof(temp));
                        if(!(data->label)) return;
                        strcat(data->label, temp);
                        symbolflag = 1;
                        free(temp);
                        temp = (char*)malloc(sizeof(char));
                        j = 0;
                        status = SPACE;
                        break;
                    }

                case COMM_ZERO:
                    free(data->vars);
                    status = END;
                    break;

                case COMM_ONE:
                    k = validOpCode(data->opCode);
                    if(validOperand(k, data)){
                        data->vars[t] = (char*)malloc(sizeof(temp));
                        if(!(data->vars[t])) return;
                        strcat(data->vars[t], temp);
                        t=0;
                        status = END;
                    }
                    else{
                        printf("Error: invalid operand for %s\n", data->opCode);
                        return;
                    }
                    break;

                case COMM_TWO_A:
                    data->vars[t] = (char*)malloc(sizeof(temp));
                    if(!(data->vars[t])) return;
                    strcat(data->vars[t], temp);
                    free(temp);
                    temp = (char*)malloc(sizeof(char));
                    t++;
                    status = SPACE;
                    mainStatus = COMM_TWO_B;
                    break;

                case COMM_TWO_B:
                    data->vars[t] = (char*)malloc(sizeof(temp));
                    if(!(data->vars[t])) return;
                    strcat(data->vars[t], temp);
                    free(temp);
                    t = 0;
                    status = END;
                    break;

                case INST:
                    data->vars[t] = (char*)malloc(sizeof(temp));
                    if(!(data->vars[t])) return;
                    strcat(data->vars[t], temp);
                    free(temp);
                    t++;
                    status = SPACE;
                    break;

                case END:
                    if(isspace(line[i])){
                        break;
                    }
                    else{
                        printf("The %s command have excesive
arguments\n", data->opCode);
                        return;
                    }
                break;
            }
        }

    }
    free(temp);
    data->next = NULL;
    return;
}

int validOpCode(char* op){
    int i;
    for(i = 0; i < OPSIZE; i++){
        if((strcmp(op, opCodes[i])) == 0){
            return i+1;
        }
    }
    return 0;
}

int validOperand(int op, SplitLine* spl){
    int ind, i;
    int vSize = sizeof(spl->vars) / sizeof(spl->vars[0]);
    static int secondVarFlag = 0;
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

void insertToDataT(char* vars[], int ind){
    int i, j;
    int size;

    switch(ind){
        case 17:
            size = sizeof(*vars)/sizeof(*vars[0]);
            for(i = DC, j = 0; i < MAX_MEMORY || j < size; i++, j++){
                copyBinarStr(DataT[i], convertToBinary(vars[j]));
            }
            DC = i;
            break;

        case 18:
            size = strlen(vars[0]);
            for(i = DC, j = 0; i < MAX_MEMORY || j < size; i++, j++){
                copyBinarStr(DataT[i], convertToBinary(&vars[0][j]));
            }
            copyBinarStr(DataT[++i], convertToBinary(0));
            DC = i;
            break;

        case 19:
            size = ((sizeof(*vars))/(sizeof(*vars[0])));
            for(i = DC, j = 1; i < MAX_MEMORY || j < size; i++, j++){
                copyBinarStr(DataT[i], convertToBinary(vars[j]));
            }
            DC = i;
            break;
    }
}

void copyBinarStr(char* x, char* y){
    int i;
    for(i = 0; i < 10; i++){
        x[i] = y[i];
    }
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
    char temp, *wierdStr ="";
    int num = atoi(str);
    int tempNum = num;
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
    char* result[DECIMAL];
    memset(result, '\0', sizeof(result));
    ind = 9;
    while(num > 0)
    {
        if(num % 2 != 0)
        {
            *result[ind] = '1';
        }
        else{
            *result[ind] = '0';
        }
        ind--;
        num /= 2;
    }
    return *result;
}

int isEmpty(char* str){
    char* p = str;
    while(*p != '\0'){
        if(isspace(*p)){
            p+=1;
        }
        else{
            return 0;
        }
    }
    return 1;
}

void insertToCode(SplitLine* p, int* r)
{
    int indx = 0, type = 0, regFlag = 0, NullFlag = 0, NumFlag=0, *row
= r, tmpR = 1;
    char* temp, *str = NULL, *s, *binary = (char*)malloc(sizeof(char)*9);
    SymbolTable* ptr;
    char* tempArr[3];
    temp = cmdToCode(p->opCode);
    strcat(Code[*row], temp);
        while(*row < MAX_MEMORY && p->vars[indx] != NULL)
        {
            type = typeAdress(p, indx);
            switch(type)
            {
                case IMMEDIATE_ADRESS:   strcat(Code[*row], "00");
                                         s = strtok(p->vars[indx], "#-+");
                                         strcat(binary, convertToBinary(s));
                                         binary =
(char*)realloc(binary, sizeof(binary)+2);
                                         strcat(binary, "00\0");
                                         strcat(Code[*row+tmpR], binary);
                                         NumFlag = 1;
                                         indx++;
                                         tmpR++;
                                         free(binary);
                                         break;

                case DIRECT_ADRESS:     strcat(Code[*row], "01");
                                        ptr =
searchSymbol(symbList->head, p->label);
                                        if(ptr == NULL)
                                        {
                                            if(isRegister(p->vars[indx])==1)
                                            {

strcat(Code[*row+tmpR], getRegister(p->vars[indx]));/* dest register
is in bytes 2-5*/

strcat(Code[*row+tmpR], "0000");
                                            }
                                        }
                                        mItoa(ptr->addr, str);
                                        convertToBinary(str);
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
                                        ptr =
searchSymbol(symbList->head, tempArr[0]);
                                        mItoa(ptr->addr, temp);
                                        convertToBinary(temp);
                                        strcat(Code[*row+tmpR], temp);
                                        tmpR++;
                                        strcat(Code[*row+tmpR],
tempArr[2]);/* dest register is in bytes 2-5*/
                                        strcat(Code[*row+tmpR],
tempArr[1]);/* source register is in bytes 6-9*/
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
                                            strcat(Code[*row+tmpR],
getRegister(p->vars[indx]));
                                            strcat(Code[*row+tmpR], "00\0");
                                        }
                                        else if(regFlag == 1)
                                        {
                                            tmpR--;
                                            strcat(Code[*row+tmpR],
getRegister(p->vars[indx]));/* dest register is in bytes 2-5*/
                                            strcat(Code[*row+tmpR],
getRegister(p->vars[indx-1]));/* source register is in bytes 6-9*/
                                            strcat(Code[*row+tmpR], "00\0");
                                        }
                                        if(NumFlag == 1)
                                        {
                                            strcat(Code[*row+tmpR],
getRegister(p->vars[indx]));
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
                                           strcat(Code[*row+tmpR],
getRegister(p->vars[indx]));
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

     if ((sign = n) < 0)  /* record sign */
         n = -n;          /* make n positive */
     i = 0;
     do {       /* generate digits in reverse order */
         s[i++] = n % 10 + '0';   /* get next digit */
     } while ((n /= 10) > 0);     /* delete it */
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

    int i, j;
    char* tempStr = "";
    for(i=0, j=0; j < MAX_CMD; j++)
    {
     if(currcmd == cmdArr[i][j])
     {
      i += 1;
      tempStr = strcat(tempStr, cmdArr[i][j]); /* concatenate the
binary "base 4" number of the right command to a temporary string*/
     }
    }
    return tempStr;
}

void parseMat(char* mat){
    int i, j, k;
    for(i = 0; i < strlen(mat); i++){
        if(mat[i] == '['){
            j++;
            k = 0;
        }
        else if(mat[i] == ']'){
            continue;
        }
        else{
            switch(j){
                case 0: name[k] = mat[i];
                        k++;
                        break;
                case 1: R1[k] = mat[i];
                        k++;
                        break;
                case 2: R2[k] = mat[i];
                        k++;
                        break;
            }
        }
    }
}

/*-----validate.c----*/

int typeAdress(SplitLine* sl, int index)
{
    if(isNumber(sl->vars[index]) != 0 )
        return IMMEDIATE_ADRESS;/*"00" */
        if(isLetter(sl->vars[index]) != 0)
        return DIRECT_ADRESS;/*"01" */
            if (isMatrix(sl->vars[index]) != 0)
        return MAT_ADRESS; /*"10" */
                if(isRegister(sl->vars[index]) != 0)
        return DIRECT_REG;/* "11" */
                    else return NULL;
}

int isNumber(const char* str)
{
    char c;
    if (*str) {
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
int isLetter(char c)
{
    if ((c>='A' && c<='Z') || (c>='a' && c<='z'))
        return 1;
    else return 0;
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
    SymbolTable* head;
    if(isEmpty(word) == 1)
    {
        printf("ERROR! Label is empty!\n");
        return 0;
    }

    if(!isalpha(word[0]))
    {
        printf("ERROR! Label must start with letters.");
        return 0;
    }
/* check the length of the label until ':'*/
    while(word[i] != ':')
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
    for(i=0; i < OPSIZE; i++)
    {
        if(strcmp(word, opCodes[i]) == 0)
        {
            printf("ERROR! Label [%s] is a saved word!\n", word);
            return 0;
        }
    }
    return 1;
}

/* isMatrix get a word and validate if it's a llegal matrix - return 1
if success*/
int isMatrix(char* word)
{
    int i, j, flagOpen, flagClose;
    char* tempStr;
    if(isOpenBracket(word[i]) == 1)
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with
alphabetic character.\n");
        return 0;
    }
    if(isdigit(word[i]))
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with
alphabetic character.\n");
        return 0;
    }
    if(!(isalpha(word[i])))
    {
        printf("ERROR! Invalid use of matrix. Matrix must start with
alphabetic character.\n");
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
        printf("ERROR! Invalid use of matrix. Matrix must start with
alphabetic character.\n");
        return 0;
    }
    if(flagOpen == 1)
    {
        j = 0;
/* sepperate the words after a llegal matrix in order to check the next word*/
        breakMat(word);

        if(isRegister(tempStr) != 1)
        {
            printf("ERROR! Invalid use of matrix. Matrix must contain
llegal register.\n");
            return 0;
        }
        else
        {
            i++;
            if(isCloseBracket(word[i]) != 1)
            {
                printf("ERROR! Invalid use of matrix. Matrix must
contain llegal register.\n");
                return 0;
            }
            else
            {
                flagClose = 1;
                i++;
            }
            if(isOpenBracket(word[i]) != 1)
            {
                printf("ERROR! Invalid use of matrix. second matrix
must start with square bracket and must be beside first matrix.\n");
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
                printf("ERROR! Invalid use of matrix. second matrix
must contain llegal register.\n");
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
                    printf("ERROR! Invalid use of matrix. After
declaration of matrix line must be empty.\n");
                    return 0;
                }
            }
        }
    }
/* last check to see that we really got lleagal matrix by matching
closing and opening square brackets */
    if(flagOpen == 2 && flagClose == 2)
        return 1;
    else
        return 0;
}


/* isMatrix get a word and validate if it's a llegal matrix - return 1
if success*/
int isMatrixObject(char* word)
{
    int i, firstNumber, secondNumber;
    if(isOpenBracket(word[i]) != 1 && isdigit(word[i+1]) == 1 )
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix
format should be [positive number][positive number].\n");
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
        printf("ERROR! Invalid declaration of Matrix Object - matrix
format should be [positive number][positive number].\n");
        return 0;
    }
    i++;
    if(isOpenBracket(word[i]) != 1 && isdigit(word[i+1]) == 1 )
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix
format should be [positive number][positive number].\n");
        return 0;
    }
    i++;
    while (isdigit(word[i]) == 1)
    {
        secondNumber = (secondNumber * 10) + word[i];
        i++;   }
        if (isCloseBracket(word[i]) !=1)
        {
            printf("ERROR! Invalid declaration of Matrix Object -
matrix format should be [positive number][positive number].\n");
            return 0;
        }
        i++;
        if(word[i] != '\0')/* different from NULL*/
        {
            printf("ERROR! Invalid declaration of Matrix Object -
matrix format should be [positive number][positive number].\n");
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
    strcat(&array[i], str);
    while(array[i]!= '\0')/* different from NULL('\0')*/
    {
        strcat(&array[++i], strtok(NULL," "));
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
    strcat(&array[i], strtok(word,","));
    while(array[i]!= '\0')
    {
        strcat(&array[++i], strtok(word,","));
    }

/*validate matrix size*/
/*check if number of elements is less or equal to matrixSize*/
    NumberOfElements = sizeof(array)/sizeof(array[0]);
    if ( NumberOfElements > matrixSize)
    {
        printf("ERROR! Invalid declaration of Matrix elements - number
of elements [%d] should match matrix size [%d].\n", NumberOfElements,
matrixSize);
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
            printf("ERROR! Invalid declaration of Matrix elements.
elements should be only numbers. failed on element [%d] \n",
array[i]);
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
    while(word[i] != ']' && i <= strlen(word))
    {
        tempStr[j] = word[i];
        i++;
        j++;
    }
}

int validData(char* var)
{
    int i = 0, size;
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
        printf("ERROR! elemnt: [%d] in .data must be a valid
number!\n", var[i]);
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
        if((input[i] == '-' || input[i] == '+') && (last == '-' ||
last == '+' )){
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
{   int i, size;
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
        printf("ERROR! string must start with \" ! Instead it starts
with: [%d]\n", var[i]);
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

int validEntry(char* word, SymbolTable* head)
{
    if(isEmpty(word) == 1)
    {
        printf("ERROR! line after '.entry' is empty!\n");
        return 0;
    }

    if(head == NULL)
    {
        printf("ERROR! Symbol after .entry must exist in symbol list!
%s does'nt exist!\n", word);
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

int validExtern(char* word, SymbolTable* head)
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
        for(i = 0; i < MAX_OPERAND; i++)
        {
            if(typeAdress(sl, i) == IMMEDIATE_ADRESS || typeAdress(sl,
i) == DIRECT_ADRESS || (isVar(sl->vars[i]) == 1))
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

SplitList* makeSplitList(){
    SplitList* lst;
    lst->head = lst->tail = NULL;
    return lst;
}

int isEmptyList(const SplitList* lst){
    return lst->head == NULL;
}

void insertLineToTail(SplitList* lst, SplitLine* line){
    if(isEmptyList(lst)){
        lst->head = lst->tail = line;
    }
    else{
        lst->tail->next = line;
        lst->tail = line;
    }
}

void freeList(SplitList* list){
    SplitLine* curr = list->head;
    SplitLine* next;
    while(curr){
        next = curr->next;
        free(curr);
        curr = next;
    }
    list->head = NULL;
}

SymbolList* makeSymbolList(){
    SymbolList* lst;
    lst->head = lst->tail = NULL;
    return lst;
}

int isEmptysList(const SymbolList* lst){
    return lst->head == NULL;
}

void insertSymbolToTail(SymbolList* lst, SymbolTable* symbol){
    if(isEmptyList(lst)){
        lst->head = lst->tail = symbol;
    }
    else{
        lst->tail->next = symbol;
        lst->tail = symbol;
    }
}

SymbolTable* searchSymbol(SymbolTable* tmp, char* symbol){
    SymbolTable* p = tmp;
    while(p){
        if((strcmp(p->label, symbol)) == 0){
            return p;
        }
        p = p->next;
    }
    return NULL;
}
