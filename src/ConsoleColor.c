#include "ConsoleColor.h"

void Console_SetTextColor(char* color, char *bg_color)
{
	int commandlen = strlen(color) + strlen(bg_color) + 5;
	char* command = malloc(commandlen*sizeof(char));
	bzero(command,commandlen);
	strcat(command,CONSOLECOLOR_COMMAND_PREFIXES);
	strcat(command,color);
	strcat(command, ";");
	strcat(command, bg_color);
	strcat(command,CONSOLECOLOR_COMMAND_SUFFIXES);
	fprintf(stdout,command);
	free(command);
}

void Console_SetTextColorDefault()
{
	char* command = malloc(5*sizeof(char));
	bzero(command,5);
	strcat(command,CONSOLECOLOR_COMMAND_PREFIXES);
	strcat(command,CONSOLECOLOR_COMMAND_DEFAULT);
	strcat(command,CONSOLECOLOR_COMMAND_SUFFIXES);
	fprintf(stdout,command);
	free(command);
}
