#ifndef USBINFO_H
#define USBINFO_H

#include "Defs.h"
#include "HWInfo.h"
#include "ds_DBLList.h"

//USB device file
#define USB_DEVICE_FILE		"/proc/bus/usb/devices"

//USB device type definitions
#define USBTYPE_OTHER_DEVICE				0
#define USBTYPE_INTERFACE					1
#define USBTYPE_PROTABLE_STORAGE_DEVICE		2
#define USBTYPE_KEYBOARD					3
#define USBTYPE_MOUSE						4

//USB device type characteristic value
#define UDTCV_INTERFACE						"Host Controller"
#define UDTCV_PROTABLE_STORAGE_DEVICE		"Storage"
#define UDTCV_KEYBOARD						"Keyboard"
#define UDTCV_MOUSE							"Mouse"

//USB device type array
extern char *___USB_DEVICE_ARRAY__[];

//USB device info list header & tail
extern 	PDBLLIST_NODETYPE 	___UDI_LIST_HEADER___;
extern 	PDBLLIST_NODETYPE 	___UDI_LIST_TAIL___;
extern	PDBLLIST_NODETYPE 	___UDI_PTR___;

//USB device file buffer
extern char **__USB_DEVICE_FILE_BUFFER_;
extern int USB_DEVICE_FILE_LENGTH;

//USB device info(UDI)
//definitions of returned value type of UDI functions
typedef int UDI_STATUS;
#define UDI_STATUS_SUCCESS						0
#define UDI_STATUS_MEM_ALLOC_FAILED				1
#define UDI_STATUS_UDINODE_CREATE_FAILED		2
#define UDI_STATUS_UDINODE_INIT_FAILED			3
#define UDI_STATUS_UDILIST_INIT_FAILED			10
#define UDI_STATUS_UDILISTNODE_CREATE_FAILED	11
#define UDI_STATUS_UDILISTNODE_APPEND_FAILED	12
#define UDI_STATUS_UDI_FILE_READ_FAILED			30

//USB device info node
typedef struct _USBINFO_NODE
{
	int usbtype;				//USB device type
	char Manufacturer[256];		//Manufacturer info. string 
	char Product[256];			//Product info. string
	char SN[256];				//SerialNumber string
	char md5_hash[40];			//md5 hash value
} USBINFO_NODE, *PUSBINFO_NODE;

//Get usb device type
//Param 1: usb device node
//Return: device type value
inline int GetUSBDeviceType(IN PUSBINFO_NODE node);

//Initializing UDI double linked list
//No input parameters
//Return: Function executing status
UDI_STATUS InitUDIList();

//Create UDI double linked list
//No input parameters
//Return: Function executing status
UDI_STATUS CreateUDIList();

//Destroy UID double linked list
//No input parameters
//Return: Function executing status
UDI_STATUS DestroyUDIList();

void SerializeUDIInfo(IN PUSBINFO_NODE udi_info, OUT char* out_str, OUT int *out_str_len);

void UnserializeUDIInfo(OUT PUSBINFO_NODE *udi_info, IN char *in_str);

//Create an UDI list node which includes an USB device information
//Param 1: New list node(for outputing)
//Param 2: USB device manufacture
//Param 3: USB device product
//Param 4: USB device serial number
//Return: Function executing status
UDI_STATUS CreateUDIListNode(OUT PDBLLIST_NODETYPE *listnode, IN char *manu, IN char *prod, IN char *sn);

//Appending a new UDI list node to UDI double linked list
//Param 1: UDI double linked list header
//Param 2: the appended node
//Return: Function executing status
UDI_STATUS AppendUDIListNode(IN PDBLLIST_NODETYPE header, IN PDBLLIST_NODETYPE node);

UDI_STATUS init_udi_node(OUT PUSBINFO_NODE *node);

//[Help function]
//Create an USB information node[USBINFO_NODE]
//Param 1: New USB information node[for outputing]
//Param 2: USB device manufacture
//Param 3: USB device product
//Param 4: USB device serial number
//Return: Function executing status
UDI_STATUS create_udi_node(OUT PUSBINFO_NODE *node, IN char *manu, IN char *prod, IN char *sn);

//[Help function]
//Read USB device file
//No input parameters
//Return: Function executing status
UDI_STATUS read_usb_device_file();

//[Help function]
//Free USB device file buffer
//No input parameters
//Return: Function executing status
UDI_STATUS free_usb_device_file();

//[Help function]
//Get UDI double linked list header
//No input parameter
//Return: UDI double linked list header
PDBLLIST_NODETYPE get_udi_list_header();

//[Help function]
//Get UDI double linked list tail
//No input parameter
//Return: UDI double linked list tail
PDBLLIST_NODETYPE get_udi_list_tail();

//[Help function]
//Print UDI double linked list (for testing)
//Param 1: Is animated when walking through the list
//Return: NULL
void udi_list_walk(int isAnimate);

//[Help function]
//Print UDI node
//Param 1: PUSBINFO_NODE type variable
//Return: NULL
void print_udi_node(IN PUSBINFO_NODE node);

#endif
