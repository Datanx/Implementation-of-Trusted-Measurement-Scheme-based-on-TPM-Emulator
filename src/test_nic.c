#include "nicinfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	PNIC_INFO pNIC = NULL;
	PNIC_INFO pNIC2 = NULL;
	char serializedInfo[HW_STRUCT_SERIALIZE_SIZE] = {0};
	int serializedInfoLen = 0;
	
	InitNICList();
	CreateNICList();
	
	pNIC =(PNIC_INFO)(___NIC_LIST_HEADER___->next->DATA_AREA.data);
	SerializeNICInfo(pNIC, serializedInfo, &serializedInfoLen);
	
	init_nic_node(&pNIC2);
	UnserializeNICInfo(&pNIC2,serializedInfo);
	
	nic_list_walk();
	print_nic_info(pNIC2);
	
	free(pNIC2);
	
	DestroyNICList();
	
	return 0;
}
