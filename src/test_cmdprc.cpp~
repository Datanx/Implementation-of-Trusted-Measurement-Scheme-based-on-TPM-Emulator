#include <stdlib.h>
#include "CmdProcessor.h"

int main()
{
	char *outmsg=NULL;
	PCPU_INFO pCPU = NULL;
	PHDD_INFO pHDD = NULL;
	PPC_INFO pPC = NULL;
	PBIOS_INFO pBIOS = NULL;
	PMOTHER_BOARD_INFO pMB = NULL;
	int *MemNum = NULL;
	CmdProcessor cp;

	/*
	outmsg=cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_CPUINFO);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg=cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_CPUINFO);
	pCPU = (PCPU_INFO)cp.CmdParser(outmsg);
	print_cpu_info(pCPU);
	FreeCPUStruct(pCPU);
	free(outmsg);
	*/
	
	/*
	outmsg=cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_CPUMD5HASH);
	cp.CmdParser(outmsg);
	free(outmsg);

	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_HDDINFO);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_HDDMD5HASH);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_HDDINFO);
	pHDD = (PHDD_INFO)cp.CmdParser(outmsg);
	FreeHDDStruct(pHDD);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_PCINFO);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_PCINFO);
	pPC = (PPC_INFO)cp.CmdParser(outmsg);
	FreePCInfo(pPC);
	free(outmsg);

	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_BIOSINFO);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_BIOSINFO);
	pBIOS = (PBIOS_INFO)cp.CmdParser(outmsg);
	FreeBiosInfo(pBIOS);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MBINFO);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_MBINFO);
	pMB = (PMOTHER_BOARD_INFO)cp.CmdParser(outmsg);
	FreeMotherBoardInfo(pMB);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMNUM);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_MEMNUM);
	MemNum = (int*)cp.CmdParser(outmsg);
	free(MemNum);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMINFO);
	//cp.CmdParser(outmsg);		//Must debug with server
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBNUM);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBINFO);
	//cp.CmdParser(outmsg);		//Must debug with server
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICNUM);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	outmsg = cp.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICINFO);
	//cp.CmdParser(outmsg);		//Must debug with server
	free(outmsg);
	*/
	
	outmsg=cp.CmdCreator(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_FIREFOXINFO);
	//printf("%s\n", outmsg);
	cp.CmdParser(outmsg);
	free(outmsg);
	
	CSoftInfo* psi = NULL;
	outmsg = cp.CmdCreator(CMD_MJ_TRN_SWINFO, CMD_MN_TRN_FIREFOXINFO);
	psi = (CSoftInfo*)cp.CmdParser(outmsg);
	delete psi;
	free(outmsg);
	
	return 0;
}
