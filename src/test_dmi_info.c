#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dmi_info.h"

int main()
{
	PBIOS_INFO BiosInfo = NULL;
	PBIOS_INFO BiosInfo2 = NULL;
	char *serializedBIOSInfo = NULL;
	int serializedBIOSInfoLen = 0;
	
	PPC_INFO PCInfo = NULL;
	PPC_INFO PCInfo2 = NULL;
	char *serializedPCInfo = NULL;
	int serializedPCInfoLen = 0;
	
	PMOTHER_BOARD_INFO MbInfo = NULL;
	PMOTHER_BOARD_INFO MbInfo2 = NULL;
	char *serializedMBInfo = NULL;
	int serializedMBInfoLen = 0;
	
	PMEM_DEVICE_INFO MMInfo = NULL;
	PMEM_DEVICE_INFO MMInfo2 = NULL;
	char *serializedMMInfo = NULL;
	int serializedMMInfoLen = 0;
	
	PrepareMachineInfo(&BiosInfo, &PCInfo, &MbInfo);
	
	//Serialize & Unserialize BIOS info
	serializedBIOSInfo = (char*)malloc(sizeof(char) * HW_STRUCT_SERIALIZE_SIZE);
	BiosInfo2 = (PBIOS_INFO)malloc(sizeof(BIOS_INFO));
	SerializeBiosInfo(BiosInfo, serializedBIOSInfo, &serializedBIOSInfoLen);
	UnserializeBiosInfo(&BiosInfo2, serializedBIOSInfo);
	print_bios_info(BiosInfo2);
	free(serializedBIOSInfo);
	free(BiosInfo2);
	
	//Serialize & Unserialize PC info
	serializedPCInfo = (char*)malloc(sizeof(char) * HW_STRUCT_SERIALIZE_SIZE);
	PCInfo2 = (PPC_INFO)malloc(sizeof(PC_INFO));
	SerializePCInfo(PCInfo, serializedPCInfo, &serializedPCInfoLen);
	UnserializePCInfo(&PCInfo2, serializedPCInfo);
	print_pc_info(PCInfo2);
	free(serializedPCInfo);
	free(PCInfo2);
	
	//Serialize & Unserialize MB info
	serializedMBInfo = (char*)malloc(sizeof(char) * HW_STRUCT_SERIALIZE_SIZE);
	MbInfo2 = (PMOTHER_BOARD_INFO)malloc(sizeof(MOTHER_BOARD_INFO));
	SerializeMBInfo(MbInfo, serializedMBInfo, &serializedMBInfoLen);
	UnserializeMBInfo(&MbInfo2, serializedMBInfo);
	print_mb_info(MbInfo2);
	free(serializedMBInfo);
	free(MbInfo2);
	
	//Serialize & Unserialize Memory info
	MMInfo = (PMEM_DEVICE_INFO)(___MMDEV_LIST_HEADER___->next->DATA_AREA.data);
	serializedMMInfo = (char*)malloc(sizeof(char) * HW_STRUCT_SERIALIZE_SIZE);
	MMInfo2 = (PMEM_DEVICE_INFO)malloc(sizeof(MEM_DEVICE_INFO));
	SerializeMMInfo(MMInfo, serializedMMInfo, &serializedMMInfoLen);
	UnserializeMMInfo(&MMInfo2, serializedMMInfo);
	print_mm_info(MMInfo2);
	free(serializedMMInfo);
	free(MMInfo2);

	PDBLLIST_NODETYPE mmListHeader = ___MMDEV_LIST_HEADER___;
	while(mmListHeader->nodetype != NTYPE_TAIL)
	{
		if(mmListHeader->nodetype == NTYPE_DATA)
		{
			print_mm_info((PMEM_DEVICE_INFO)mmListHeader->DATA_AREA.data);
		}
		mmListHeader=mmListHeader->next;
	}
	
	DestroyDMIInfo(BiosInfo, PCInfo, MbInfo);
	return 0;
}
