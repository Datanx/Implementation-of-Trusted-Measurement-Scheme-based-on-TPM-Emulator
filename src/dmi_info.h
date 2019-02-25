#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef DMI_INFO_H
#define DMI_INFO_H

#include "Defs.h"
#include "HWInfo.h"
#include "ds_DBLList.h"

#define MEM_FILE	"/dev/mem"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct _DMI_HEADER
{
	u8 type;
	u8 length;
	u16 handle;
} DMI_HEADER, *PDMI_HEADER;

typedef struct _BIOS_INFO
{
	char Vendor[256];
	char Version[64];
	char Date[16];
	char md5_hash[40];
} BIOS_INFO, *PBIOS_INFO;

typedef struct _PC_INFO
{
	char Vendor[256];
	char Product[256];
	char Version[64];
	char SN[64];
	char Model[64];
	char Extra[64];
	char md5_hash[40];
} PC_INFO, *PPC_INFO;

typedef struct _MOTHER_BOARD_INFO
{
	char Vendor[256];
	char Product[256];
	char Version[64];
	char md5_hash[40];
} MOTHER_BOARD_INFO, *PMOTHER_BOARD_INFO;

typedef struct _MEM_DEVICE_INFO
{
	char Vendor[256];
	char Product[256];
	char SN[64];
	char Slot[32];
	char Bank[32];
	char Extra[32];
	char md5_hash[40];
} MEM_DEVICE_INFO, *PMEM_DEVICE_INFO;

//Memory device info list header & tail
extern PDBLLIST_NODETYPE ___MMDEV_LIST_HEADER___;
extern PDBLLIST_NODETYPE ___MMDEV_LIST_TAIL___;
extern PDBLLIST_NODETYPE ___MMDEV_LIST_PTR___;
//The following pointers are only used for storing specific address when sending memory info to server
//For memory info is fetched together with the following info
extern PBIOS_INFO 			___BIOS_PTR___;
extern PPC_INFO 			___PC_PTR___;
extern PMOTHER_BOARD_INFO 	___MB_PTR___;

void InitBiosInfo(OUT PBIOS_INFO *bios_info);
void FreeBiosInfo(IN PBIOS_INFO bi);
void SerializeBiosInfo(IN PBIOS_INFO biosinfo, OUT char* out_str, OUT int *out_str_len);
void UnserializeBiosInfo(OUT PBIOS_INFO *biosinfo, IN char *in_str);

void InitPCInfo(OUT PPC_INFO *pc_info);
void FreePCInfo(IN PPC_INFO pi);
void SerializePCInfo(IN PPC_INFO pcinfo, OUT char* out_str, OUT int *out_str_len);
void UnserializePCInfo(OUT PPC_INFO *pcinfo, IN char *in_str);

void InitMotherBoardInfo(OUT PMOTHER_BOARD_INFO *mb_info);
void FreeMotherBoardInfo(IN PMOTHER_BOARD_INFO mbi);
void SerializeMBInfo(IN PMOTHER_BOARD_INFO mbinfo, OUT char* out_str, OUT int *out_str_len);
void UnserializeMBInfo(OUT PMOTHER_BOARD_INFO *mbinfo, IN char *in_str);

void InitMemDevInfo(OUT PMEM_DEVICE_INFO *mmdev_info);
void InitMMDevList();
void DestroyMMDevList();
void SerializeMMInfo(IN PMEM_DEVICE_INFO mminfo, OUT char* out_str, OUT int *out_str_len);
void UnserializeMMInfo(OUT PMEM_DEVICE_INFO *mminfo, IN char *in_str);

void PrepareMachineInfo(OUT PBIOS_INFO *bios_info, OUT PPC_INFO *pc_info, OUT PMOTHER_BOARD_INFO *mb_info);
void ReadMachineInfo(IN int fd, IN u32 base, IN int len, IN int num, IN int dmiversionmaj, IN int dmiversionmin, OUT PBIOS_INFO *bios_info, OUT PPC_INFO *pc_info, OUT PMOTHER_BOARD_INFO *mb_info);
void DestroyDMIInfo(IN PBIOS_INFO bios_info, IN PPC_INFO pc_info, IN PMOTHER_BOARD_INFO mb_info);

void print_bios_info(PBIOS_INFO bios_info);
void print_mb_info(PMOTHER_BOARD_INFO mb_info);
void print_mm_info(PMEM_DEVICE_INFO mm);
void mem_info_list_walk();
void print_pc_info(PPC_INFO pc_info);
#endif
