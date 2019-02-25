#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "ds_DBLList.h"
#include "dmi_info.h"
#include "ConsoleColor.h"
#include "Animation.h"

//Memory device info list header & tail
PDBLLIST_NODETYPE ___MMDEV_LIST_HEADER___ 	= NULL;
PDBLLIST_NODETYPE ___MMDEV_LIST_TAIL___ 	= NULL;
PDBLLIST_NODETYPE ___MMDEV_LIST_PTR___ 	= NULL;
PBIOS_INFO 			___BIOS_PTR___ = NULL;
PPC_INFO 			___PC_PTR___ = NULL;	
PMOTHER_BOARD_INFO 	___MB_PTR___ = NULL;

void InitBiosInfo(PBIOS_INFO *bios_info)
{
	if(*bios_info != NULL)
		free(*bios_info);
	
	*bios_info = NULL;
	
	*bios_info = (PBIOS_INFO)malloc(sizeof(BIOS_INFO));
	if(*bios_info == NULL)
		return;
		
	memset((*bios_info)->Vendor, 0, 256);
	memset((*bios_info)->Version, 0, 64);
	memset((*bios_info)->Date, 0, 16);
	memset((*bios_info)->md5_hash, 0, 40);
}

void FreeBiosInfo(PBIOS_INFO bi)
{
	if(bi != NULL)
		free(bi);
}

