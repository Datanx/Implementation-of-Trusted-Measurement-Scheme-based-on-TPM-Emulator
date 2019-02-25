#include "HWInfo.h"
#include "cpuinfo.h"
#include "usbinfo.h"
#include "hdinfo.h"
#include "nicinfo.h"
#include "dmi_info.h"
#include <stdio.h>
#include <stdlib.h>

//////////////////////////Global Variables/////////////////////////
void *__HWLIST__[10] = {NULL};		//Local machine hardware list

void InitHwInfo()
{
    int i=0;
    
    FreeHwInfo();
    for(i=0; i<10; i++)
        __HWLIST__[i] = NULL;
}

HWINFO_STATUS GetHwInfo()
{
	//Get CPU Info
	PCPU_INFO pCPUInfo = NULL;
	pCPUInfo = (PCPU_INFO)malloc(sizeof(CPU_INFO));
	InitCPUStruct(pCPUInfo);
	FillCPUStruct(pCPUInfo);
	
	//Get DMI Info(BIOS, Memory, MotherBoard, PC info)
	PBIOS_INFO BiosInfo = NULL;
	PPC_INFO PCInfo = NULL;
	PMOTHER_BOARD_INFO MbInfo = NULL;
        if(___MMDEV_LIST_HEADER___ != NULL)
            DestroyMMDevList();
	PrepareMachineInfo(&BiosInfo, &PCInfo, &MbInfo);
	
	//Get HDD Info
	PHDD_INFO pHDDInfo = NULL;
	InitHDDStruct(&pHDDInfo);
	FillHDDStruct(pHDDInfo);
	
	//Get USB Info
	InitUDIList();
	CreateUDIList();
	
	//Get NIC Info
	InitNICList();
	CreateNICList();
	
	//Construct hardware list
	__HWLIST__[HWPRT_CPU] = pCPUInfo;
	__HWLIST__[HWPRT_MEM] = ___MMDEV_LIST_HEADER___;
	__HWLIST__[HWPRT_HDD] = pHDDInfo;
	__HWLIST__[HWPRT_USB] = get_udi_list_header();
	__HWLIST__[HWPRT_NIC] = get_nic_list_header();
	__HWLIST__[HWPRT_MTHRBRD] = MbInfo;
	__HWLIST__[HWPRT_BIOS] = BiosInfo;
	__HWLIST__[HWPRT_PC] = PCInfo;
	
	return HWINFO_SUCCESS;
}

HWINFO_STATUS FreeHwInfo()
{
	//Free CPU Struct
    if(__HWLIST__[HWPRT_CPU] != NULL)
    {
	FreeCPUStruct((PCPU_INFO)(__HWLIST__[HWPRT_CPU]));
        __HWLIST__[HWPRT_CPU] = NULL;
    }
	//Destroy USB device info list
    if(__HWLIST__[HWPRT_USB] != NULL)
    {
	DestroyUDIList();
        __HWLIST__[HWPRT_USB] = NULL;
    }
	//Destroy NIC device info list
    if(__HWLIST__[HWPRT_NIC] != NULL)
    {
	DestroyNICList();
        __HWLIST__[HWPRT_NIC] = NULL;
    }
	//Free HDD Struct
    if(__HWLIST__[HWPRT_HDD] != NULL)
    {
	FreeHDDStruct((PHDD_INFO)(__HWLIST__[HWPRT_HDD]));
        __HWLIST__[HWPRT_HDD] = NULL;
    }
	//Free BIOS Info
    if(__HWLIST__[HWPRT_BIOS] != NULL)
    {
	FreeBiosInfo((PBIOS_INFO)(__HWLIST__[HWPRT_BIOS]));
        __HWLIST__[HWPRT_BIOS] = NULL;
    }
	//Free Mother board Info
    if(__HWLIST__[HWPRT_MTHRBRD] != NULL)
    {
	FreeMotherBoardInfo((PMOTHER_BOARD_INFO)(__HWLIST__[HWPRT_MTHRBRD]));
        __HWLIST__[HWPRT_MTHRBRD] = NULL;
    }
	//Free PC Info
    if(__HWLIST__[HWPRT_PC] != NULL)
    {
	FreePCInfo((PPC_INFO)(__HWLIST__[HWPRT_PC]));
        __HWLIST__[HWPRT_PC] = NULL;
    }
	//Free Memory info list
    if(__HWLIST__[HWPRT_MEM] != NULL)
    {
	DestroyMMDevList();
        __HWLIST__[HWPRT_MEM] = NULL;
    }
	
	return HWINFO_SUCCESS;
}

void helpfunc_hwiprint(int print_type)
{
	PCPU_INFO cpuinfo = NULL;
	switch(print_type)
	{
		case HWPRT_CPU:
			cpuinfo = (PCPU_INFO)malloc(sizeof(CPU_INFO));
			InitCPUStruct(cpuinfo);
			FillCPUStruct(cpuinfo);
			
			print_cpu_info(cpuinfo);
			printf("\n");
			
			FreeCPUStruct(cpuinfo);
			break;
		case HWPRT_MEM:
			break;
		case HWPRT_HDD:
			break;
		case HWPRT_USB:
			break;
		case HWPRT_NIC:
			break;
		default:
			break;
	}
}
