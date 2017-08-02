#include <stdio.h>
#include <string.h>
#include <ctype.h>

int read_line(char * line, command* cmd, instruction* tmpInst)
{
	void* currCmd;
	char* newStr;
	char* tempLine[81]; /* words in line must be less than 80 + 1 for '/0' */
	int numOfWords=0;
	int data_flag, entry_flag, extern_flag;
	int length;
	char* isDot;
	int i, j, counter;
	int check = 0;
	/*for logic of the loop*/
	int check_status=0;
	/*count the lines of memory*/
	static int cline =1;

	newStr = strtok (line," \t\n");/* remove spaces & \n */
	while (newStr != NULL) /* while is not empty*/
	{
		tempLine[numOfWords] = newStr; /* temporary array get the new string*/
		if(strstr(newStr,";") && numOfWords == 0) /* if it begins with ';' */
		{
			return 0;
		}
		newStr = strtok(NULL, " ,\t\n"); /* terminate spaces and ',' */
		numOfWords++;
	}

	if(numOfWords == 0)
	{
		return 0;
	}

	if(numOfWords > 0) /*is the first word a lable? */
	{
		if (strstr(tempLine[0],":")) /* check if it contains ':'*/
		{
			/* do something HERE!*/
		}
		else
		{
			if(strstr(tempLine[0],".data") && numOfWords >= 0)/* check if the first word is ".data"*/
			{
				data_flag++;/* turn on data flag to check if it appears twice*/
				strcpy(tmpInst->instr, tempLine[0]);
				tmpInst->data=(char*)malloc(sizeof(char)*numOfWords);
				for (i=0; i < numOfWords; i++)
				{
					tmpInst->data[i] = tempLine[i];
				}
			if(cmdValidation(line) == 0)
				printf("Error! %s appears more than one time\n", [j]);
				return 2;
			}

			if(strstr(tempLine[0],".entry") && numOfWords >= 0)/* check if the first word is ".entry"*/
			{
				entry_flag++;
				strcpy(cmd->lableName, tempLine[0]);
				strcpy(tmpInst->instr, tempLine[0]);
			j = 1;
			while(j < numOfWords && entry_flag != 0 )
			{
			if(strstr(tempLine[j],".entry") && numOfWords > 1)
			{
				printf("Error! .entry appears more than one time\n");
			}
			}
				return 3;
		    }

			if (strstr(tempLine[0],".extern") && numOfWords >= 0)/* check if the first word is ".extern"*/
			{
				extern_flag++;
				strcpy(cmd->lableName, tempLine[0]);
				strcpy(tmpInst->instr, tempLine[0]);
				j = 1;
			while(j < numOfWords && extern_flag != 0)
			{
			if(strstr(tempLine[j],".extern") && numOfWords > 1)
			{
				printf("Error! .extern appears more than one time\n");
			}
		    }
				return 3;
			}
			
		}

	}
	if(numOfWords >= 1)
	{
		
			if(strstr(tempLine[1],".string") && numOfWords >= 2) /*check if .string ? - starts from 1 because before there was a lable */
			{
				while (strstr(tempLine[2],"\"") != NULL){
				        counter++;
				        strstr(tempLine += 1,"\"");
				 }
				if(counter == 2)
				{
					strcpy(cmd->opCode, tempLine[1]);
					newStr =strtok(tempLine[2],"\"");
					counter = 0;
					while (newStr != NULL)
					{
						tempLine[counter] = newStr;
						newStr = strtok(tempLine[2], " ,\t\n");
						counter += 1;
				strcpy(instruction->data, tempLine[0]);
				return 2;
				}
				else
				{
					printf("ERROR! string doesn't contains apostraphies: \" ")
				}
			}
			
		}
		else if (strstr(words[1],"#")) /*check differnet operand options*/
		{
			newStr = tempLine[1];
			if(isalpha(newStr) != 0)/* isALPHA?*/
			{
				instruction->data = tempLine[2];
				return 1;
			}
		}
		else if(strstr(tempLine[1],"[") )
		{
			/* check if matrix*/ 
		}
	}
}
