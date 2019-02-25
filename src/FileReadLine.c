#include "FileReadLine.h"

int GetFileArray(char* FilePath, char*** FileArray)
{
	//Inital Link List Head Node and Link List Present Node
	struct LinkNode HeadNode;
	struct LinkNode *PresentNode;
	HeadNode.data = NULL;
	HeadNode.next = NULL;
	HeadNode.leng = 0;
	PresentNode = &HeadNode;

	//Inital Buf for Reading and Line Processing
	char ReadBuf[1024];
	char LineBuf[1024];
	int  ReadCount = 0;
	int  LineCount = 0;
	int  WordCount = 0;

	//Clear the Buf
	bzero(ReadBuf, 1024*sizeof(char));
	bzero(LineBuf, 1024*sizeof(char));

	//Inital Loop Counter
	int  i = 0;

	//Open a File to Read
	int fd = open(FilePath, O_RDONLY);

	//Read File To ReadBuf and Processing
	while((ReadCount = read(fd, ReadBuf, sizeof(ReadBuf))) > 0)
	{
		for(i=0; i < ReadCount; i++)
		{
			if(ReadBuf[i]!='\n' && ReadBuf[i]!=EOF)
			{
				LineBuf[WordCount] = ReadBuf[i];
				WordCount++;
			}
			else
			{
				struct LinkNode *node = malloc(sizeof(struct LinkNode));
				node->data = malloc((WordCount+1)*sizeof(char));
				node->data[WordCount] = '\0';
				memcpy(node->data, LineBuf, WordCount*sizeof(char));
				node->leng = WordCount+1;
				node->next = NULL;
				PresentNode->next = node;
				PresentNode = node;
				LineCount++;
				WordCount = 0;
			}
		}
	}//while
	close(fd);
	
	//Copy the Link List to File Array and Release the Link Space on Heap. Especially the HeadNode is on the Stack
	
	char** tmp;
	tmp = malloc(LineCount*sizeof(char*));
	if(LineCount!=0)
	{
		PresentNode = HeadNode.next;
		i = 0;
		while(PresentNode!=NULL)
		{
			struct LinkNode *OldNode;
			OldNode = PresentNode;
			tmp[i] = malloc(PresentNode->leng*sizeof(char));
			memcpy(tmp[i], PresentNode->data, PresentNode->leng*sizeof(char));
			PresentNode = PresentNode->next;
			i++;
			free(OldNode->data);
			free(OldNode);
		}
	}
	
	//
	*FileArray = tmp;

	return LineCount;
}

void FreeFileArray(char** FileArray, int size)
{
	int i = 0;
	for(i = 0; i < size; i++)
		free(FileArray[i]);
	free(FileArray);
}
