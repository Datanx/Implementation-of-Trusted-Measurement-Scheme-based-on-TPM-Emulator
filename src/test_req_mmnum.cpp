#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CmdProcessor.h"

int main()
{
	char *outmsg=NULL;
	int *MemNum = NULL;
	int i = 0;
	CmdProcessor cp;

	for(i=0; i<200; i++)
	{
		outmsg = NULL;
		MemNum = NULL;
		outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMNUM);
		cp.CmdParser(outmsg);
		free(outmsg);
				
		outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_MEMNUM);
		MemNum = (int*)cp.CmdParser(outmsg);
		free(MemNum);
		free(outmsg);
	}

	return 0;
}
