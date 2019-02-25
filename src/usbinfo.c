#include "usbinfo.h"
#include "FileReadLine.h"
#include "ConsoleColor.h"
#include "Animation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//USB device type array
char *___USB_DEVICE_ARRAY__[]={
"Others",
"Empty Interface",
"Protable Storage",
"Keyboard",
"Mouse"
};

//USB device info list header & tail
PDBLLIST_NODETYPE ___UDI_LIST_HEADER___ 	= NULL;
PDBLLIST_NODETYPE ___UDI_LIST_TAIL___ 		= NULL;
PDBLLIST_NODETYPE ___UDI_PTR___				= NULL;

//USB device file buffer
char **__USB_DEVICE_FILE_BUFFER_;
int USB_DEVICE_FILE_LENGTH;

int GetUSBDeviceType(IN PUSBINFO_NODE node)
{
	char *p = node->Product;
	char *tmp_s = NULL;
	
	if(*p != '\0')
	{
		tmp_s = strstr(p, UDTCV_INTERFACE);
		if(tmp_s != NULL)
			return USBTYPE_INTERFACE;
		
		tmp_s = strstr(p, UDTCV_PROTABLE_STORAGE_DEVICE);
		if(tmp_s != NULL)
			return USBTYPE_PROTABLE_STORAGE_DEVICE;
		
		tmp_s = strstr(p, UDTCV_KEYBOARD);
		if(tmp_s != NULL)
			return USBTYPE_KEYBOARD;
		
		tmp_s = strstr(p, UDTCV_MOUSE);
		if(tmp_s != NULL)
			return USBTYPE_MOUSE;
	}
	
	return USBTYPE_OTHER_DEVICE;
}

UDI_STATUS InitUDIList()
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	
	//Create UDI list header & tail
	if(dbll_InitList(&___UDI_LIST_HEADER___, &___UDI_LIST_TAIL___) != DBLL_STATUS_SUCCESS)
	{
		nRet = UDI_STATUS_UDILIST_INIT_FAILED;
		return nRet;
	}
	
	return nRet;
}

UDI_STATUS CreateUDIList()
{
	int i = 0;
	char *tmp_str = NULL;
	char *bgn_ptr = NULL;
	char *strptr = NULL;
	char *Manu_ptr = NULL;	//"Manufacturer string beginning pointer"
	char *Prod_ptr = NULL;	//"Product string beginning pointer"
	char *SN_ptr = NULL;		//"SerialNumber string beginning pointer"
	
	char Manu[256] = {0};
	char Prod[256] = {0};
	char SN[256] = {0};
	
	PDBLLIST_NODETYPE newUDINode = NULL;	//new UDI node
	
	//Read USB device file
	read_usb_device_file();
	
	//scann USB device file buffer and build UDI list
	i = 0;
	while (i < USB_DEVICE_FILE_LENGTH)
	{
		if(strstr(__USB_DEVICE_FILE_BUFFER_[i], "T:") != NULL)
		{
			i++;	//go on
			//reset Manu. Prod, SN
			memset(Manu, 0, 256);
			memset(Prod, 0, 256);
			memset(SN, 0, 256);
			//begin scanning one USB info group
			while(i < USB_DEVICE_FILE_LENGTH && (bgn_ptr=strstr(__USB_DEVICE_FILE_BUFFER_[i], "T:")) == NULL)
			{
				if((strptr=strstr(__USB_DEVICE_FILE_BUFFER_[i], "S:")) != NULL)
				{
					if((Manu_ptr = strstr(strptr, "Manufacturer")) != NULL)	//Manufacturer information found
					{
						//Search for "=" token
						tmp_str = strstr(Manu_ptr, "=");
						tmp_str++; //move pointer to next character
						strcpy(Manu, tmp_str);
					}
					if((Prod_ptr = strstr(strptr, "Product")) != NULL)	//Product information found
					{
						//Search for "=" token
						tmp_str = strstr(Prod_ptr, "=");
						tmp_str++; //move pointer to next character
						strcpy(Prod, tmp_str);
					}
					if((SN_ptr = strstr(strptr, "SerialNumber")) != NULL)	//SerialNumber information found
					{
						//Search for "=" token
						tmp_str = strstr(SN_ptr, "=");
						tmp_str++; //move pointer to next character
						strcpy(SN, tmp_str);
					}
				}//if
				i++;
			}//while(next T: found)

			//if USB is an empty interface, ignore it
			if(strstr(Prod, UDTCV_INTERFACE) != NULL)
				continue;

			//USB info group scanning complete
			//Create UDI list node
			newUDINode = NULL;
			if(CreateUDIListNode(&newUDINode, Manu, Prod, SN) != UDI_STATUS_SUCCESS)
			{
				return UDI_STATUS_UDILISTNODE_CREATE_FAILED;
			}
			//Append the new UDI list node to UDI list
			if(AppendUDIListNode(get_udi_list_header(), newUDINode) != UDI_STATUS_SUCCESS)
			{
				return UDI_STATUS_UDILISTNODE_APPEND_FAILED;
			}
			
		}//if(One USB info group end)
		
		if(bgn_ptr == NULL)
			i++;
	}//while
	
	//Free usb device file buffer
	free_usb_device_file();
	
	return UDI_STATUS_SUCCESS;
}

