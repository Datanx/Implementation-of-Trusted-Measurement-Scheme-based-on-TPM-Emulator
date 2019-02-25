#include "HWCheck.h"

static char* get_filename_by_kw(char* path, char* kw)
{
    struct dirent* ent = NULL;
    DIR *pDir = NULL;
    static char* pfname = NULL;

    pDir = opendir(path);
    while(NULL != (ent=readdir(pDir)))
    {
        if(ent->d_type == 8)    //file
        {
            if((pfname=strstr(ent->d_name, kw)) != NULL)
                return pfname;
        }
    }

    return NULL;
}

HWCHK_STATUS CheckCPU(char *cpuinfo_file_path)
{
    FILE *fp;
    char* pfname = NULL;
    char filename[256]={0};
    char buf[256]={0};
    PCPU_INFO pCPUinfo = NULL;

    if(__HWLIST__[HWPRT_CPU] != NULL)
    {
        pCPUinfo = (PCPU_INFO)(__HWLIST__[HWPRT_CPU]);	//Get CPU info pointer from hardware info list
        pfname = get_filename_by_kw(cpuinfo_file_path, "cpuid");
        if(pfname == NULL)  //cpuid not found, no checking on CPU
            return HWCHK_STATUS_IGNORE_CPU;

        //Read cpuid file
        sprintf(filename, "%s%s", cpuinfo_file_path, pfname);
        fp = fopen(filename, "r");
        if(fp == NULL)
            return HWCHK_STATUS_ERR_PC;

        fread(buf, 32, sizeof(char), fp);
        fclose(fp);

        if(strstr(pfname, "NOT") != NULL)       //file name contains NOT("Check NOT")
        {
            if(strcmp(buf, pCPUinfo->md5_hash) != 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_WRONG_CPU;
        }
        else
        {
            if(strcmp(buf, pCPUinfo->md5_hash) == 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_ERR_CPU;
        }
    }

    return HWCHK_STATUS_ERR_CPU;
}

HWCHK_STATUS CheckUSB(char *usbinfo_file_path)
{
    struct dirent* ent = NULL;
    DIR *pDir = NULL;
    FILE *fp = NULL;
    char *pfname = NULL;
    PDBLLIST_NODETYPE foundnode = NULL;
    char buf[256]={0};
    char filename[256]={0};

    pfname = get_filename_by_kw(usbinfo_file_path, "usbid");
    if(pfname == NULL)  //usbid not found, no checking on USB
        return HWCHK_STATUS_IGNORE_USB;

    //Scan files with names including "usbid"
    pDir = opendir(usbinfo_file_path);
    while(NULL != (ent=readdir(pDir)))
    {
            if(ent->d_type == 8)	//file, not dir
            {
                //Match the file
                if((pfname=strstr(ent->d_name, "usbid")) != NULL)
                {
                    sprintf(filename, "%s%s", usbinfo_file_path, pfname);
                    fp = fopen(filename, "r");
                    fread(buf, 32, sizeof(char), fp);
                    fclose(fp);

                    //If no USB information, return HWCHK_STATUS_FAILED
                    if(__HWLIST__[HWPRT_USB] == NULL)
                        return HWCHK_STATUS_ERR_USB;

                    if(strstr(pfname, "NOT") != NULL)   //Check NOT
                    {
                        if(dbll_FindNodeByData(get_udi_list_header(), &foundnode, (void*)buf, usb_info_check) != DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_WRONG_USB;
                    }
                    else
                    {
                        if(dbll_FindNodeByData(get_udi_list_header(), &foundnode, (void*)buf, usb_info_check) == DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_ERR_USB;
                    }
                }
            }
    }//while

    return HWCHK_STATUS_SUCCESS;
}

int usb_info_check(void *d1, void *d2)
{
	PUSBINFO_NODE pdata1 = (PUSBINFO_NODE)d1;
	char *pdata2 = (char*)d2;
	
	if(strcmp(pdata1->md5_hash, pdata2) == 0)
		return 0;
	return 1;
}

HWCHK_STATUS CheckHDD(char *hddinfo_file_path)
{
    FILE *fp;
    char* pfname = NULL;
    char filename[256]={0};
    char buf[256]={0};
    PHDD_INFO pHDDinfo = NULL;

    if(__HWLIST__[HWPRT_HDD] != NULL)
    {
        pHDDinfo = (PHDD_INFO)(__HWLIST__[HWPRT_HDD]);	//Get HDD info pointer from hardware info list
        pfname = get_filename_by_kw(hddinfo_file_path, "hddid");
        if(pfname == NULL)  //hddid not found, no checking on HDD
            return HWCHK_STATUS_SUCCESS;

        //Read hddid file
        sprintf(filename, "%s%s", hddinfo_file_path, pfname);
        fp = fopen(filename, "r");
        if(fp == NULL)
            return HWCHK_STATUS_ERR_PC;

        fread(buf, 32, sizeof(char), fp);
        fclose(fp);

        if(strstr(pfname, "NOT") != NULL)       //file name contains NOT("Check NOT")
        {
            if(strcmp(buf, pHDDinfo->md5_hash) != 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_WRONG_HDD;
        }
        else
        {
            if(strcmp(buf, pHDDinfo->md5_hash) == 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_ERR_HDD;
        }
    }

    return HWCHK_STATUS_ERR_HDD;
}

HWCHK_STATUS CheckNIC(char *nicinfo_file_path)
{
    struct dirent* ent = NULL;
    DIR *pDir = NULL;
    FILE *fp = NULL;
    char *pfname = NULL;
    PDBLLIST_NODETYPE foundnode = NULL;
    char buf[256]={0};
    char filename[256]={0};

    pfname = get_filename_by_kw(nicinfo_file_path, "nicid");
        if(pfname == NULL)  //nicid not found, no checking on NIC
            return HWCHK_STATUS_IGNORE_NIC;

    //Scan files with names including "nicid"
    pDir = opendir(nicinfo_file_path);
    while(NULL != (ent=readdir(pDir)))
    {
            if(ent->d_type == 8)	//file, not dir
            {
                //Match the file
                if((pfname=strstr(ent->d_name, "nicid")) != NULL)
                {
                    sprintf(filename, "%s%s", nicinfo_file_path, pfname);
                    fp = fopen(filename, "r");
                    fread(buf, 32, sizeof(char), fp);
                    fclose(fp);

                    //If no NIC information, return HWCHK_STATUS_FAILED
                    if(__HWLIST__[HWPRT_NIC] == NULL)
                        return HWCHK_STATUS_ERR_NIC;
                    
                    if(strstr(pfname, "NOT") != NULL)   //Check NOT
                    {
                        if(dbll_FindNodeByData(get_nic_list_header(), &foundnode, (void*)buf, nic_info_check) != DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_WRONG_NIC;
                    }
                    else
                    {
                        if(dbll_FindNodeByData(get_nic_list_header(), &foundnode, (void*)buf, nic_info_check) == DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_ERR_NIC;
                    }
                }
            }
    }//while

    return HWCHK_STATUS_SUCCESS;
}

int nic_info_check(void *d1, void *d2)
{
	PNIC_INFO pdata1 = (PNIC_INFO)d1;
	char *pdata2 = (char*)d2;
	
	if(strcmp(pdata1->md5_hash, pdata2) == 0)
		return 0;
	return 1;
}

HWCHK_STATUS CheckPC(char *pcinfo_file_path)
{
    FILE *fp;
    char* pfname = NULL;
    char filename[256]={0};
    char buf[256]={0};
    PPC_INFO pPcInfo = NULL;

    if(__HWLIST__[HWPRT_PC] != NULL)
    {
        pPcInfo = (PPC_INFO)__HWLIST__[HWPRT_PC];	//Get PC information from __HWLIST__
        pfname = get_filename_by_kw(pcinfo_file_path, "pcid");
        if(pfname == NULL)  //pcid not found, no checking on PC Info
            return HWCHK_STATUS_SUCCESS;

        //Read pcid file
        sprintf(filename, "%s%s", pcinfo_file_path, pfname);
        fp = fopen(filename, "r");
        if(fp == NULL)
            return HWCHK_STATUS_ERR_PC;

        fread(buf, 32, sizeof(char), fp);
        fclose(fp);

        if(strstr(pfname, "NOT") != NULL)       //file name contains NOT("Check NOT")
        {
            if(strcmp(buf, pPcInfo->md5_hash) != 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_WRONG_PC;
        }
        else
        {
            if(strcmp(buf, pPcInfo->md5_hash) == 0)
                return HWCHK_STATUS_SUCCESS;
            return HWCHK_STATUS_ERR_PC;
        }
    }

    return HWCHK_STATUS_ERR_PC;
}

HWCHK_STATUS CheckBIOS(char *biosinfo_file_path)
{
	FILE *fp;
        char* pfname = NULL;
	char filename[256]={0};
	char buf[256]={0};
	PBIOS_INFO pBIOSInfo = NULL;

        if(__HWLIST__[HWPRT_BIOS] != NULL)
        {
            pBIOSInfo = (PBIOS_INFO)__HWLIST__[HWPRT_BIOS];	//Get BIOS information from __HWLIST__
            pfname = get_filename_by_kw(biosinfo_file_path, "biosid");
            if(pfname == NULL)  //biosid not found, no checking on BIOS
                return HWCHK_STATUS_SUCCESS;

            //Read biosid file
            sprintf(filename, "%s%s", biosinfo_file_path, pfname);
            fp = fopen(filename, "r");
            if(fp == NULL)
                return HWCHK_STATUS_ERR_BIOS;

            fread(buf, 32, sizeof(char), fp);
            fclose(fp);
            
            if(strstr(pfname, "NOT") != NULL)       //file name contains NOT("Check NOT")
            {
                if(strcmp(buf, pBIOSInfo->md5_hash) != 0)
                    return HWCHK_STATUS_SUCCESS;
                return HWCHK_STATUS_WRONG_BIOS;
            }
            else
            {
                if(strcmp(buf, pBIOSInfo->md5_hash) == 0)
                    return HWCHK_STATUS_SUCCESS;
                return HWCHK_STATUS_ERR_BIOS;
            }
        }

        return HWCHK_STATUS_ERR_BIOS;
}

HWCHK_STATUS CheckMB(char *mbinfo_file_path)
{
    FILE *fp;
    char* pfname = NULL;
    char filename[256]={0};
    char buf[256]={0};
    PMOTHER_BOARD_INFO pMBInfo = NULL;

    if(__HWLIST__[HWPRT_MTHRBRD] != NULL)
    {
        pMBInfo = (PMOTHER_BOARD_INFO)__HWLIST__[HWPRT_MTHRBRD];    //Get Mother board information from __HWLIST__

        pfname = get_filename_by_kw(mbinfo_file_path, "mbid");
            if(pfname == NULL)  //mbid not found, no checking on MB
                return HWCHK_STATUS_SUCCESS;

            //Read mbid file
            sprintf(filename, "%s%s", mbinfo_file_path, pfname);
            fp = fopen(filename, "r");
            if(fp == NULL)
                return HWCHK_STATUS_ERR_BIOS;

            fread(buf, 32, sizeof(char), fp);
            fclose(fp);

            if(strstr(pfname, "NOT") != NULL)       //file name contains NOT("Check NOT")
            {
                if(strcmp(buf, pMBInfo->md5_hash) != 0)
                    return HWCHK_STATUS_SUCCESS;
                return HWCHK_STATUS_WRONG_MB;
            }
            else
            {
                if(strcmp(buf, pMBInfo->md5_hash) == 0)
                    return HWCHK_STATUS_SUCCESS;
                return HWCHK_STATUS_ERR_MB;
            }
    }

    return HWCHK_STATUS_ERR_MB;
}

HWCHK_STATUS CheckMem(char *meminfo_file_path)
{
    struct dirent* ent = NULL;
    DIR *pDir = NULL;
    FILE *fp = NULL;
    char *pfname = NULL;
    PDBLLIST_NODETYPE foundnode = NULL;
    char buf[256]={0};
    char filename[256]={0};

    pfname = get_filename_by_kw(meminfo_file_path, "memid");
    if(pfname == NULL)  //memid not found, no checking on MEM
        return HWCHK_STATUS_IGNORE_MEM;

    //Scan files with names including "memid"
    pDir = opendir(meminfo_file_path);
    while(NULL != (ent=readdir(pDir)))
    {
            if(ent->d_type == 8)	//file, not dir
            {
                //Match the file
                if((pfname=strstr(ent->d_name, "memid")) != NULL)
                {
                    sprintf(filename, "%s%s", meminfo_file_path, pfname);
                    fp = fopen(filename, "r");
                    fread(buf, 32, sizeof(char), fp);
                    fclose(fp);

                    //If no MEM information, return HWCHK_STATUS_FAILED
                    if(__HWLIST__[HWPRT_MEM] == NULL)
                        return HWCHK_STATUS_ERR_MEM;

                    if(strstr(pfname, "NOT") != NULL)   //Check NOT
                    {
                        if(dbll_FindNodeByData(___MMDEV_LIST_HEADER___, &foundnode, (void*)buf, mem_info_check) != DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_WRONG_MEM;
                    }
                    else
                    {
                        if(dbll_FindNodeByData(___MMDEV_LIST_HEADER___, &foundnode, (void*)buf, mem_info_check) == DBLL_STATUS_SUCCESS)
                            continue;
                        return HWCHK_STATUS_ERR_MEM;
                    }
                }
            }
    }//while

    return HWCHK_STATUS_SUCCESS;
}

int mem_info_check(void *d1, void *d2)
{
	PMEM_DEVICE_INFO pdata1 = (PMEM_DEVICE_INFO)d1;
	char *pdata2 = (char*)d2;
	
	if(strcmp(pdata1->md5_hash, pdata2) == 0)
		return 0;
	return 1;
}
