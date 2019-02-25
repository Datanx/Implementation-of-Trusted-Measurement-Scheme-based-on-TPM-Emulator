#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "usbinfo.h"

int main()
{
	PUSBINFO_NODE pUSB = NULL;
	//PUSBINFO_NODE pUSB2 = NULL;
	char *serializedStr = NULL;
	int serializedStrLen = 0;
	
	InitUDIList();
	CreateUDIList();

	printf("USB Device Number: %d\n", ___UDI_LIST_HEADER___->DATA_AREA.length);
	udi_list_walk(0);

	//Get an USB info struct
	pUSB = (PUSBINFO_NODE)(___UDI_LIST_HEADER___->next->DATA_AREA.data);
	//Serialize pUSB
	//serializedStr = (char*)malloc(sizeof(char) * HW_STRUCT_SERIALIZE_SIZE);
	//memset(serializedStr, 0, HW_STRUCT_SERIALIZE_SIZE);
	//SerializeUDIInfo(pUSB, serializedStr, &serializedStrLen);
	
	//Unserialize USB info
	//init_udi_node(&pUSB2);
	//UnserializeUDIInfo(&pUSB2, serializedStr);
	
	//print_udi_node(pUSB2);

	//free(pUSB2);
	//free(serializedStr);
	DestroyUDIList();

	return 0;
}
