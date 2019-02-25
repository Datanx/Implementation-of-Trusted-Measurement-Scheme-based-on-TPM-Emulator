#include "nicinfo.h"
#include "md5lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <unistd.h>
#include <limits.h>
#include "alg.h"
#include "pcihdr.h"
#include "ConsoleColor.h"
#include "Animation.h"

//USB device type array
char *__NIC_DEVICE_ARRAY__[]={
"Others",
"Ethernet Card",
"Wireless Card"
};

//NIC device info list header & tail
PDBLLIST_NODETYPE ___NIC_LIST_HEADER___ 	= NULL;
PDBLLIST_NODETYPE ___NIC_LIST_TAIL___ 		= NULL;
PDBLLIST_NODETYPE ___NIC_LIST_PTR___ 		= NULL;

NIC_STATUS InitNICList()
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	
	//Create NIC list header & tail
	if(dbll_InitList(&___NIC_LIST_HEADER___, &___NIC_LIST_TAIL___) != DBLL_STATUS_SUCCESS)
	{
		nRet = NIC_STATUS_NICLIST_INIT_FAILED;
		return nRet;
	}
	
	return nRet;
}

NIC_STATUS CreateNICList()
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	
	int i = 0, j = 0;
	char buf[256] = {0};					//Temp buffer
	char buf2[256] = {0};					//Temp buffer 2
	char *ptrch = NULL;						//Temp char pointer
	char mac_address[20] = {0};				//Mac Address
	char vendor[256] = {0};					//Vendor
	unsigned short vendor_num = 0;			//Vendor Number
	int vendor_index = -1;					//Found vendor index in PciVenTable
	char product[256] = {0};
	int unsigned short device_num = 0;		//PCI Device Number
	int device_index = -1;					//PCI Device index in PciDevTable
	char businfo[256] = {0};
	char logical_name[20] = {0};
	PDBLLIST_NODETYPE newNICNode = NULL;	//new NIC node
	char nic_logicalname[20][32];			//NIC logical name array
	int nic_logicalname_count = 0;			//NIC logical name count
	struct dirent* ent = NULL;
	DIR *pDir;
	FILE *fp;
	
	////Get all NIC logical name////
	//initialize NIC logical name array
	for(i = 0; i < 20; i++)
	{
		memset(nic_logicalname[i], 0, 32);
	}
	
	//scanning /sys/class/net
	pDir = opendir("/sys/class/net");
	while(NULL != (ent=readdir(pDir)))
	{
		if (ent->d_reclen==16)	//dir file
		{
			if (ent->d_type==4 || ent->d_type==10)	//dir
			{
				//Network card information directory
				if(strstr(ent->d_name, "eth")!=NULL || strstr(ent->d_name, "wlan") != NULL)
				{
					strcpy(nic_logicalname[nic_logicalname_count], ent->d_name);
					nic_logicalname_count++;
				}
			}
		}
	}//while
	closedir(pDir);
	////End of get NIC logical name/////
	////////////////////////////////////
	
	////Processing each NIC device////
	for(i=0; i<nic_logicalname_count; i++)
	{
		//Get Mac Address
		memset(buf, 0, 256);
		sprintf(buf, "%s%s%s", "/sys/class/net/", nic_logicalname[i], "/address");
		fp = fopen(buf, "r");
		memset(mac_address, 0, 20);
		fread(mac_address, sizeof(char), 17, fp);
		fclose(fp);
		
		//Get Vendor info
		memset(buf, 0, 256);
		sprintf(buf, "%s%s%s", "/sys/class/net/", nic_logicalname[i], "/device/vendor");
		fp = fopen(buf, "r");
		memset(buf2, 0, 256);
		fread(buf2, sizeof(char), 6, fp);
		vendor_num = Str2Int(buf2);
		vendor_index = BinSrch(PciVenTable, &vendor_num, 0, PCI_VENTABLE_LEN-1, ven_srch_cmpfunc);
		if(vendor_index >= 0)
			strcpy(vendor, PciVenTable[vendor_index].VenFull);
		fclose(fp);
		
		//Get Product info
		memset(buf, 0, 256);
		sprintf(buf, "%s%s%s", "/sys/class/net/", nic_logicalname[i], "/device/device");
		fp = fopen(buf, "r");
		memset(buf2, 0, 256);
		fread(buf2, sizeof(char), 6, fp);
		fclose(fp);
		device_num = Str2Int(buf2);
		for(j=0; j<PCI_DEVTABLE_LEN; j++)	//find the beginning of specific vendor group
		{
			if(PciDevTable[j].VenId == vendor_num)
				break;
		}
		while(j < PCI_DEVTABLE_LEN)			//find the specific device in the vendor group
		{
			if(PciDevTable[j].VenId != vendor_num)
				break;
			if(PciDevTable[j].DevId == device_num)
			{
				device_index = j;
				break;
			}
			j++;
		}
		if(device_index >= 0)	//device information has been found
			strcpy(product, PciDevTable[device_index].ChipDesc);
			
		//Get bus info
		memset(buf, 0, 256);
		sprintf(buf, "%s%s%s", "/sys/class/net/", nic_logicalname[i], "/device");
		memset(buf2, 0, 256);
		realpath(buf, buf2);		//convert the symbol link to real path
		ptrch = strstr(buf2, "pci");
		strcpy(businfo, ptrch);
		
		//////Create NIC node and append it to NIC list//////
		newNICNode = NULL;
		if(CreateNICListNode(&newNICNode, vendor, product, mac_address, businfo, nic_logicalname[i]) != NIC_STATUS_SUCCESS)
		{
			nRet = NIC_STATUS_NICLISTNODE_CREATE_FAILED;
			return nRet;
		}
		if(AppendNICListNode(___NIC_LIST_HEADER___, newNICNode) != NIC_STATUS_SUCCESS)
		{
			nRet = NIC_STATUS_NICLISTNODE_APPEND_FAILED;
			return nRet;
		}
	}//for
	
	return nRet;
}

