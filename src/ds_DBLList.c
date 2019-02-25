#include "ds_DBLList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global variables
PDBLLIST_NODETYPE gHeaderNode, gTailNode;

DBLL_STATUS dbll_InitList(OUT PDBLLIST_NODETYPE *header, OUT PDBLLIST_NODETYPE *tail)
{
	DBLL_STATUS nRet = DBLL_STATUS_SUCCESS;
	
	//Allocate memory for header node
	*header = (PDBLLIST_NODETYPE)malloc(sizeof(DBLLIST_NODETYPE));
	if(*header == NULL)
	{
		nRet = DBLL_STATUS_MEM_ALLOC_FAILED;
		return nRet;
	}
	
	//Allocate memory for tail node
	*tail = (PDBLLIST_NODETYPE)malloc(sizeof(DBLLIST_NODETYPE));
	if(*tail == NULL)
	{
		nRet = DBLL_STATUS_MEM_ALLOC_FAILED;
		return nRet;
	}
	
	//Initialize header
	(*header)->nodetype = NTYPE_HDR;
	(*header)->DATA_AREA.length = 0;
	(*header)->prev = NULL;
	(*header)->next = NULL;
	
	//Initialize tail
	(*tail)->nodetype = NTYPE_TAIL;
	(*tail)->DATA_AREA.length = 0;
	(*tail)->prev = NULL;
	(*tail)->next = NULL;
	
	//Initialize the relationship between header & tail
	(*header)->next = *tail;
	(*tail)->prev = *header;
	
	return nRet;
}

DBLL_STATUS dbll_CreateNode(OUT PDBLLIST_NODETYPE *node, void *data, IN int datalength)
{
	DBLL_STATUS nRet = DBLL_STATUS_SUCCESS;
	
	//If node is not NULL, release it
	if(*node != NULL)
		free((void*)node);
	
	//allocate for new node
	*node = (PDBLLIST_NODETYPE)malloc(sizeof(DBLLIST_NODETYPE));
	if(*node==NULL)
		nRet = DBLL_STATUS_MEM_ALLOC_FAILED;
	
	//Initialize node
	(*node)->nodetype = NTYPE_DATA;
	(*node)->DATA_AREA.data = malloc(sizeof(char) * (datalength + 1));
        memset((*node)->DATA_AREA.data, 0, sizeof(char) * (datalength + 1));
	memcpy((*node)->DATA_AREA.data, data, datalength);
	(*node)->prev = NULL;
	(*node)->next = NULL;
	
	return nRet;
}

DBLL_STATUS dbll_AppendNode(PDBLLIST_NODETYPE header, PDBLLIST_NODETYPE node)
{
	DBLL_STATUS nRet = DBLL_STATUS_SUCCESS;
	
	if(node == NULL)
	{
		nRet = DBLL_STATUS_NULL_NODE;
		return nRet;
	}
	
	//Append the node after the header
	PDBLLIST_NODETYPE tmpNode;
	tmpNode = header->next;
	header->next = node;
	node->prev = header;
	node->next = tmpNode;
	tmpNode->prev = node;
	
	header->DATA_AREA.length++;
	
	return nRet;
}

DBLL_STATUS dbll_FindNodeByData(IN PDBLLIST_NODETYPE header, OUT PDBLLIST_NODETYPE *foundNode, IN void *data, IN CmpFunc comparefunc)
{
	DBLL_STATUS nRet = DBLL_STATUS_SUCCESS;
	*foundNode = NULL;
	PDBLLIST_NODETYPE tmpNode = NULL;
	
	if(header == NULL)
	{
		nRet = DBLL_STATUS_NULL_NODE;
		return nRet;
	}
	
	tmpNode = header->next;
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		if(comparefunc(tmpNode->DATA_AREA.data, data) == 0)
		{
			*foundNode = tmpNode;
			return nRet;
		}
		tmpNode = tmpNode->next;
	}
	
	nRet = DBLL_STATUS_NODE_NOT_FOUND;
	return nRet;
}

DBLL_STATUS dbll_DeleteNodeByData(PDBLLIST_NODETYPE header, void *data, CmpFunc func)
{
	DBLL_STATUS nRet = DBLL_STATUS_SUCCESS;
	
	PDBLLIST_NODETYPE foundNode = NULL;
	if(dbll_FindNodeByData(header, &foundNode, data, func) != DBLL_STATUS_SUCCESS)
	{
		nRet = DBLL_STATUS_DELETE_NODE_FAILED;
		
		return nRet;
	}
	
	PDBLLIST_NODETYPE prevNode = NULL;
	PDBLLIST_NODETYPE nextNode = NULL;
	nextNode = foundNode->next;
	prevNode = foundNode->prev;
	
	prevNode->next = nextNode;
	nextNode->prev = prevNode;
	
	//Decrease list length
	header->DATA_AREA.length--;
	
	//Free memory
	free(foundNode->DATA_AREA.data);
	free(foundNode);
	
	return nRet;
}

int dbll_GetListLength(PDBLLIST_NODETYPE header)
{
	if(header != NULL)
	{
		return header->DATA_AREA.length;
	}
	return 0;
}

void dbll_DestoryList(OUT PDBLLIST_NODETYPE *header)
{
	PDBLLIST_NODETYPE tmpNode = NULL;
	PDBLLIST_NODETYPE delNode = NULL;
	
	tmpNode = (*header)->next;
	while(tmpNode->nodetype != NTYPE_TAIL)
	{
		delNode = tmpNode;
		
		tmpNode->prev->next = tmpNode->next;
		tmpNode->next->prev = tmpNode->prev;
		
		tmpNode = tmpNode->next;
		
		free(delNode->DATA_AREA.data);
		free(delNode);
	}
	
	free((*header)->next);	//free tail node
	free((*header));
}

void dbll_PrintList(IN PDBLLIST_NODETYPE header, IN PrintDBLIst print)
{
    if(header == NULL)
        return;

    print(header);
}
