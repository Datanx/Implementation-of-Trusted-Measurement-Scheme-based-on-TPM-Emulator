#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lib_StringAdd.h"

void RTrim(IN char* src, IN int srclen, OUT char* outstr)
{
	char *ptr = NULL;
	char *ptr2 = NULL;
	int i=0;
	
	if(src != NULL && outstr != NULL)
	{
		ptr = src + srclen - 1;
		while(is_invisible_char(*ptr))
		{
			ptr--;
		}
		
		strncpy(outstr, src, ptr-src + 1);
		ptr2 = outstr + (ptr-src+1);
		*ptr2='\0';
	}
}

void LTrim(IN char* src, OUT char* outstr)
{
	char *ptr = NULL;
	int i=0;
	
	if(src != NULL && outstr != NULL)
	{
		ptr = src;
		while(is_invisible_char(*ptr) == 1)
		{
			ptr++;
		}
		
		//Copy the LTrimed string to outstr
		strcpy(outstr, ptr);
	}
}

int is_invisible_char(char ch)
{
	if(ch=='\r' || ch=='\n' || ch=='\t' || ch==' ')
		return 1;
	else
		return 0;
}

int Str2Int(char* str)
{
   if(strstr(str,"0x") || strstr(str,"0X"))
   {
      str += 2;
      int tmp = 0;
      int len = strlen(str);
      int i = 0;
      for(i = 0;i < len ;i++)
      {
         int nDecNum;
         switch(str[i])
         {
        case 'a':
        case 'A':
           nDecNum = 10;
           break;
        case 'b':
        case 'B':
           nDecNum = 11;
           break;
         case 'c':
         case 'C':
            nDecNum = 12;
            break;
        case 'd':
        case 'D':
            nDecNum = 13;
            break;
        case 'e':
        case 'E':
            nDecNum = 14;
            break;
        case 'f':
        case 'F':
            nDecNum = 15;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            nDecNum = str[i] - '0';
            break;
        default:
            return 0;
        }
        tmp += nDecNum * pow(16,len-i-1);
     }
     return tmp;
   }
   else
   {
      return atoi(str);
   }
}
