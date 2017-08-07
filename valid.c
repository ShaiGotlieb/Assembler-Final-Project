#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IMMEDIATE_ADRESS 00
#define DIRECT_ADRESS 01
#define MAT_ADRESS 10
#define DIRECT_REG 11

char* adress_type(char*);
int checkSpaces(char*)

char *comma;
char *position;


char* getAddressTypeByGroup(char* command, char* word)
{
	char* result;
	if (command == "mov" || command == "cmp" || command == "add" || command == "sub" || || command == "lea")/*group 1 commands*/
		result = handleGroup1Command(word);
	if (command == "not" || command == "clr" || command == "inc" || command == "dec" || command == "jmp" || command == "bne" 
		|| command == "red" || command == "prn" || command == "jsr" ) /*group 2 commands */
		result = getAddressTypeForGroup2(command, word);
	if (command == "stop" || command == "rts") /*group 3 commands */
		result = getAddressTypeForGroup3(command, word);

	return result;
}

/*char* getAddressTypeForGroup1(char* command, char* word)
{

	switch (command){
		case "mov":
			result = handleGroup1Command(word);
			break;
		case "cmp":
			result = handleGroup1Command(word);
			break;
		case "add":
			result = handleGroup1Command(word);
			break;
		case "sub":
			result = handleGroup1Command(word);
			break;
		case "lea":
			result = handleGroup1Command(word);
			break;
		default:
			result = null;
			break;
		}	
		return result;
	
}*/

//get command with commas and validate move is ok with given word
char* handleGroup1Command(char* word)
{
	char* res1, res2;
	char* tempStr = "";
	/* split to a word by a comma*/
	res1 = split_line(word);
	/* Position is now the comma*/
    position++;
    comma = strchr (position, ',');
	res2 = split_line(word);
	if(res1 != NULL && res2 != NULL)/* check if there are 2 operands - check the size of the array, number of words!!!!!!*/	
	{
	if((isRegister(res1) != 0) && (isRegister(res2) != 0 ))/* check if they are 2 registers*/
	{
		tempStr = (tempStr, DIRECT_REG);
		tempStr = (tempStr, DIRECT_REG);
		return tempStr; /* return '1111'*/
	}
	else if((isNumber(res1) != 0 ) && (isResgiter(res2) != 0))/* check if first is a number and second is registers*/
		{
		tempStr = (tempStr, IMMEDIATE_ADRESS);
		tempStr = (tempStr, DIRECT_REG);
		return tempStr;
		}
	else if ((isLetter(res1) != 0) && isResgiter(res2) != 0)/* check if first word is variable and second is registers*/
		{
		tempStr = (tempStr, DIRECT_ADRESS);
		tempStr = (tempStr, DIRECT_REG);
		return tempStr;
		}
		/* I should probably check for more optiones: mov A,B \ mov #1, A \ mov r1, A, mov M[r1],[r2] etc..*/
	else return NULL;
	}
}

char* getAddressTypeForGroup2(char* command, char* word)
{

	switch (command){
		case "not":
			result = handleNotCommand(word);
			break;
		case "clr":
			result = handleClrCommand(word);
			break;
		case "inc":
			result = handleIncCommand(word);
			break;
		case "dec":
			result = handleDecCommand(word);
			break;
		case "jmp":
			result = handleJmpCommand(word);
			break;
		case "bne":
			result = handleBneCommand(word);
			break;
		case "red":
			result = handleRedCommand(word);
			break;
		case "prn":
			result = handlePrnCommand(word);
			break;
		case "jsr":
			result = handleJsrCommand(word);
			break;
		default:
			result = null;
			break;
		}
		return result;
	}

// handle not command
char* handleNotCommand(char* word)
{
	
	/* TODO: check size of operands, suppose to have just one operand*/ 
		if(isRegister(word) != 0)
			{
				tempStr = (tempStr, IMMEDIATE_ADRESS);
				tempStr = (tempStr, DIRECT_REG);
			return tempStr;
			}
		if(isNumber(word) != 0)
			{
				tempStr = (tempStr, IMMEDIATE_ADRESS);
				tempStr = (tempStr, IMMEDIATE_ADRESS);
			return tempStr;
		}
		if(isLetter(word) != 0)
			{
				tempStr = (tempStr, IMMEDIATE_ADRESS);
				tempStr = (tempStr, DIRECT_ADRESS);
			return tempStr;
		}
		else return NULL;
}

char* getAddressTypeForGroup3(char* command, char* word)
{
	char* result;
	/* TODO: check number of operands. no operands in this group*/
	switch (command){
		case "rts":
			result = '0000';
			break;
		case "stop":
			result ='0000';
			break;
		default:
			result = NULL;
			break;
	}
		return result;
}


int isNumber(const char* str)
{
    if (*str) {
    	 if((*str == '-' || *str == '+') && *str == '#') /* minus and plus is  allowed, number must start with # */
    		 str++;
         char c;
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

char* split_line(char* line)
{
char temp[31];
comma = strchr(line, ',');
position = string;
while(comma) 
{
    int i = 0;

    while(position < comma && i <= 31) {
        temp[i] = *position;
        i++;
        position++;
    }

    /* Add a NULL to the end of the string*/
    temp[i] = '\0';
}
 return temp;
}