void SerializeBiosInfo(IN PBIOS_INFO biosinfo, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    n = strlen(biosinfo->Vendor);
    memcpy(ptr, biosinfo->Vendor, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(biosinfo->Version);
    memcpy(ptr, biosinfo->Version, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(biosinfo->Date);
    memcpy(ptr, biosinfo->Date, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);

	n = strlen(biosinfo->md5_hash);
    memcpy(ptr, biosinfo->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializeBiosInfo(OUT PBIOS_INFO *biosinfo, IN char *in_str)
{
	if(*biosinfo == NULL || in_str == NULL)
        return;

    char *ptr = in_str;
	
    strcpy((*biosinfo)->Vendor, ptr);
    ptr += 1 + strlen((*biosinfo)->Vendor);
    
    strcpy((*biosinfo)->Version, ptr);
    ptr += 1 + strlen((*biosinfo)->Version);
    
    strcpy((*biosinfo)->Date, ptr);
    ptr += 1 + strlen((*biosinfo)->Date);
    
    strcpy((*biosinfo)->md5_hash, ptr);

    return;
}

void InitPCInfo(PPC_INFO *pc_info)
{
	if(*pc_info != NULL)
		free(*pc_info);
	
	*pc_info = NULL;
	
	
	*pc_info = (PPC_INFO)malloc(sizeof(PC_INFO));
	
	if(*pc_info == NULL)
		return;
		
	memset((*pc_info)->Vendor, 0, 256);
	memset((*pc_info)->Product, 0, 256);
	memset((*pc_info)->Version, 0, 64);
	memset((*pc_info)->SN, 0, 64);
	memset((*pc_info)->Model, 0, 64);
	memset((*pc_info)->Extra, 0, 64);
	memset((*pc_info)->md5_hash, 0, 40);
}

void FreePCInfo(IN PPC_INFO pi)
{
	if(pi != NULL)
		free(pi);
}

void SerializePCInfo(IN PPC_INFO pcinfo, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    n = strlen(pcinfo->Vendor);
    memcpy(ptr, pcinfo->Vendor, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(pcinfo->Product);
    memcpy(ptr, pcinfo->Product, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(pcinfo->Version);
    memcpy(ptr, pcinfo->Version, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(pcinfo->SN);
    memcpy(ptr, pcinfo->SN, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(pcinfo->Model);
    memcpy(ptr, pcinfo->Model, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(pcinfo->Extra);
    memcpy(ptr, pcinfo->Extra, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);

	n = strlen(pcinfo->md5_hash);
    memcpy(ptr, pcinfo->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializePCInfo(OUT PPC_INFO *pcinfo, IN char *in_str)
{
	if(*pcinfo == NULL || in_str == NULL)
        return;

    char *ptr = in_str;
	
    strcpy((*pcinfo)->Vendor, ptr);
    ptr += 1 + strlen((*pcinfo)->Vendor);
    
    strcpy((*pcinfo)->Product, ptr);
    ptr += 1 + strlen((*pcinfo)->Product);
    
    strcpy((*pcinfo)->Version, ptr);
    ptr += 1 + strlen((*pcinfo)->Version);
    
    strcpy((*pcinfo)->SN, ptr);
    ptr += 1 + strlen((*pcinfo)->SN);
    
    strcpy((*pcinfo)->Model, ptr);
    ptr += 1 + strlen((*pcinfo)->Model);
    
    strcpy((*pcinfo)->Extra, ptr);
    ptr += 1 + strlen((*pcinfo)->Extra);
    
    strcpy((*pcinfo)->md5_hash, ptr);

    return;
}

void InitMotherBoardInfo(PMOTHER_BOARD_INFO *mb_info)
{
	if(*mb_info != NULL)
		free(*mb_info);
	
	*mb_info = NULL;
	
	
	*mb_info = (PMOTHER_BOARD_INFO)malloc(sizeof(MOTHER_BOARD_INFO));
	
	if(*mb_info == NULL)
		return;
		
	memset((*mb_info)->Vendor, 0, 256);
	memset((*mb_info)->Product, 0, 256);
	memset((*mb_info)->Version, 0, 64);
	memset((*mb_info)->md5_hash, 0, 40);
}

void FreeMotherBoardInfo(IN PMOTHER_BOARD_INFO mbi)
{
	if(mbi != NULL)
		free(mbi);
}

void SerializeMBInfo(IN PMOTHER_BOARD_INFO mbinfo, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    n = strlen(mbinfo->Vendor);
    memcpy(ptr, mbinfo->Vendor, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(mbinfo->Product);
    memcpy(ptr, mbinfo->Product, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(mbinfo->Version);
    memcpy(ptr, mbinfo->Version, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);

	n = strlen(mbinfo->md5_hash);
    memcpy(ptr, mbinfo->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializeMBInfo(OUT PMOTHER_BOARD_INFO *mbinfo, IN char *in_str)
{
	if(*mbinfo == NULL || in_str == NULL)
        return;

    char *ptr = in_str;
	
    strcpy((*mbinfo)->Vendor, ptr);
    ptr += 1 + strlen((*mbinfo)->Vendor);
    
    strcpy((*mbinfo)->Product, ptr);
    ptr += 1 + strlen((*mbinfo)->Product);
    
    strcpy((*mbinfo)->Version, ptr);
    ptr += 1 + strlen((*mbinfo)->Version);
    
    strcpy((*mbinfo)->md5_hash, ptr);

    return;
}

void InitMemDevInfo(PMEM_DEVICE_INFO *mmdev_info)
{
	if(*mmdev_info != NULL)
		free(*mmdev_info);
	
	*mmdev_info = NULL;
	
	
	*mmdev_info = (PMEM_DEVICE_INFO)malloc(sizeof(MEM_DEVICE_INFO));
	
	if(*mmdev_info == NULL)
		return;
		
	memset((*mmdev_info)->Vendor, 0, 256);
	memset((*mmdev_info)->Product, 0, 256);
	memset((*mmdev_info)->SN, 0, 64);
	memset((*mmdev_info)->Slot, 0, 32);
	memset((*mmdev_info)->Bank, 0, 32);
	memset((*mmdev_info)->Extra, 0, 32);
	memset((*mmdev_info)->md5_hash, 0, 40);
}

void InitMMDevList()
{
	dbll_InitList(&___MMDEV_LIST_HEADER___, &___MMDEV_LIST_TAIL___);
	
	return;
}

void DestroyMMDevList()
{
	dbll_DestoryList(&___MMDEV_LIST_HEADER___);
        ___MMDEV_LIST_HEADER___ = NULL;
        ___MMDEV_LIST_TAIL___ = NULL;
        ___MMDEV_LIST_PTR___ = NULL;
}

void SerializeMMInfo(IN PMEM_DEVICE_INFO mminfo, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    n = strlen(mminfo->Vendor);
    memcpy(ptr, mminfo->Vendor, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(mminfo->Product);
    memcpy(ptr, mminfo->Product, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(mminfo->SN);
    memcpy(ptr, mminfo->SN, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(mminfo->Slot);
    memcpy(ptr, mminfo->Slot, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(mminfo->Bank);
    memcpy(ptr, mminfo->Bank, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(mminfo->Extra);
    memcpy(ptr, mminfo->Extra, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);

	n = strlen(mminfo->md5_hash);
    memcpy(ptr, mminfo->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializeMMInfo(OUT PMEM_DEVICE_INFO *mminfo, IN char *in_str)
{
	if(*mminfo == NULL || in_str == NULL)
        return;

    char *ptr = in_str;
	
    strcpy((*mminfo)->Vendor, ptr);
    ptr += 1 + strlen((*mminfo)->Vendor);
    
    strcpy((*mminfo)->Product, ptr);
    ptr += 1 + strlen((*mminfo)->Product);
    
    strcpy((*mminfo)->SN, ptr);
    ptr += 1 + strlen((*mminfo)->SN);
    
    strcpy((*mminfo)->Slot, ptr);
    ptr += 1 + strlen((*mminfo)->Slot);
    
    strcpy((*mminfo)->Bank, ptr);
    ptr += 1 + strlen((*mminfo)->Bank);
    
    strcpy((*mminfo)->Extra, ptr);
    ptr += 1 + strlen((*mminfo)->Extra);
    
    strcpy((*mminfo)->md5_hash, ptr);

    return;
}

void ReadMachineInfo(int fd, u32 base, int len, int num, int dmiversionmaj, int dmiversionmin, PBIOS_INFO *bios_info, PPC_INFO *pc_info, PMOTHER_BOARD_INFO *mb_info)
{
	unsigned char *buf = (unsigned char*)malloc(len);
	u8 *data = NULL;
	u32 mmoffset = 0;
	void *mmp = NULL;
	u8 *data_ptr = NULL;
	u8 *dptr = NULL;
	char *md5_str = NULL;
	char infobuffer[1024]={0};
	char *info_ptr = NULL;
	int n = 0;
	
	if(len == 0)
		return;
	if(buf == NULL)
		return;
		
	mmoffset = base % getpagesize();
	
	mmp = mmap(0, mmoffset + len, PROT_READ, MAP_SHARED, fd, base - mmoffset);
	if(mmp == MAP_FAILED)
	{
		free(buf);
		return;
	}
	memcpy(buf, (u8*) mmp + mmoffset, len);
	
	munmap(mmp, mmoffset + len);
	
	data = buf;
	
	//Initializing dmi info
	InitBiosInfo(bios_info);
	InitPCInfo(pc_info);
	InitMotherBoardInfo(mb_info);
	
	if(*bios_info == NULL || *pc_info == NULL || *mb_info == NULL)
		return;
	
	//Parsing BIOS, PC, MotherBoard information from "data"
	PBIOS_INFO bi = *bios_info;
	PPC_INFO pi = *pc_info;
	PMOTHER_BOARD_INFO mbi = *mb_info;
	
	PDMI_HEADER dm = (PDMI_HEADER) data;
	
	//Fill BIOS information
	data_ptr = data + dm->length;
	strcpy(bi->Vendor, data_ptr);
	data_ptr += strlen(bi->Vendor) + 1;
	strcpy(bi->Version, data_ptr);
	data_ptr += strlen(bi->Version) + 1;
	strncpy(bi->Date, data_ptr, 10);
	data_ptr += strlen(bi->Date) + 2;
	//Calculate MD5 hash for Bios information
	memset(infobuffer, 0, 1024);
	info_ptr = infobuffer;
	n = strlen(bi->Vendor);
	strcpy(info_ptr, bi->Vendor);
	info_ptr += n;
	n = strlen(bi->Version);
	strcpy(info_ptr, bi->Version);
	info_ptr += n;
	n = strlen(bi->Date);
	strcpy(info_ptr, bi->Date);
	md5_str = (char*)MDString(infobuffer);
	strcpy(bi->md5_hash, md5_str);
	
	//Fill PC information
	dm = (PDMI_HEADER)data_ptr;
	data_ptr += dm->length;
	strcpy(pi->Vendor, data_ptr);
	data_ptr += strlen(pi->Vendor) + 1;
	strcpy(pi->Product, data_ptr);
	data_ptr += strlen(pi->Product) + 1;
	strcpy(pi->Version, data_ptr);
	data_ptr += strlen(pi->Version) + 1;
	strcpy(pi->SN, data_ptr);
	data_ptr += strlen(pi->SN) + 1;
	strcpy(pi->Model, data_ptr);
	data_ptr += strlen(pi->Model) + 1;
	strcpy(pi->Extra, data_ptr);
	data_ptr += strlen(pi->Extra) + 2;
	//Calculate MD5 hash for PC information
	memset(infobuffer, 0, 1024);
	info_ptr = infobuffer;
	n = strlen(pi->Vendor);
	strcpy(info_ptr, pi->Vendor);
	info_ptr += n;
	n = strlen(pi->Product);
	strcpy(info_ptr, pi->Product);
	info_ptr += n;
	n = strlen(pi->Version);
	strcpy(info_ptr, pi->Version);
	info_ptr += n;
	n = strlen(pi->SN);
	strcpy(info_ptr, pi->SN);
	info_ptr += n;
	n = strlen(pi->Model);
	strcpy(info_ptr, pi->Model);
	info_ptr += n;
	n = strlen(pi->Extra);
	strcpy(info_ptr, pi->Extra);
	md5_str = (char*)MDString(infobuffer);
	strcpy(pi->md5_hash, md5_str);
	
	//Fill Mother Board Information
	dm = (PDMI_HEADER)data_ptr;
	data_ptr += dm->length;
	strcpy(mbi->Vendor, data_ptr);
	data_ptr += strlen(mbi->Vendor) + 1;
	strcpy(mbi->Product, data_ptr);
	data_ptr += strlen(mbi->Product) + 1;
	strcpy(mbi->Version, data_ptr);
	//Calculate MD5 hash for Mother Board information
	memset(infobuffer, 0, 1024);
	info_ptr = infobuffer;
	n = strlen(mbi->Vendor);
	strcpy(info_ptr, mbi->Vendor);
	info_ptr += n;
	n = strlen(mbi->Product);
	strcpy(info_ptr, mbi->Product);
	md5_str = (char*)MDString(infobuffer);
	strcpy(mbi->md5_hash, md5_str);
	
	////////Find address for memory bank //////////
	//Preparing for memory device list
	PDBLLIST_NODETYPE newMemDevListNode = NULL;
	PMEM_DEVICE_INFO newMemDevInfo = NULL;
	
	InitMMDevList();	//Initializing Memory Device List
	
	data_ptr = data;
	while (data_ptr + sizeof(DMI_HEADER) <= (u8 *) buf + len)
	{
		dm = (PDMI_HEADER)data_ptr;
		newMemDevListNode = NULL;
		newMemDevInfo = NULL;
		
		if (data_ptr + dm->length > (u8 *) buf + len)
			break;
		
		if(dm->type == 17)		//this type represents memory device
		{
			//Memory device found
			InitMemDevInfo(&newMemDevInfo);	//initialize memory device info
			
			dptr = data_ptr;
			dptr += dm->length;
			//Memory Slot
			strcpy(newMemDevInfo->Slot, dptr);
			dptr += strlen(newMemDevInfo->Slot) + 1;
			//Memory Bank No
			strcpy(newMemDevInfo->Bank, dptr);
			dptr += strlen(newMemDevInfo->Bank) + 1;
			if(strstr(dptr, "Unknown") == NULL)	//if vendor info exists, fill vendor and other info
			{
				//Memory Vendor
				strcpy(newMemDevInfo->Vendor, dptr);
				dptr += strlen(newMemDevInfo->Vendor) + 1;
				//Serial Number
				strcpy(newMemDevInfo->SN, dptr);
				dptr += strlen(newMemDevInfo->SN) + 1;
				//Extra
				strcpy(newMemDevInfo->Extra, dptr);
				dptr += strlen(newMemDevInfo->Extra) + 1;
				//Product
				strcpy(newMemDevInfo->Product, dptr);
			}
			//Calculate MD5 hash for Mother Board information
			memset(infobuffer, 0, 1024);
			info_ptr = infobuffer;
			n = strlen(newMemDevInfo->Slot);
			strcpy(info_ptr, newMemDevInfo->Slot);
			info_ptr += n;
			n = strlen(newMemDevInfo->Bank);
			strcpy(info_ptr, newMemDevInfo->Bank);
			info_ptr += n;
			n = strlen(newMemDevInfo->Vendor);
			strcpy(info_ptr, newMemDevInfo->Vendor);
			info_ptr += n;
			n = strlen(newMemDevInfo->SN);
			strcpy(info_ptr, newMemDevInfo->SN);
			info_ptr += n;
			n = strlen(newMemDevInfo->Extra);
			strcpy(info_ptr, newMemDevInfo->Extra);
			info_ptr += n;
			n = strlen(newMemDevInfo->Product);
			strcpy(info_ptr, newMemDevInfo->Product);
			md5_str = (char*)MDString(infobuffer);
			strcpy(newMemDevInfo->md5_hash, md5_str);
			
			//Create new memory device list node
			dbll_CreateNode(&newMemDevListNode, (void*)newMemDevInfo, sizeof(MEM_DEVICE_INFO));
			//Append new memory device list node to memory device list
			dbll_AppendNode(___MMDEV_LIST_HEADER___, newMemDevListNode);
			
			//Free newMemDevInfo
			free(newMemDevInfo);
		}//if
		
		data_ptr += dm->length;
		while (*data_ptr || data_ptr[1])
			data_ptr++;
		data_ptr += 2;
	}

	//free buf
	free(buf);
	return;
}

void PrepareMachineInfo(OUT PBIOS_INFO *bios_info, OUT PPC_INFO *pc_info, OUT PMOTHER_BOARD_INFO *mb_info)
{
	unsigned char buf[20];
	int fd = open("/dev/mem", O_RDONLY);
	long fp = 0xE0000L;
	u32 mmoffset = 0;
	void *mmp = NULL;
	u16 dmimaj = 0, dmimin = 0;
	u8 smmajver = 0, smminver = 0;
	
	fp -= 0x10;
	while(fp < 0xFFFFF)
	{
		fp += 0x10;
		mmoffset = fp % getpagesize();
		mmp = mmap(0, mmoffset + 0x20, PROT_READ, MAP_SHARED, fd, fp-mmoffset);
		memset(buf, 0, sizeof(buf));
		if(mmp != MAP_FAILED)
		{
			memcpy(buf, (u8*)mmp + mmoffset, sizeof(buf));
			munmap(mmp, mmoffset + 0x20);
		}
		if(mmp == MAP_FAILED)
		{
			close(fd);
			break;
		}
		else if(memcmp(buf, "_SM_", 4) == 0)
		{
			smmajver = buf[6];
			smminver = buf[7];
		}
		else if(memcmp(buf, "_DMI_", 5) == 0)
		{
			u16 num = buf[13] << 8 | buf[12];
			u16 len = buf[7] << 8 | buf[6];
			u32 base = buf[11] << 24 | buf[10] << 16 | buf[9] << 8 | buf[8];
			dmimaj = buf[14] ? buf[14] >> 4 : smmajver;
			dmimin = buf[14] ? buf[14] & 0x0F : smminver;
			ReadMachineInfo(fd, base, len, num, dmimaj, dmimin, bios_info, pc_info, mb_info);
			
			break;
		}
	}
}

void DestroyDMIInfo(IN PBIOS_INFO bios_info, IN PPC_INFO pc_info, IN PMOTHER_BOARD_INFO mb_info)
{
	FreeBiosInfo(bios_info);
	FreePCInfo(pc_info);
	FreeMotherBoardInfo(mb_info);
	DestroyMMDevList();
}

void print_bios_info(PBIOS_INFO bios_info)
{
	if(bios_info == NULL)
		return;
	
	printf("BIOS Information:\n");
	printf("\tBIOS Vendor: %s\n", bios_info->Vendor);
	printf("\tBIOS Version: %s\n", bios_info->Version);
	printf("\tBIOS Date: %s\n", bios_info->Date);
	printf("\tBIOS Info hash value: %s\n", bios_info->md5_hash);
}

void print_mb_info(PMOTHER_BOARD_INFO mb_info)
{
	if(mb_info == NULL)
		return;
	
	printf("Mother Board Information:\n");
	printf("\tMother Board Vendor: %s\n", mb_info->Vendor);
	printf("\tMother Board Product: %s\n", mb_info->Product);
	printf("\tMother Board Version: %s\n", mb_info->Version);
	printf("\tMother Board Info hash value: %s\n", mb_info->md5_hash);
}

void print_mm_info(PMEM_DEVICE_INFO mm)
{
	printf("\n\tVendor: %s\n", mm->Vendor);
	printf("\tProduct: %s\n", mm->Product);
	printf("\tSN: %s\n", mm->SN);
	printf("\tSlot: %s\n", mm->Slot);
	printf("\tBank: %s\n", mm->Bank);
	printf("\tExtra: %s\n", mm->Extra);
	printf("\tmd5 hash: %s\n", mm->md5_hash);
		
	printf("\n");
}

void mem_info_list_walk()
{
	PDBLLIST_NODETYPE tmpNode = NULL;
	PMEM_DEVICE_INFO tmpMemNode = NULL;
	int i = 0;
	
	if(___MMDEV_LIST_HEADER___ == NULL)
	{
		return;
	}
	
	if(___MMDEV_LIST_HEADER___->DATA_AREA.length <= 0)
	{
		return;
	}
	
	i = 1;
	tmpNode = ___MMDEV_LIST_HEADER___->next;
	Console_SetTextColor(CONSOLECOLOR_CYAN, CONSOLECOLOR_BG_BLACK);
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		printf("\nMem Dev %d Info:", i);
		RotatePrint(1, ".", SOW_OFF);

		tmpMemNode = (PMEM_DEVICE_INFO)(tmpNode->DATA_AREA.data);
		print_mm_info(tmpMemNode);
		
		tmpNode = tmpNode->next;
		i++;
	}
}

void print_pc_info(PPC_INFO pc_info)
{
	if(pc_info == NULL)
		return;
	
	printf("PC Information:\n");
	printf("\tPC Vendor: %s\n", pc_info->Vendor);
	printf("\tPC Product: %s\n", pc_info->Product);
	printf("\tPC Version: %s\n", pc_info->Version);
	printf("\tPC Serial Number: %s\n", pc_info->SN);
	printf("\tPC Model: %s\n", pc_info->Model);
	printf("\tPC Extra: %s\n", pc_info->Extra);
	printf("\tPC Info hash value: %s\n", pc_info->md5_hash);
}