UDI_STATUS DestroyUDIList()
{
	dbll_DestoryList(&___UDI_LIST_HEADER___);
        ___UDI_LIST_HEADER___ = NULL;
        ___UDI_LIST_TAIL___ = NULL;
        ___UDI_PTR___ = NULL;
	
	return UDI_STATUS_SUCCESS;
}

void SerializeUDIInfo(IN PUSBINFO_NODE udi_info, OUT char* out_str, OUT int *out_str_len)
{
	if(out_str == NULL)
        return;

    char *ptr = out_str;
    *out_str_len = 0;
    int n = 0;
    
    memset(out_str, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    //USB type
    n = sizeof(udi_info->usbtype);
    memcpy(ptr, &(udi_info->usbtype), n);
    ptr += 1 + n;
    *out_str_len += (1 + n);
    
    n = strlen(udi_info->Manufacturer);
    memcpy(ptr, udi_info->Manufacturer, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(udi_info->Product);
    memcpy(ptr, udi_info->Product, n);
    ptr += 1 + n;
    *out_str_len  += (1 + n);
    
    n = strlen(udi_info->SN);
    memcpy(ptr, udi_info->SN, n);
    ptr += 1 + n;
    *out_str_len += (1 + n);

	n = strlen(udi_info->md5_hash);
    memcpy(ptr, udi_info->md5_hash, n);
    ptr += 1+n;
    *out_str_len += (1 + n);

    return;
}

void UnserializeUDIInfo(OUT PUSBINFO_NODE *udi_info, IN char *in_str)
{
	if(*udi_info == NULL || in_str == NULL)
        return;

    char *ptr = in_str;

	memcpy(&((*udi_info)->usbtype), (int*)ptr, sizeof(int));
	ptr += 1 + sizeof(int);
	
    strcpy((*udi_info)->Manufacturer, ptr);
    ptr += 1 + strlen((*udi_info)->Manufacturer);
    
    strcpy((*udi_info)->Product, ptr);
    ptr += 1 + strlen((*udi_info)->Product);
    
    strcpy((*udi_info)->SN, ptr);
    ptr += 1 + strlen((*udi_info)->SN);
    
    strcpy((*udi_info)->md5_hash, ptr);

    return;
}

UDI_STATUS CreateUDIListNode(OUT PDBLLIST_NODETYPE *listnode, IN char *manu, IN char *prod, IN char *sn)
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	
	//Create an UDI node first
	PUSBINFO_NODE newUDINode = NULL;
	if(create_udi_node(&newUDINode, manu, prod, sn) != UDI_STATUS_SUCCESS)
	{
		nRet = UDI_STATUS_UDINODE_CREATE_FAILED;
		return nRet;
	}
	
	//Create list node
	if(dbll_CreateNode(listnode, (void*)newUDINode, sizeof(USBINFO_NODE)) != DBLL_STATUS_SUCCESS)
	{
		nRet = UDI_STATUS_UDILISTNODE_CREATE_FAILED;
		return nRet;
	}
	
	//Free UDI Node
	free(newUDINode);
	
	return nRet;
}

UDI_STATUS AppendUDIListNode(IN PDBLLIST_NODETYPE header, IN PDBLLIST_NODETYPE node)
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	
	if(dbll_AppendNode(header, node) != DBLL_STATUS_SUCCESS)
	{
		nRet = UDI_STATUS_UDILISTNODE_APPEND_FAILED;
		return nRet;
	}
	
	return nRet;
}

