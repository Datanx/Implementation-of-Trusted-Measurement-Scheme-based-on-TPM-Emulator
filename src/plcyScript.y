%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "plcyScript.h"

extern int yylex(void);
extern int yyparse(void);
extern void yyerror(char *);

char* extractMachineName(char *);	/*Extract machine name from the IDENTIFIER*/
char* extractServiceName(char *);	/*Extract machine name from the IDENTIFIER*/
char* extractLogicString(char *);	/*Extract machine name from the IDENTIFIER*/

/*Global Variables*/
char gMachineName[PSI_BUFFER_LEN]={0};		/*To store machine or domain name*/
char gMachineOrDomain[PSI_BUFFER_LEN]={0};	/*Indicate machine or domain*/
SRV_PLCY gPsiArray[PSI_BUFFER_LEN]={		/*To store service name and its relavant policy string that extracted from policy script*/
	{0,0}
};
int gPsiArrayIndex = 0;						/*Global index of gPsiArray*/

extern FILE* yyin;
%}
%token IDENTIFIER QUOTE
%token SET POLICY POLICY_VAL ON BEGIN END AS MACHINE DOMAIN SERVICE 
%left  AND OR
%right NOT
%%
program:
	SET POLICY ON MACHINE IDENTIFIER BEGIN policy_stmts END
	{
		char* mchn_name = extractMachineName((char*)$5);
		/*printf("%s\n", mchn_name);*/
		strcpy(gMachineName, mchn_name);
		strcpy(gMachineOrDomain, "MACHINE");
		free(mchn_name);
	}
	|SET POLICY ON DOMAIN IDENTIFIER BEGIN policy_stmts END
	{
		char* dom_name = extractMachineName((char*)$5);
		/*printf("%s\n", dom_name);*/
		strcpy(gMachineName, dom_name);
		strcpy(gMachineOrDomain, "DOMAIN");
		free(dom_name);
	}
	|error _PROG_ERR
		{yyerrok;yyclearin;}
	;
policy_stmt:
	SET POLICY_VAL ON SERVICE IDENTIFIER AS QUOTE policy_str QUOTE
	{
		char* srv_name = extractServiceName((char*)$5);
		char* logic_str = extractLogicString((char*)$8);
		strcpy(gPsiArray[gPsiArrayIndex].srv_name, srv_name);
		/*printf("%s  %s\n", srv_name, logic_str);*/
		strcpy(gPsiArray[gPsiArrayIndex].plcy_str, logic_str);
		gPsiArrayIndex++;
		free(srv_name);
		free(logic_str);
	}
	;
policy_stmts:
	policy_stmt
	|policy_stmts policy_stmt
	;
policy_str:
	IDENTIFIER		{}
	|NOT policy_str		{}
	|policy_str AND policy_str	{}
	|policy_str OR policy_str	{}
	|'(' policy_str ')'			{}
	;
%%

void yyerror(char *s)
{
	printf("ERROR: %s\n", s);
}

char* extractMachineName(char *s)
{
	int i = 0;
	char* retStr = NULL;
	
	while(*(s+i) != '\0')
	{
		if(*(s+i) == '\n' || *(s+i) == ' ' || *(s+i) == '\t')
		{
			retStr = (char*)malloc((i+1) * sizeof(char));
			memset(retStr, 0, i+1);
			memcpy(retStr, s, i);
			return retStr;
		}
		i++;
	}
	
	return retStr;
}

char* extractServiceName(char *s)
{
	return extractMachineName(s);
}

char* extractLogicString(char *s)
{
	int i = 0;
	char* retStr = NULL;
	
	while(*(s+i) != '\0')
	{
		if(*(s+i) == '"')
		{
			retStr = (char*)malloc((i+1) * sizeof(char));
			memset(retStr, 0, i+1);
			memcpy(retStr, s, i);
			return retStr;
		}
		i++;
	}
	
	return retStr;
}
