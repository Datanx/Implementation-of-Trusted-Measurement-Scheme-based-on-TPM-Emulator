%{
#include <stdio.h>
#include "y.tab.h"

#define YYSTYPE char*

extern void yyerror(char *);
extern int yylex(void);

extern YYSTYPE yylval;
%}
%%
set|SET		return SET;
policy|POLICY	return POLICY;
policy_val|POLICY_VAL	return POLICY_VAL;
on|ON	return ON;
begin|BEGIN		return BEGIN;
end|END			return END;
as|AS			return AS;
machine|MACHINE	return MACHINE;
domain|DOMAIN	return DOMAIN;
service|SERVICE	return SERVICE;
not|NOT			yylval=yytext;return NOT;
and|AND			yylval=yytext;return AND;
or|OR			yylval=yytext;return OR;
[A-Za-z_][A-Za-z0-9_]+	yylval=yytext;return IDENTIFIER;
\"	return QUOTE;
[()]	return *yytext;	/*Return ( or ) directly*/
\/\*.*\*\/		;/*Ignore Comments*/
\/\/.*\n			;/*Ignore Comments*/
[ \t\n]		;/*Ignore*/
.	yyerror("Unknown character");
%%

int yywrap(void)
{
	return 1;
}
