#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib_StringAdd.h"
#include "CmdPackage.h"

void CmdPack_Init(IN PCMD_PACKAGE pkg)
{
	if(pkg == NULL)
		return;
		
	memset(pkg->RcgCode, 0, 32);
	pkg->CMD_MJ_CODE = CMD_MJ_NULL;
	pkg->CMD_MN_CODE = CMD_MN_NULL;
	pkg->DataBuf = NULL;
	pkg->DataBufLen = 0;
	pkg->Extension = NULL;
	pkg->ExtensionLen = 0;
}

void CmdPack_Free(IN PCMD_PACKAGE pkg)
{
	if(pkg == NULL)
		return;
	
	if(pkg->DataBuf != NULL)
		free(pkg->DataBuf);
	
	if(pkg->Extension != NULL)
		free(pkg->Extension);
	
	free(pkg);
}

int CmdPack_IsPkgValid(IN PCMD_PACKAGE pkg)
{
	return 1;
}

int CmdPack_IsMsgValid(IN char *msg)
{
	return 1;
}

char* CmdPack_Serialize(OUT int *out_msg_len, IN PCMD_PACKAGE pkg)
{
	char* ptr;
	char* buf;
	int buflen;
	int rcgcodelen;
	int nTemp;
	
	ptr = NULL;
	buf = NULL;
	buflen = 0;
	rcgcodelen = 0;
	nTemp = 0;
	
	//Calculate buf len
	rcgcodelen = strlen(pkg->RcgCode);
	buflen = rcgcodelen + sizeof(unsigned short) * 2 + sizeof(int) * 2 + pkg->DataBufLen + pkg->ExtensionLen + 2;
	buf = (char*)malloc(buflen * sizeof(char));
	memset(buf, 0, buflen);
	
	ptr = buf;
	//RcgCode
	memcpy(ptr, pkg->RcgCode, rcgcodelen + 1);	//including a '\0' character
	ptr += rcgcodelen + 1;
	
	//Major Code
	nTemp = sizeof(unsigned short);
	memcpy(ptr, &pkg->CMD_MJ_CODE, nTemp);
	ptr += nTemp;
	
	//Minor Code
	nTemp = sizeof(unsigned short);
	memcpy(ptr, &pkg->CMD_MN_CODE, nTemp);
	ptr += nTemp;
	
	//Buffer length
	nTemp = sizeof(int);
	memcpy(ptr, &pkg->DataBufLen, sizeof(int));
	ptr += nTemp;
	
	//Buffer
	nTemp = sizeof(char) * pkg->DataBufLen;
	memcpy(ptr, pkg->DataBuf, nTemp);
	ptr += nTemp;
	
	//Extension Length
	nTemp = sizeof(int);
	memcpy(ptr, &pkg->ExtensionLen, sizeof(int));
	ptr += nTemp;
	
	//Extension
	nTemp = sizeof(char) * pkg->ExtensionLen;
	memcpy(ptr, pkg->Extension, nTemp);
	ptr += nTemp;
	
	*ptr = '\0';
	
	*out_msg_len = ptr + 1 - buf;
	
	return buf;
}

void CmdPack_Unserialize(IN PCMD_PACKAGE pkg, IN char *in_msg)
{
	if(pkg == NULL)
		return;
	if(in_msg == NULL)
		return;
		
	if(CmdPack_IsMsgValid(in_msg) != 1)
		return;
	
	
	char *ptr = NULL;
	int nTemp;
	
	nTemp = 0;
	
	CmdPack_Init(pkg);
	
	ptr = in_msg;
	
	//RcgCode
	nTemp = strlen(ptr);
	strcpy(pkg->RcgCode, ptr);
	ptr += nTemp + 1;
	
	//Major Code
	nTemp = sizeof(unsigned short);
	memcpy(&pkg->CMD_MJ_CODE, ptr, nTemp);
	ptr += nTemp;
	
	//Minor Code
	nTemp = sizeof(unsigned short);
	memcpy(&pkg->CMD_MN_CODE, ptr, nTemp);
	ptr += nTemp;
	
	//DataBufLen
	nTemp = sizeof(int);
	memcpy(&pkg->DataBufLen, ptr, nTemp);
	ptr += nTemp;
	
	//DataBuf
	if(pkg->DataBufLen > 0)
	{
		nTemp = pkg->DataBufLen * sizeof(char);
		pkg->DataBuf = (char*)malloc(nTemp + 1);
		memset(pkg->DataBuf, 0, nTemp + 1);
		memcpy(pkg->DataBuf, ptr, nTemp);
		ptr += nTemp;
	}
	
	//Extension Len
	nTemp = sizeof(int);
	memcpy(&pkg->ExtensionLen, ptr, nTemp);
	ptr += nTemp;
	
	//Extension
	if(pkg->ExtensionLen > 0)
	{
		nTemp = pkg->ExtensionLen * sizeof(char);
		pkg->Extension = (char*)malloc(nTemp + 1);
		memset(pkg->Extension, 0, nTemp + 1);
		memcpy(pkg->Extension, ptr, nTemp);
		ptr += nTemp;
	}
	
	return;
}
