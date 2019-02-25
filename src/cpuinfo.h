#ifndef CPUINFO_H
#define CPUINFO_H

#include "HWInfo.h"

/***************************************************************************
 *   CPU Macro & Constant Definitions   *
 *      *
 *                                                                         *
 ***************************************************************************/
//Obtain CPU info by cpuid instruction
#define cpuid(func,eax,ebx,ecx,edx)\
        __asm__ __volatile__ ("cpuid":\
        "=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) : "a" (func));


/*==========================================================================
 *   CPU info Structure Declarations   *
 *      *
 *                                                                         *
 ==========================================================================*/
 //CPU info structure
 typedef struct _CPU_INFO
{
	char Company[256];
	char BaseParam[256];
	char SerialNumber[256];
	char md5_hash[40];
} CPU_INFO, *PCPU_INFO;

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

/*==========================================================================
 *   CPU info Functions Declarations   *
 *      *
 *                                                                         *
 ==========================================================================*/
////[Help Funcion]////
//Function: Get CPU Company
//Parameters:
//1.Stores returned company info
//2.cpuid ebx
//3.cpuid ecx
//4.cpuid edx
//Retruns: void
//Commet: This function can only be called by other functions
void helpfunc_GetCPUCompany(OUT char* cCom, IN int bx,IN int cx,IN int dx);

////[Normal Function]////
//Function: Get local CPU vendor
//Parameters:
//1.Stores returned company info
//Returns: void
void tpm_hwinfo_GetCPUCompany(OUT char *cCom);

////[Normal Function]////
//Function: Get local CPU base parameters
//Parameters:
//1.Stores returned base parameters
//Returns: void
void tpm_hwinfo_GetCPUBaseParam(OUT char *baseParam);

////[Normal Function]////
//Function: Get local CPU ID
//Parameters:
//1.Stores returned CPU id
//Returns: void
void tpm_hwinfo_GetCPUID(OUT char *cpuId);

////[Normal Function]////
//Function: Initialize CPU info structure
//Parameters:
//Param 1: PCPU_INFO pointer
//Returns: void
void InitCPUStruct(OUT PCPU_INFO cpustruct);

////[Normal Function]////
//Function: Filling the CPU info structure
//Parameters:
//Param 1: PCPU_INFO variable
//Returns: void
void FillCPUStruct(IN PCPU_INFO cpustruct);

////[Normal Function]////
//Function: Free CPU info structure
//Parameters:
//Param 1: PCPU_INFO variable
//Returns: void
void FreeCPUStruct(IN PCPU_INFO cpustruct);

void SerializeCPUStruct(IN PCPU_INFO cpustruct, OUT char *out_str, OUT int *out_str_len);

void UnserializeCPUStruct(OUT PCPU_INFO *cpustruct, IN char *in_str);

////[help Function]////
//Function: Print CPU info structure
//Parameters:
//Param 1: PCPU_INFO variable
//Returns: void
//Comment: this function only for test
void print_cpu_info(IN PCPU_INFO cpustruct);


/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#endif