NIC_STATUS DestroyNICList()
{
	dbll_DestoryList(&___NIC_LIST_HEADER___);
        ___NIC_LIST_HEADER___ = NULL;
        ___NIC_LIST_TAIL___ = NULL;
        ___NIC_LIST_PTR___ = NULL;
	
	return NIC_STATUS_SUCCESS;
}

void SerializeNICInfo(IN PNIC_INFO nic_info, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    //NIC type
    n = sizeof(nic_info->NIC_type);
    memcpy(ptr, &(nic_info->NIC_type), n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(nic_info->Vendor);
    memcpy(ptr, nic_info->Vendor, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(nic_info->Product);
    memcpy(ptr, nic_info->Product, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(nic_info->MacAddr);
    memcpy(ptr, nic_info->MacAddr, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(nic_info->BusInfo);
    memcpy(ptr, nic_info->BusInfo, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(nic_info->LogicalName);
    memcpy(ptr, nic_info->LogicalName, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);

	n = strlen(nic_info->md5_hash);
    memcpy(ptr, nic_info->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializeNICInfo(OUT PNIC_INFO *nic_info, IN char *in_str)
{
	if(*nic_info == NULL || in_str == NULL)
        return;

    char *ptr = in_str;

	memcpy(&((*nic_info)->NIC_type), (int*)ptr, sizeof(int));
	ptr += 1 + sizeof(int);
	
    strcpy((*nic_info)->Vendor, ptr);
    ptr += 1 + strlen((*nic_info)->Vendor);
    
    strcpy((*nic_info)->Product, ptr);
    ptr += 1 + strlen((*nic_info)->Product);
    
    strcpy((*nic_info)->MacAddr, ptr);
    ptr += 1 + strlen((*nic_info)->MacAddr);
    
    strcpy((*nic_info)->BusInfo, ptr);
    ptr += 1 + strlen((*nic_info)->BusInfo);
    
    strcpy((*nic_info)->LogicalName, ptr);
    ptr += 1 + strlen((*nic_info)->LogicalName);
    
    strcpy((*nic_info)->md5_hash, ptr);

    return;
}

NIC_STATUS CreateNICListNode(OUT PDBLLIST_NODETYPE *listnode, IN char *vendor, IN char *prod, IN char *macaddr, IN char *businfo, IN char *logname)
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	
	//Create a NIC node first
	PNIC_INFO newNICInfo = NULL;
	if(create_nic_node(&newNICInfo, vendor, prod, macaddr, businfo, logname) != NIC_STATUS_SUCCESS)
	{
		nRet = NIC_STATUS_NICNODE_CREATE_FAILED;
		return nRet;
	}
	
	//Create list node
	if(dbll_CreateNode(listnode, (void*)newNICInfo, sizeof(NIC_INFO)) != DBLL_STATUS_SUCCESS)
	{
		nRet = NIC_STATUS_NICLISTNODE_CREATE_FAILED;
		return nRet;
	}
	
	//Free NIC node
	free(newNICInfo);
	
	return nRet;
}

NIC_STATUS AppendNICListNode(IN PDBLLIST_NODETYPE header, IN PDBLLIST_NODETYPE node)
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	
	//Append list node to NIC list
	if(dbll_AppendNode(header, node) != DBLL_STATUS_SUCCESS)
	{
		nRet = NIC_STATUS_NICLISTNODE_APPEND_FAILED;
		return nRet;
	}
	
	return nRet;
}

NIC_STATUS init_nic_node(OUT PNIC_INFO *node)
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	PNIC_INFO nd = NULL;
	
	if(*node != NULL)
		free(*node);
		
	*node = (PNIC_INFO)malloc(sizeof(NIC_INFO));
	if(*node == NULL)
	{
		nRet = NIC_STATUS_MEM_ALLOC_FAILED;
		return nRet;
	}
	
	nd = *node;
	memset(nd->Vendor, 0, 256);
	memset(nd->Product, 0, 256);
	memset(nd->MacAddr, 0, 20);
	memset(nd->BusInfo, 0, 256);
	memset(nd->LogicalName, 0, 20);
	memset(nd->md5_hash, 0, 40);
	
	return nRet;
}

NIC_STATUS create_nic_node(OUT PNIC_INFO *node, char *vendor, char *prod, char *macaddr, char *businfo, char *logname)
{
	NIC_STATUS nRet = NIC_STATUS_SUCCESS;
	PNIC_INFO nd;
	char infobuffer[1024] = {0};
	char *bufptr = NULL;
	char *md5str = NULL;
	int n = 0;
	
	if(*node != NULL)
		free(*node);
		
	init_nic_node(node);
	
	nd = *node;
	//Copy values
	strcpy(nd->Vendor, vendor);
	strcpy(nd->Product, prod);
	strcpy(nd->MacAddr, macaddr);
	strcpy(nd->BusInfo, businfo);
	strcpy(nd->LogicalName, logname);
	
	//Initialize infobuffer
	memset(infobuffer, 0, 1024);
	
	//Construct info buffer
	n = strlen(vendor);
	strcpy(infobuffer, vendor);
	bufptr = infobuffer + n;
	
	n = strlen(prod);
	strcpy(bufptr, prod);
	bufptr += n;
	
	n = strlen(macaddr);
	strcpy(bufptr, macaddr);
	bufptr += n;
	
	n = strlen(businfo);
	strcpy(bufptr, businfo);
	bufptr += n;
	
	n = strlen(logname);
	strcpy(bufptr, logname);
	bufptr += n;
	
	*bufptr = '\0';
	
	md5str = MDString(infobuffer);
	n = strlen(md5str);
	strcpy(nd->md5_hash, md5str);
	
	//Determine device type
	if(strstr(logname, "eth") != NULL)
		nd->NIC_type = NICTYPE_ETHCARD;
	else if(strstr(logname, "wlan") != NULL)
		nd->NIC_type = NICTYPE_WLANCARD;
	else
		nd->NIC_type = NICTYPE_OTHER;
	
	return nRet;
}

int ven_srch_cmpfunc(void *array, int index, void *d)
{
	PPCI_VENTABLE pci_array = (PPCI_VENTABLE)array;
	PPCI_VENTABLE elem = &(pci_array[index]);
	int data = *((int *)d);
	
	if(elem->VenId < data)
		return -1;
	else if(elem->VenId > data)
		return 1;
	else
		return 0;
}

PDBLLIST_NODETYPE get_nic_list_header()
{
	return  ___NIC_LIST_HEADER___;
}

PDBLLIST_NODETYPE get_nic_list_tail()
{
	return ___NIC_LIST_TAIL___;
}

void print_nic_info(PNIC_INFO nic)
{
	printf("\n\tNIC type: %s\n", __NIC_DEVICE_ARRAY__[nic->NIC_type]);
	printf("\tManufacture: %s\n", nic->Vendor);
	printf("\tProduct: %s\n", nic->Product);
	printf("\tMac Address: %s\n", nic->MacAddr);
	printf("\tBus Info: %s\n", nic->BusInfo);
	printf("\tLogical Name: %s\n", nic->LogicalName);
	printf("\tmd5 hash: %s\n", nic->md5_hash);
		
	printf("\n");
}

void nic_list_walk()
{
	PDBLLIST_NODETYPE tmpNode = NULL;
	PNIC_INFO tmpNICNode = NULL;
	int i = 0;
	
	if(get_nic_list_header() == NULL)
	{
		return;
	}
	
	if(get_nic_list_header()->DATA_AREA.length <= 0)
	{
		return;
	}
	
	i = 1;
	tmpNode = ___NIC_LIST_HEADER___->next;
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		printf("NIC %d Info:", i);
		//RotatePrint(1, ".", SOW_OFF);

		tmpNICNode = (PNIC_INFO)(tmpNode->DATA_AREA.data);
		print_nic_info(tmpNICNode);
		
		tmpNode = tmpNode->next;
		i++;
	}
}