UDI_STATUS init_udi_node(OUT PUSBINFO_NODE *node)
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	
	if(*node != NULL)
		free(*node);
	
	*node = (PUSBINFO_NODE)malloc(sizeof(USBINFO_NODE));
	if(*node == NULL)
	{
		nRet = UDI_STATUS_MEM_ALLOC_FAILED;
		return nRet;
	}
	memset((*node)->Manufacturer, 0, 256);
	memset((*node)->Product, 0, 256);
	memset((*node)->SN, 0, 256);
	memset((*node)->md5_hash, 0, 40);
	
	return nRet;
}

UDI_STATUS create_udi_node(PUSBINFO_NODE *node, char *manu, char *prod, char *sn)
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	char infobuffer[1024];
	char *bufptr = NULL;
	char *md5str = NULL;
	int n = 0;
	
	if(init_udi_node(node) != UDI_STATUS_SUCCESS)
		return UDI_STATUS_UDINODE_INIT_FAILED;
	
	strcpy((*node)->Manufacturer, manu);
	strcpy((*node)->Product, prod);
	strcpy((*node)->SN, sn);
	
	//initialize infobuffer
	memset(infobuffer, 0, 1024);
	n = strlen(manu);
	
	//Construct usb info string
	strcpy(infobuffer, manu);
	bufptr = infobuffer + n;
	
	n = strlen(prod);
	strcpy(bufptr, prod);
	bufptr += n;
	
	n = strlen(sn);
	strcpy(bufptr, sn);
	bufptr += n;
	
	*bufptr = '\0';
	
	//Calculate md5 value of usb info string
	md5str = (char*)MDString(infobuffer);
	n = strlen(md5str);
	strcpy((*node)->md5_hash, md5str);
	
	//To determine device type
	(*node)->usbtype = GetUSBDeviceType(*node);
	
	return nRet;
}

PDBLLIST_NODETYPE get_udi_list_header()
{
	return  ___UDI_LIST_HEADER___;
}

PDBLLIST_NODETYPE get_udi_list_tail()
{
	return ___UDI_LIST_TAIL___;
}

void udi_list_walk(int isAnimate)
{
	PDBLLIST_NODETYPE tmpNode = NULL;
	PUSBINFO_NODE tmpUDINode = NULL;
	int i = 0;
	
	if(get_udi_list_header() == NULL)
	{
		return;
	}
	
	if(get_udi_list_header()->DATA_AREA.length <= 0)
	{
		return;
	}
	
	i = 1;
	tmpNode = ___UDI_LIST_HEADER___->next;
	Console_SetTextColor(CONSOLECOLOR_WHITE, CONSOLECOLOR_BG_BLACK);
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		printf("\nUSB %d Info:", i);
		if(isAnimate)
			RotatePrint(1, ".", SOW_OFF);

		tmpUDINode = (PUSBINFO_NODE)(tmpNode->DATA_AREA.data);
		print_udi_node(tmpUDINode);
		
		printf("\n");
		
		tmpNode = tmpNode->next;
		i++;
	}
}

void print_udi_node(IN PUSBINFO_NODE node)
{
	printf("\n\tUSB type: %s\n", ___USB_DEVICE_ARRAY__[node->usbtype]);
	printf("\tManufacture: %s\n", node->Manufacturer);
	printf("\tProduct: %s\n", node->Product);
	printf("\tSerialNumber: %s\n", node->SN);
	printf("\tmd5 hash: %s\n", node->md5_hash);
		
	printf("\n");
}

UDI_STATUS read_usb_device_file()
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;

	USB_DEVICE_FILE_LENGTH = 0;
	
	USB_DEVICE_FILE_LENGTH = GetFileArray(USB_DEVICE_FILE, &__USB_DEVICE_FILE_BUFFER_);
	
	if(USB_DEVICE_FILE_LENGTH <= 0)
	{
		return UDI_STATUS_UDI_FILE_READ_FAILED;
	}
	
	return nRet;
}

UDI_STATUS free_usb_device_file()
{
	UDI_STATUS nRet = UDI_STATUS_SUCCESS;
	
	FreeFileArray(__USB_DEVICE_FILE_BUFFER_, USB_DEVICE_FILE_LENGTH);
	
	return nRet;
}
