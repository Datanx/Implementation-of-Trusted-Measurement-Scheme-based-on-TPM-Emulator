#ifndef NICINFO_H
#define NICINFO_H

#include "Defs.h"
#include "HWInfo.h"
#include "ds_DBLList.h"

//NIC device directory
#define NIC_DEVICE_DIR "/sys/class/net"

extern char *__NIC_DEVICE_ARRAY__[];

//NIC device type
#define NICTYPE_OTHER		0
#define NICTYPE_ETHCARD		1
#define NICTYPE_WLANCARD	2

//NIC device info list header & tail
extern PDBLLIST_NODETYPE ___NIC_LIST_HEADER___;
extern PDBLLIST_NODETYPE ___NIC_LIST_TAIL___;
extern PDBLLIST_NODETYPE ___NIC_LIST_PTR___;

//NIC device info
//definitions of returned value type of NIC functions
typedef int NIC_STATUS;
#define NIC_STATUS_SUCCESS						0
#define NIC_STATUS_MEM_ALLOC_FAILED				1
#define NIC_STATUS_NICNODE_CREATE_FAILED		2
#define NIC_STATUS_NICLIST_INIT_FAILED			10
#define NIC_STATUS_NICLISTNODE_CREATE_FAILED	11
#define NIC_STATUS_NICLISTNODE_APPEND_FAILED	12
#define NIC_STATUS_NIC_FILE_READ_FAILED			30

typedef struct _NIC_INFO
{
	int NIC_type;
	char Vendor[256];
	char Product[256];
	char MacAddr[20];
	char BusInfo[256];
	char LogicalName[20];
	char md5_hash[40];
} NIC_INFO, *PNIC_INFO;

NIC_STATUS InitNICList();
NIC_STATUS CreateNICList();
NIC_STATUS DestroyNICList();
void SerializeNICInfo(IN PNIC_INFO nic_info, OUT char* out_str, OUT int *out_str_len);
void UnserializeNICInfo(OUT PNIC_INFO *nic_info, IN char *in_str);

NIC_STATUS CreateNICListNode(OUT PDBLLIST_NODETYPE *listnode, IN char *vendor, IN char *prod, IN char *macaddr, IN char *businfo, IN char *logname);
NIC_STATUS AppendNICListNode(IN PDBLLIST_NODETYPE header, IN PDBLLIST_NODETYPE node);
NIC_STATUS init_nic_node(OUT PNIC_INFO *node);
NIC_STATUS create_nic_node(OUT PNIC_INFO *node, IN char *vendor, IN char *prod, IN char *macaddr, IN char *businfo, IN char *logname);
PDBLLIST_NODETYPE get_nic_list_header();
PDBLLIST_NODETYPE get_nic_list_tail();
void nic_list_walk();
void print_nic_info(PNIC_INFO nic);
int ven_srch_cmpfunc(IN void *array, IN int index, IN void *d);

#endif
