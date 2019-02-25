#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CmdPackage.h"

int main()
{
	int i = 0;
	PCMD_PACKAGE pkg = NULL;
	PCMD_PACKAGE oPkg = NULL;
	char *serializedPkg = NULL;
	int nVal = 25;
	
	pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
	CmdPack_Init(pkg);
	
	strcpy(pkg->RcgCode, "PAPILIO");
	pkg->CMD_MJ_CODE = CMD_MJ_REQ_HWINFO;
	pkg->CMD_MN_CODE = CMD_MN_REQ_CPUSHA1HASH;
	pkg->DataBuf = (int*)malloc(sizeof(int));
	memcpy(pkg->DataBuf, &nVal, sizeof(int));
	pkg->DataBufLen = sizeof(int);
	/*
	pkg->DataBuf = (char*)malloc(32 * sizeof(char));
	memset(pkg->DataBuf, 0, 32);
	i = strlen("Intel Centrino 2");
	memcpy(pkg->DataBuf, "Intel Centrino 2", i);
	pkg->DataBufLen = i;
	
	pkg->Extension = (char*)malloc(48 * sizeof(char));
	memset(pkg->Extension, 0, 48);
	i = strlen("Extra information");
	memcpy(pkg->Extension, "Extra information", i);
	pkg->ExtensionLen = i;
	*/
	
	int len = 0;
	serializedPkg = CmdPack_Serialize(&len,  pkg);
	
	oPkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
	CmdPack_Unserialize(oPkg, serializedPkg);
	
	free(serializedPkg);
	CmdPack_Free(pkg);
	CmdPack_Free(oPkg);
	return 0;
}
