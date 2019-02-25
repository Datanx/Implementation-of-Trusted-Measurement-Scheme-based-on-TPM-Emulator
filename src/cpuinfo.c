#include "cpuinfo.h"
#include "ConsoleColor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void helpfunc_GetCPUCompany(OUT char* cCom, IN int bx, IN int cx, IN int dx)
{
	int b,i;
	for(i=0;bx>0;i++)
	{
		b=bx;
		b=(b>>8);
		b=(b<<8);
		cCom[i]=bx-b;
		bx=(bx>>8);
	}
	for(;dx>0;i++)
	{
		b=dx;
		b=(b>>8);
		b=(b<<8);
		cCom[i]=dx-b;
		dx=(dx>>8);
	}
	for(;cx>0;i++)
	{
		b=cx;
		b=(b>>8);
		b=(b<<8);
		cCom[i]=cx-b;
		cx=(cx>>8);
	}
	cCom[12]='\0';
}

void tpm_hwinfo_GetCPUCompany(OUT char *cCom)
{
	//name buffer
	char ComName[13];
    //some int variables
	int a, b, c, d;

	memset((void*)ComName, 0, 13);
	cpuid(0, a, b, c, d);
	helpfunc_GetCPUCompany(ComName, b, c, d);
	strcpy(cCom, ComName);
}

void tpm_hwinfo_GetCPUBaseParam(OUT char *baseParam)
{
	char bp_buf[HWI_BUFFER_SIZE];
	unsigned long DCPUBaseInfo;
	int a, b, c, d;

	memset((void*)bp_buf, 0, HWI_BUFFER_SIZE);
	cpuid(1, a, b, c, d);
	DCPUBaseInfo = a;
	sprintf(bp_buf, "Family:%X Model:%X Stepping ID:%X",(DCPUBaseInfo & 0x0F00) >> 8,(DCPUBaseInfo & 0xF0) >> 4, DCPUBaseInfo & 0xF);
	strcpy(baseParam, bp_buf);
}

void tpm_hwinfo_GetCPUID(OUT char *cpuId)
{
	int a, b, c, d;
	char cpuid_buf[HWI_BUFFER_SIZE];

	memset((void *)cpuid_buf, 0, HWI_BUFFER_SIZE);
	cpuid(0x80000004,a,b,c,d);
	sprintf(cpuid_buf, "%#010x %#010x %#010x %#010x", a, b, c, d);
	strcpy(cpuId, cpuid_buf);
}

void InitCPUStruct(IN PCPU_INFO cpustruct)
{
	if(cpustruct == NULL)
		return;
	
	//Initialize cpustruct
	memset(cpustruct->Company, 0, 256);
	memset(cpustruct->BaseParam, 0, 256);
	memset(cpustruct->SerialNumber, 0, 256);
	memset(cpustruct->md5_hash, 0, 40);
}

void FillCPUStruct(IN PCPU_INFO cpustruct)
{
	char tmpStr[256];
	char infobuffer[1024];
	char *bufptr = NULL;
	char *md5str = NULL;
	int n = 0;
	
	if(cpustruct == NULL)
		return;
	
	//Initialize infobuffer
	memset(infobuffer, 0, 1024);
	
	//Get CPU Company
	memset(tmpStr, 0, 256);
	tpm_hwinfo_GetCPUCompany(tmpStr);
	n = strlen(tmpStr);
	strcpy(cpustruct->Company, tmpStr);
	strcpy(infobuffer, tmpStr);
	bufptr = infobuffer + n;
	
	memset(tmpStr, 0, 256);
	tpm_hwinfo_GetCPUBaseParam(tmpStr);
	n = strlen(tmpStr);
	strcpy(cpustruct->BaseParam, tmpStr);
	strcpy(bufptr, tmpStr);
	bufptr += n;
	
	memset(tmpStr, 0, 256);
	tpm_hwinfo_GetCPUID(tmpStr);
	n = strlen(tmpStr);
	strcpy(cpustruct->SerialNumber, tmpStr);
	strcpy(bufptr, tmpStr);
	bufptr += n;
	
	//Calculate md5 hash
	md5str = (char *)MDString(infobuffer);
	strcpy(cpustruct->md5_hash, md5str);
}

void FreeCPUStruct(IN PCPU_INFO cpustruct)
{
	if(cpustruct != NULL)
		free(cpustruct);
}

void SerializeCPUStruct(IN PCPU_INFO cpustruct, OUT char *out_str, OUT int *out_str_len)
{
    if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    strcpy(ptr, cpustruct->BaseParam);
    n = strlen(cpustruct->BaseParam);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    strcpy(ptr, cpustruct->Company);
    n = strlen(cpustruct->Company);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    strcpy(ptr, cpustruct->SerialNumber);
    n = strlen(cpustruct->SerialNumber);
    ptr += 1 + n;
    *out_str_len += (1 + n);

    strcpy(ptr, cpustruct->md5_hash);
    n = strlen(cpustruct->md5_hash);
    ptr += 1+n;
    *out_str_len += (1+n);

    return;
}

void UnserializeCPUStruct(OUT PCPU_INFO *cpustruct, IN char *in_str)
{
    if(*cpustruct == NULL || in_str == NULL)
        return;

    char *ptr = in_str;

    strcpy((*cpustruct)->BaseParam, ptr);
    ptr += 1 + strlen((*cpustruct)->BaseParam);
    strcpy((*cpustruct)->Company, ptr);
    ptr += 1 + strlen((*cpustruct)->Company);
    strcpy((*cpustruct)->SerialNumber, ptr);
    ptr += 1 + strlen((*cpustruct)->SerialNumber);
    strcpy((*cpustruct)->md5_hash, ptr);

    return;
}

void print_cpu_info(IN PCPU_INFO cpustruct)
{
	if(cpustruct == NULL)
		return;
	
	printf("CPU Information:\n");
	printf("\tCPU Manufacturer: %s\n", cpustruct->Company);
	printf("\tCPU Base Parameters: %s\n", cpustruct->BaseParam);
	printf("\tCPU Serial Number: %s\n", cpustruct->SerialNumber);
	printf("\tCPU Info hash value: %s\n", cpustruct->md5_hash);
}
