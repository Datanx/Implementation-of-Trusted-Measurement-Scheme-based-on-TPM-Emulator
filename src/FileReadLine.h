#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef FILEREADLINE_H
#define FILEREADLINE_H

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct LinkNode
{
	char *data;
	int leng;
	struct LinkNode *next;
};

int GetFileArray(IN char*, OUT char***);
void FreeFileArray(IN char**, IN int);

#endif
