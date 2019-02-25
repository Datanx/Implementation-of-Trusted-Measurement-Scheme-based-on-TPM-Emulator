#ifndef		IN
#define		IN
#endif

#ifndef		OUT
#define		OUT
#endif

#ifndef 	LIB_STRINGADD_H
#define		LIB_STRINGADD_H

//This lib provides extra string functions

//Trim the right part of a string
void RTrim(IN char* src, IN int srclen, OUT char* outstr);

//Trim the left part of a string
void LTrim(IN char* src, OUT char* outstr);

//Trim the whole string
void Trim(IN char* src, OUT char* outstr);

//Whether ch is an invisible character
int is_invisible_char(char ch);

//Convert string to integer
int Str2Int(char* str);
#endif
