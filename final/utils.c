void readLine(SplitLine* data, const char* line){
    int size = strlen(line);
    int i, j=0, index, t=0;
    int status = SPACE;
    int mainStatus = START;
    int symbolflag = 0;
    int secondVarFlag = 0;
    int commaflag = 0;
    char* temp = (char*)malloc(sizeof(char));
    if(mallocValid(temp)) return;
 
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
                        if(mallocValid(temp)) return;
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
                    if(mallocValid(temp)) return;
                    temp[j+1] = '\0';
 
                    if(mainStatus == START){
                        if(temp[j] == ':'){
                            temp[j] = '\0';
 
                            status = LABEL;
                            break;
                        }
                        else{
                            index = validOpCode(temp);
                            switch(index){
                                case 0: printf("ERROR: invalid command\n");
                                return;
                                case 1: case 2:
                                        status = mainStatus = COMM_ZERO;
                                        data->opCode = (char*)malloc(sizeof(temp));
                                        if(mallocValid(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11:
                                        status = SPACE;
                                        mainStatus = COMM_ONE;
                                        data->opCode = (char*)malloc(sizeof(temp));
                                        if(mallocValid(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 12: case 13: case 14: case 15: case 16:
                                        status = SPACE;
                                        mainStatus = COMM_TWO_A;
                                        data->opCode = (char*)malloc(sizeof(temp));
                                        if(mallocValid(data->opCode)) return;
                                        strcat(data->opCode, temp);
                                        free(temp);
                                        temp = (char*)malloc(sizeof(char));
                                        j = 0;
                                break;
                                case 17: case 18: case 19: case 20: case 21:
                                /* if its string all between "" enter togheter - to fix*/
                                        status = SPACE;
                                        mainStatus = INST;
                                        data->opCode = (char*)malloc(sizeof(temp));
                                        if(mallocValid(data->opCode)) return;
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
                    if(validOpCode(temp) != 0){ /*shai code instead validOpCode*/
                        printf("ERROR: invalid symbol name\n");
                        return;
                    }
                    else{
                        data->label = (char*)malloc(sizeof(temp));
                        if(mallocValid(data->label)) return;
                        strcat(data->label, temp);
                        symbolflag = 1;
                        free(temp);
                        temp = (char*)malloc(sizeof(char));
                        j = 0;
                        status = SPACE;
                        break;
                    }
 
                case COMM_ZERO:
                    data->vars = NULL;
                    status = END;
                    break;
 
                case COMM_ONE:
                    if(validOperand(data->opCode, temp)){
                        data->vars[t] = (char*)malloc(sizeof(temp));
                        if(mallocValid(data->vars[t])) return;
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
                    if(mallocValid(data->vars[t])) return;
                    strcat(data->vars[t], temp);
                    free(temp);
                    temp = (char*)malloc(sizeof(char));
                    t++;
                    status = SPACE;
                    mainStatus = COMM_TWO_B;
                    break;
 
                case COMM_TWO_B:
                    data->vars[t] = (char*)malloc(sizeof(temp));
                    if(mallocValid(data->vars[t])) return;
                    strcat(data->vars[t], temp);
                    free(temp);
                    t = 0;
                    status = END;
                    break;
 
                case INST: 
                    data->vars[t] = (char*)malloc(sizeof(temp));
                    if(mallocValid(data->vars[t])) return;
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
                        printf("The %s command have excesive arguments\n", mainOp);
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
        if((strcmp(op, opCodes[i])) == 0)
            return i+1;
        return 0;
    }
}
 
int validOperand(int op, char* tmp){
    int index;
    index = operkind(tmp);
    switch(op){
        case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 11:
            if(index == 1 || index == 2 || index == 3)
                    return 1;
            return 0;
        case 10: case 13:
            if(index == 0 || index == 1 || index == 2 || index == 3)
                return 1;
            return 0;
        case 12: case 14: case 15:
            if(secondVarFlag == 0){
                if(index == 0 || index == 1 || index == 2 || index == 3){
                    secondVarFlag = 1;
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                if(index == 1 || index == 2 || index == 3){
                    secondVarFlag = 0;
                    return 1;
                }
                else{
                    return 0;
                }
            }
        case 16:
            if(secondVarFlag == 0){
                if(index == 1 || index == 2){
                    secondVarFlag = 1;
                    return 1;
                }
                else{
                    return 0;
                }
            }
            else{
                if(index == 1 || index == 2 || index == 3){
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
 
void insertToDataT(char vars[], int index){
    int i, j;
    int size;
 
    switch(index){
        case 17:
            size = sizeof(vars)/sizeof(vars[0]);
            for(i = DC, j = 0; i < MAX_MEMORY || j < size; i++, j++){
                copyBinarStr(DataT[i], convertToBinary(vars[j]));
            }
            DC = i;
            break;
 
        case 18:
            size = strlen(vars[0]);
            for(i = DC, j = 0; i < MAX_MEMORY || j < size; i++, j++){
                copyBinarStr(DataT[i], convertToBinary(vars[0][j]));
            }
            copyBinarStr(DataT[++i], convertToBinary(0));
            DC = i;
            break;
 
        case 19:
            size = sizeof(vars)/sizeof(vars[0]);
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
 
void createFileName(const char* origName, char* fName, char* extension)
{
    fName = (char*)malloc(sizeof(origName));
    if(mallocValid(fName)) return;
    strcat(fName, origName);
    strcat(fName, extension);
}
 
char* convert_base4(char* inputNum)
{
int num = atoi(inputNum);
char* result[DECIMAL] = '\0';
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
return result;
}
 
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
 
char* convertToBinary(char* inputNum)
{
    if(strstr(inputNum, '#') || strstr(inputNum, '-') || strstr(inputNum, '+'))
    {
        strtok(inputNum, '#');
        strtok(inputNum, '-');
        strtok(inputNum, '+');
    }
    int num = atoi(inputNum);
    char* result[DECIMAL] = '\0';
    int index = 7;
    while(num > 0)
    {
        if(num % 2 != 0)
        {
            result[index] = 1;
        }
        else{
            result[index] = 0;
        }
        index--;
        num /= 2;
    }
    return result;
}

int insertToCode(SplitLine p, int* r)
{
    int index = 0, type = 0, regFlag = 0, row = r, tmpR = 1;
    char* temp, str;
    SymbolTable* ptr;
    char* tempArr[3];

    temp = cmdToCode(p->opCode);
    strcat(Code[i], temp);
        
    
    
        
        while(row < MAX_MEMORY && p->vars[index] != NULL)
        {
            type = typeAdress(p, index);
            switch(type)
            {
                case IMMEDIATE_ADRESS:   str = convertToBinary(p->vars[index]);
                                         strcat(Code[row], "00");
                                         strcat(Code[row+tmpR], str);   
                                         index++;
                                         tmpR++;
                                         break;

                case DIRECT_ADRESS:     strcat(Code[row], "01");
                                        ptr = searchSymbol(symbList, p->label);
                                        str = convertToBinary(ptr->addr);
                                        strcat(Code[row+tmpR], str);
                                        index++;
                                        tmpR++;
                                        break;
                    
                case MAT_ADRESS:        strcat(Code[row], "10");
                                        tempArr = parseMat(p->vars[index]);/* malloc*/
                                        ptr = searchSymbol(symbList, tempArr[0]);
                                        temp = convertToBinary(ptr->addr, type);
                                        strcat(Code[row+tmpR], temp);
                                        tmpR++;
                                        strcat(Code[row+tmpR], tempArr[1]);
                                        strcat(Code[row+tmpR], tempArr[2]);
                                        index++;
                                        tmpR++;
                                        break;

                case REG_ADRESS:        strcat(Code[row], "11");
                                        if(regFlag == 1)
                                        {
                                            tmpR--;
                                            strcat(Code[row+tmpR], getRegister(p->vars[index]));
                                        }
                                        strcat(Code[row+tmpR], getRegister(p->vars[index]));
                                        regFlag = 1;
                                        index++;
                                        tmpR++;
                                        break;                        
            }
        }
    
}
