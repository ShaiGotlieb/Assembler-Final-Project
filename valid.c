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



char* getAddressTypeByGroup(char* command, char* word)
{
	char* result;
	if (command == "mov" || ...)//group 1 commands
		result = getAddressTypeForGroup1(command, word);
	if (command == "not" || ) //group 1 commands
		result = getAddressTypeForGroup2(command, word);
	if (command == "stop" || ) //group 1 commands
		result = getAddressTypeForGroup3(command, word);

	return result;
}

char* getAddressTypeForGroup1(char* command, char* word)
{

	switch (command){
		case "mov":
			result = handleMoveCommand(word);
			break;
		case "cmp":
			result = handleCmpCommand(word);
			break;
		case "add":
			result = handleAddCommand(word);
			break;
		case "sub":
			result = handleSubCommand(word);
			break;
		case "lea":
			result = handleLeaCommand(word);
			break;
		default:
			result = null;
			break;
		}	
		return result;
	
}

//get command with commas and validate move is ok with given word
char* handleMoveCommand(char* word)
{
	// TODO res = split word by comma
	if (res size != 2)
		return null;
	if (isRegisterToResgiter(res[0],res[1]))
		return DIRECT_REG+DIRECT_REG:
	else if (isNumberToResgiter(res[0],res[1]))
		return IMMEDIATE_ADRESS;
	else if (isParameterToResgiter(res[0],res[1]))//ask what type is this
		return DIRECT_REG;
	else return null;

}

//TODO add all group 1 handleCommands

bool isNumberToResgiter(char* word1, char* word2)
{
	if word 1 is number (#-1212 or #+12312 or #324232))
		
	&&
		word 2 is register ('r0 - r7');

		return true;
		else return false;
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
	
	//if valid word
		if (word == 'r0-r7' || word == number || word == matrix)
			return '00 ' + DIRECT_ADRESS;
		else return null;
}

char* getAddressTypeForGroup3(char* command, char* word)
{
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
