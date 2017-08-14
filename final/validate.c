int typeAdress(splitLines* sl, int index)
{
    if(isNumber(sl.vars[index]) != 0 ) 
        return IMMEDIATE_ADRESS;/*"00" */
        if(isLetter(sl.vars[index]) != 0)
        return DIRECT_ADRESS;/*"01" */
            if (isMatrix(sl.vars[index]) != 0)
        return MAT_ADRESS; /*"10" */
                if(isRegister(sl.vars[index]) != 0)
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
    if((word[0] == 'r') && (word[1] >= 0) && (word[1] <= 7) && (strlen(word) == 2))
return 1; /* success */
        else
return 0; /* not valid register - print error outside of the function */
}
 
int validLabel(char* word)
{
    int i = 0;
    int labLength = 0;
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
 
/* isMatrix get a word and validate if it's a llegal matrix - return 1 if success*/
int isMatrix(char* word)
{
    int i, j, flagOpen, flagClose;
    char* tempStr;
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
        breakMat();
 
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
            breakMat();
 
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
/* last check to see that we really got lleagal matrix by matching closing and opening square brackets */
    if(flagOpen == 2 && flagClose == 2)
        return 1;
    else
        return 0;
} 
 
 
/* isMatrix get a word and validate if it's a llegal matrix - return 1 if success*/
int isMatrixObject(char* word)
{
    int i;
    if(isOpenBracket(word[i]) != 1 && isdigit(word[i+1]) == 1 )
    {
        printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
        return 0;
    }
    int firstNumber = 0;
    int secondNumber = 0;
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
        if (word[i] != NULL) 
        {
            printf("ERROR! Invalid declaration of Matrix Object - matrix format should be [positive number][positive number].\n");
            return 0;
        }
        return firstNumber*secondNumber;
    }
 
 
    int validateMatCommandObject(char* word)
    {
/*split word by space*/
        char *array[strlen(word)];
        int NumberOfElements = 0;
        int i=0;
        array[i] = strtok(word," ");
        int matrixSize;
        int result;
        while(array[i]!=NULL)
        {
            array[++i] = strtok(NULL," ");
        }
/*validate matrix size*/
        NumberOfElements = sizeof(array)/sizeof(array[0]);
 
        if (NumberOfElements >2) 
        {
            printf("ERROR! Invalid use of matrix. Too many spaces in word.\n");
            return 0 ;
        }
        matrixSize = isMatrixObject(array[0]);
        if (matrixSize == 0)
        {
            printf("ERROR! Invalid use of matrix.\n", );
            return 0;
        }
        result = isMatrixInputValid(array[1], matrixSize);
        if ( result == 0)
        {
// error with matrix elements input
            printf("ERROR! Invalid use of matrix. Invalid matrix element input.\n");
            return 0;
        }
 
        return matrixSize;
 
    }
/* isMatrixInputValid*/
    int isMatrixInputValid(char* word, int matrixSize)
    {
 
/*split word by commas*/
        char *array[matrixSize];
        int i=0;
        int NumberOfElements = 0;
        int value = 0;
        array[i] = strtok(word,",");
        while(array[i]!=NULL)
        {
            array[++i] = strtok(NULL,",");
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
        i = 0
        while ( array[i] != NULL)
        {
/*0 is a valid number but atoi returns 0 on error so we need to check it seperatly */
            if ( array[i] != '0' && atoi(array[i]) == 0) 
            {
                printf("ERROR! Invalid declaration of Matrix elements. elements should be only numbers. failed on element [%s] \n", array[i]);
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
    void breakMat(void)
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
 
/* validString check if a string after ".string" is lligal and return the size of the string (in order to increase DC)
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
 
int validEntry(char* word, Symblist* head)
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
 
int validExtern(char* word, Symblist* head)
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
            printf("ERROR! Local label cannot be declared as external.\n")
            return 0;
        }
        head = head->next;
    }
    return 1;
}
/* Function to increase L by the type adress of the variable. return L which is the number of memory to increase.*/
int memorySize(splitLines* sl)
{   
    int L = 1, regFlag;
    if(sl.opCode == NULL)
    {
        return 0;
    }
    else
    {
        for(i = 0; i < 3)
        {
            if(typeAdress(sl.var[i]) == 0 || typeAdress(sl.var[i]) == 1 || (isVar(sl.var[i]) == 1))
            {
                L++;
            }
            if(typeAdress(sl.var[i]) == 2)
            {
                L += 2
            }
            if(typeAdress(sl.var[i]) == 3)
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
 
int isVar(char* word)
{
    int i = 0;
    while(i < strlen(word))
    {
        c = word[i];
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || isdigit(c) != 0)
                i++;
                else return 0;
    }
    return 1;
}
