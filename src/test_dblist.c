#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ds_DBLList.h"

int main()
{
	PDBLLIST_NODETYPE list_header = NULL;
	PDBLLIST_NODETYPE list_tail = NULL;
	PDBLLIST_NODETYPE newNode = NULL;
	PDBLLIST_NODETYPE tmpNode = NULL;
	int i = 0;
	
	dbll_InitList(&list_header, &list_tail);
	for(i = 0; i < 20; i++)
	{
		dbll_CreateNode(&newNode, &i, sizeof(int));
		dbll_AppendNode(list_header, newNode);
		newNode = NULL;
	}
	
	tmpNode = list_header->next;
	i = 0;
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		printf("Node %d = %d\n", i, *((int*)tmpNode->DATA_AREA.data));
		tmpNode = tmpNode->next;
		i ++;
	}
	
	dbll_DestoryList(&list_header);
	
	return 0;
}
