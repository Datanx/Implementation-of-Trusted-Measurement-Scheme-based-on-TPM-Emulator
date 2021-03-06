#ifndef _CONSOLECOLOR_H
#define _CONSOLECOLOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CONSOLECOLOR_COMMAND_PREFIXES "\033["
#define CONSOLECOLOR_COMMAND_SUFFIXES "m"
#define CONSOLECOLOR_COMMAND_DEFAULT "0"

#define CONSOLECOLOR_BLACK "30"
#define CONSOLECOLOR_RED "31"
#define CONSOLECOLOR_GREEN "32"
#define CONSOLECOLOR_YELLOW "33"
#define CONSOLECOLOR_BULE "34"
#define CONSOLECOLOR_PURPLE "35"
#define CONSOLECOLOR_CYAN "36"
#define CONSOLECOLOR_WHITE "37"

#define CONSOLECOLOR_BG_BLACK "40"
#define CONSOLECOLOR_BG_RED "41"
#define CONSOLECOLOR_BG_GREEN "42"
#define CONSOLECOLOR_BG_YELLOW "43"
#define CONSOLECOLOR_BG_BULE "44"
#define CONSOLECOLOR_BG_PURPLE "45"
#define CONSOLECOLOR_BG_CYAN "46"
#define CONSOLECOLOR_BG_WHITE "47"

void Console_SetTextColor(char*, char *);
void Console_SetTextColorDefault();

#endif
