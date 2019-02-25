#ifndef DS_DBLLIST_H
#define DS_DBLLIST_H

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif
//A double linked list is implemented in this file
//List Header
#define NTYPE_HDR	0
//List Tail
#define NTYPE_TAIL	1
//List Data
#define NTYPE_DATA	2

typedef int DBLL_STATUS;
#define DBLL_STATUS_SUCCESS 			0
#define DBLL_STATUS_MEM_ALLOC_FAILED	1
#define DBLL_STATUS_NODE_NOT_FOUND		2
#define DBLL_STATUS_NULL_NODE			3
#define DBLL_STATUS_DELETE_NODE_FAILED			4

//Compare function type
typedef int(*CmpFunc)(void *d1, void *d2);

//List Node Type
typedef struct _DBLLIST_NODETYPE
{
	int nodetype;	//Node Type (NTYPE_HDR, NTYPE_TAIL, NTYPE_DATA)
	union			//Node data field
	{
		void *data;	//node data
		int length;	//list length
	} DATA_AREA;
	struct _DBLLIST_NODETYPE* prev;	//previous node
	struct _DBLLIST_NODETYPE* next;	//next node
} DBLLIST_NODETYPE, *PDBLLIST_NODETYPE;

typedef void(*PrintDBLIst)(IN PDBLLIST_NODETYPE header);

////Common Function////
//Function: Initialize list header & tail
//Param 1: list header pointer
//Param 2: list tail pointer
//Return: DBLL_STATUS
DBLL_STATUS dbll_InitList(OUT PDBLLIST_NODETYPE *header, OUT PDBLLIST_NODETYPE *tail);

////Common Function////
//Function: Create a list node
//Param 1: new node pointer
//Param 2: node data pointer
//Param 3: data length
//Return: DBLL_STATUS
DBLL_STATUS dbll_CreateNode(OUT PDBLLIST_NODETYPE *node, IN void *data, IN int datalength);

////Common Function////
//Function: Append a node to list
//Param 1: list header
//Param 2: new node
//Return: DBLL_STATUS
DBLL_STATUS dbll_AppendNode(IN PDBLLIST_NODETYPE header, IN PDBLLIST_NODETYPE node);

////Common Function////
//Function: Find a node by data value
//Param 1: list header
//Param 2: found node pointer
//Param 3: data pointer
//Param 4: data compare function pointer
//Return: DBLL_STATUS
DBLL_STATUS dbll_FindNodeByData(IN PDBLLIST_NODETYPE header, OUT PDBLLIST_NODETYPE *foundNode, IN void *data, IN CmpFunc comparefunc);

////Common Function////
//Function: Delete a node by data value
//Param 1: list header
//Param 2: data pointer
//Param 3: data compare function pointer
//Return: DBLL_STATUS
DBLL_STATUS dbll_DeleteNodeByData(IN PDBLLIST_NODETYPE header, IN void *data, IN CmpFunc func);

////Common Function////
//Function: Get list length
//Param 1: list header
//Return: list length
int dbll_GetListLength(IN PDBLLIST_NODETYPE header);

////Common Function////
//Function: Destroy list
//Param 1: list header
//Return: NULL
void dbll_DestoryList(OUT PDBLLIST_NODETYPE *header);

void dbll_PrintList(IN PDBLLIST_NODETYPE header, IN PrintDBLIst print);
#endif
