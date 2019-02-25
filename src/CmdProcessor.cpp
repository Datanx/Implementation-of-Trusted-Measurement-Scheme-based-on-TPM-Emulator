/* 
 * File:   CmdProcessor.cc
 * Author: root
 * 
 * Created on November 15, 2009, 10:14 AM
 */

#include <stdlib.h>

#include "ExString.h"
#include "CmdProcessor.h"
#include "Agent.hpp"

CmdProcessor::CmdProcessor() {
}

CmdProcessor::CmdProcessor(const CmdProcessor& orig) {
}

CmdProcessor::~CmdProcessor() {
}

//Composite CmdPackage according to CMDMJCODE & CMDMNCODE
char* CmdProcessor::CmdCreator(int CMDMJCODE, int CMDMNCODE)
{
    switch(CMDMJCODE)
    {
    	case CMD_MJ_REQ_COMMCLOSE:
    		return this->creator_req_commclose();
    		break;
        case CMD_MJ_REQ_HWINFO:
            switch(CMDMNCODE)
            {
            	case CMD_MN_REQ_PCINFO:
            		return this->creator_req_pcinfo();
            		break;
            	case CMD_MN_REQ_BIOSINFO:
            		return this->creator_req_biosinfo();
            		break;
            	case CMD_MN_REQ_MBINFO:
            		return this->creator_req_mbinfo();
            		break;
            	case CMD_MN_REQ_MEMINFO:
            		return this->creator_req_mminfo();
            		break;
            	case CMD_MN_REQ_MEMNUM:
            		return this->creator_req_mmnum();
            		break;
                case CMD_MN_REQ_CPUINFO:
                    return this->creator_req_cpuinfo();
                    break;
                case CMD_MN_REQ_CPUMD5HASH:
                    return this->creator_req_cpumd5hash();
                    break;
                case CMD_MN_REQ_HDDINFO:
                	return this->creator_req_hddinfo();
                    break;
                case CMD_MN_REQ_HDDMD5HASH:
                	return this->creator_req_hddmd5hash();
                	break;
                case CMD_MN_REQ_USBNUM:
                	return this->creator_req_usbnum();
                	break;
                case CMD_MN_REQ_USBINFO:
                	return this->creator_req_usbinfo();
                	break;
                case CMD_MN_REQ_NICNUM:
                	return this->creator_req_nicnum();
               		break;
               	case CMD_MN_REQ_NICINFO:
                	return this->creator_req_nicinfo();
               		break;
                default:
                	break;
            }
            break;
        case CMD_MJ_REQ_SWINFO:
        	switch(CMDMNCODE)
        	{
        		case CMD_MN_REQ_KERNELINFO:
        			return this->creator_req_kernel();
        			break;
        		case CMD_MN_REQ_GCCINFO:
        			return this->creator_req_gcc();
        			break;
        		case CMD_MN_REQ_BASHINFO:
        			return this->creator_req_bash();
        			break;
        		case CMD_MN_REQ_OPENOFFICEINFO:
        			return this->creator_req_openoffice();
        			break;
        		case CMD_MN_REQ_FIREFOXINFO:
        			return this->creator_req_firefox();
        			break;
        		default:
        			break;
        	}
            break;
        case CMD_MJ_TRN_HWINFO:
            switch(CMDMNCODE)
            {
            	case CMD_MN_TRN_PCINFO:
            		return this->creator_trans_pcinfo();
            		break;
            	case CMD_MN_TRN_BIOSINFO:
            		return this->creator_trans_biosinfo();
            		break;
            	case CMD_MN_TRN_MBINFO:
            		return this->creator_trans_mbinfo();
            		break;
            	case CMD_MN_TRN_MEMINFO:
            		return this->creator_trans_mminfo();
            		break;
            	case CMD_MN_TRN_MEMNUM:
            		return this->creator_trans_mmnum();
            		break;
                case CMD_MN_TRN_CPUINFO:
                    return this->creator_trans_cpuinfo();
                    break;
                case CMD_MN_TRN_CPUMD5HASH:
                    return this->creator_trans_cpumd5hash();
                    break;
                case CMD_MN_TRN_CPUSHA1HASH:
                    break;
                case CMD_MN_TRN_HDDINFO:
                    return this->creator_trans_hddinfo();
                    break;
                case CMD_MN_TRN_HDDMD5HASH:
                    return this->creator_trans_hddmd5hash();
                    break;
                case CMD_MN_TRN_HDDSHA1HASH:
                    break;
                case CMD_MN_TRN_USBNUM:
                    return this->creator_trans_usbnum();
                    break;
                case CMD_MN_TRN_USBINFO:
                    return this->creator_trans_usbinfo();
                    break;
                case CMD_MN_TRN_NICNUM:
                    return this->creator_trans_nicnum();
                    break;
                case CMD_MN_TRN_NICINFO:
                    return this->creator_trans_nicinfo();
                    break;
                default:
                    break;
            }
            break;
        case CMD_MJ_TRN_SWINFO:
            switch(CMDMNCODE)
        	{
        		case CMD_MN_TRN_KERNELINFO:
        			return this->creator_trans_kernel();
        			break;
        		case CMD_MN_TRN_GCCINFO:
        			return this->creator_trans_gcc();
        			break;
        		case CMD_MN_TRN_BASHINFO:
        			return this->creator_trans_bash();
        			break;
        		case CMD_MN_TRN_OPENOFFICEINFO:
        			return this->creator_trans_openoffice();
        			break;
        		case CMD_MN_TRN_FIREFOXINFO:
        			return this->creator_trans_firefox();
        			break;
        		default:
        			break;
        	}
            break;
        default:
            break;
    }
    
    return NULL;
}

char* CmdProcessor::CmdCreator(int CMDMJCODE, int CMDMNCODE, char *data, int data_len, char *extra_data, int extra_data_len)
{
	switch(CMDMJCODE)
    {
        case CMD_MJ_REQ_EXEC_SHELL_CMD:
            return this->creator_req_exec_shell_cmd(data, data_len, extra_data, extra_data_len);
    	case CMD_MJ_REQ_EXEC_OSSTART_POLICY:
            //data stores the policy string
            //extra_data stores the mem & nic & usb device hash value
            return this->creator_req_exec_osstart_policy(data, data_len, extra_data, extra_data_len);
        case CMD_MJ_TRN_EXEC_OSSTART_POLICY:
            return this->creator_trans_exec_osstart_policy(data, data_len, extra_data, extra_data_len);
    	default:
            break;
    }

        return NULL;
}

void* CmdProcessor::CmdParser(char* in_msg)
{
    if(in_msg == NULL)
        return NULL;

    char *decodedmsg = NULL;
    PCMD_PACKAGE oPkg = NULL;
    
    //base64 decode in_msg
    decodedmsg = base64_decode(in_msg);
    oPkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Unserialize(oPkg, decodedmsg);
	free(decodedmsg);

	/*
	switch(oPkg->CMD_MJ_CODE)
	{
		case CMD_MJ_REQ_COMMCLOSE:
		case CMD_MJ_REQ_HWINFO:
		case CMD_MJ_REQ_SWINFO:
		case CMD_MJ_TRN_HWINFO:
		case CMD_MJ_TRN_SWINFO:
			return this->CmdExecutor(oPkg);
			break;
		default:
			break;
	}
	*/
	
	return this->CmdExecutor(oPkg);
	//return NULL;
}

void* CmdProcessor::CmdExecutor(PCMD_PACKAGE pack)
{
    if(pack == NULL)
        return NULL;

    switch(pack->CMD_MJ_CODE)
    {
    	case CMD_MJ_REQ_COMMCLOSE:
    		this->exec_req_commclose(pack);
    		break;
        case CMD_MJ_REQ_EXEC_SHELL_CMD:
            this->exec_req_exec_shell_cmd(pack);
            break;
        case CMD_MJ_REQ_EXEC_OSSTART_POLICY:
            this->exec_req_exec_osstart_policy(pack);
            break;
        case CMD_MJ_TRN_EXEC_OSSTART_POLICY:
            return this->exec_trans_exec_osstart_policy(pack);
        case CMD_MJ_REQ_HWINFO:
            switch(pack->CMD_MN_CODE)
            {
            	case CMD_MN_REQ_PCINFO:
            		this->exec_req_pcinfo(pack);
            		break;
            	case CMD_MN_REQ_BIOSINFO:
            		this->exec_req_biosinfo(pack);
            		break;
            	case CMD_MN_REQ_MBINFO:
            		this->exec_req_mbinfo(pack);
            		break;
            	case CMD_MN_REQ_MEMINFO:
            		this->exec_req_mminfo(pack);
            		break;
            	case CMD_MN_REQ_MEMNUM:
            		this->exec_req_mmnum(pack);
            		break;
                case CMD_MN_REQ_CPUINFO:    //For request CPU cmd, send cpuinfo to server
                    this->exec_req_cpuinfo(pack);
                    break;
                case CMD_MN_REQ_CPUMD5HASH: //For request CPU md5 hash cmd, send cpuinfo md5 hash value to server
                    this->exec_req_cpumd5hash(pack);
                    break;
                case CMD_MN_REQ_HDDINFO:    //For request HDD cmd, send hddinfo to server
                    this->exec_req_hddinfo(pack);
                    break;
                case CMD_MN_REQ_HDDMD5HASH:	//For request HDD md5 hash cmd, send hddinfo md5 hash value to server
                	this->exec_req_hddmd5hash(pack);
                	break;
                case CMD_MN_REQ_USBNUM:
                	this->exec_req_usbnum(pack);
                	break;
               	case CMD_MN_REQ_USBINFO:
               		this->exec_req_usbinfo(pack);
               		break;
               	case CMD_MN_REQ_NICNUM:
               		this->exec_req_nicnum(pack);
               		break;
               	case CMD_MN_REQ_NICINFO:
               		this->exec_req_nicinfo(pack);
               		break;
                default:
                	break;
            }
            break;
        case CMD_MJ_REQ_SWINFO:
        	this->exec_req_swinfo(pack, pack->CMD_MN_CODE);
            break;
        case CMD_MJ_TRN_HWINFO:
        	switch(pack->CMD_MN_CODE)
        	{
        		case CMD_MN_TRN_PCINFO:
        			return this->exec_trans_pcinfo(pack);
        		case CMD_MN_TRN_BIOSINFO:
        			return this->exec_trans_biosinfo(pack);
        		case CMD_MN_TRN_MBINFO:
        			return this->exec_trans_mbinfo(pack);
        		case CMD_MN_TRN_MEMINFO:
        			return this->exec_trans_mminfo(pack);
        		case CMD_MN_TRN_MEMNUM:
        			return this->exec_trans_mmnum(pack);
        		case CMD_MN_TRN_CPUINFO:
        			return this->exec_trans_cpuinfo(pack);
        		case CMD_MN_TRN_CPUMD5HASH:
        		case CMD_MN_TRN_HDDINFO:
        			return this->exec_trans_hddinfo(pack);
        		case CMD_MN_TRN_HDDMD5HASH:
        			break;
        		case CMD_MN_TRN_USBNUM:
        			return this->exec_trans_usbnum(pack);
        		case CMD_MN_TRN_USBINFO:
        			return this->exec_trans_usbinfo(pack);
        		case CMD_MN_TRN_NICNUM:
        			return this->exec_trans_nicnum(pack);
        		case CMD_MN_TRN_NICINFO:
        			return this->exec_trans_nicinfo(pack);
        		default:
        			break;
        	}
            break;
        case CMD_MJ_TRN_SWINFO:
        	return this->exec_trans_swinfo(pack);
            break;
        default:
        	break;
    }

    return NULL;
}

/********************************************************************
*			REQ creator help function								*
********************************************************************/
char* CmdProcessor::creator_req_pcinfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_PCINFO);
}

char* CmdProcessor::creator_req_biosinfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_BIOSINFO);
}

char* CmdProcessor::creator_req_mbinfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MBINFO);
}

char* CmdProcessor::creator_req_mminfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMINFO);
}

char* CmdProcessor::creator_req_mmnum()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMNUM);
}

char* CmdProcessor::creator_req_commclose()
{
	return this->creator_req_helper(CMD_MJ_REQ_COMMCLOSE, CMD_MN_NULL);
}

char* CmdProcessor::creator_req_cpuinfo()
{
    return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_CPUINFO);
}

char* CmdProcessor::creator_req_cpumd5hash()
{
    return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_CPUMD5HASH);
}

char* CmdProcessor::creator_req_hddinfo()
{
    return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_HDDINFO);
}

char* CmdProcessor::creator_req_hddmd5hash()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_HDDMD5HASH);
}

char* CmdProcessor::creator_req_usbnum()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBNUM);
}

char* CmdProcessor::creator_req_usbinfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBINFO);
}

char* CmdProcessor::creator_req_nicnum()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICNUM);
}

char* CmdProcessor::creator_req_nicinfo()
{
	return this->creator_req_helper(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICINFO);
}

char* CmdProcessor::creator_req_kernel()
{
	return this->creator_req_helper(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_KERNELINFO);
}
char* CmdProcessor::creator_req_gcc()
{
	return this->creator_req_helper(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_GCCINFO);
}

char* CmdProcessor::creator_req_bash()
{
	return this->creator_req_helper(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_BASHINFO);
}

char* CmdProcessor::creator_req_openoffice()
{
	return this->creator_req_helper(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_OPENOFFICEINFO);
}

char* CmdProcessor::creator_req_firefox()
{
	return this->creator_req_helper(CMD_MJ_REQ_SWINFO, CMD_MN_REQ_FIREFOXINFO);
}

char* CmdProcessor::creator_req_exec_osstart_policy(char *data, int data_len, char *extra_data, int extra_data_len)
{
	return this->creator_req_helper(
		CMD_MJ_REQ_EXEC_OSSTART_POLICY, 
		CMD_MN_NULL,
		data,
		data_len,
		extra_data,
		extra_data_len
	);
}

char* CmdProcessor::creator_req_exec_shell_cmd(char* data, int data_len, char* extra_data, int extra_data_len)
{
    return this->creator_req_helper(
            CMD_MJ_REQ_EXEC_SHELL_CMD,
            CMD_MN_NULL,
            data,
            data_len,
            extra_data,
            extra_data_len
    );
}

//help to create REQ package
char *CmdProcessor::creator_req_helper(int majorcode, int minorcode)
{
	PCMD_PACKAGE pPKG;
    char *serializedInfo;
    int serializedInfoLen;
    char *b64_encoded;
    
    pPKG = NULL;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    b64_encoded = NULL;
    //Create CmdPackage for requesting hw info
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    strcpy(pPKG->RcgCode, "TPMASCOMM");
    pPKG->CMD_MJ_CODE = majorcode;
    pPKG->CMD_MN_CODE = minorcode;

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);
    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    CmdPack_Free(pPKG);
    
    return b64_encoded;
}

//help to create REQ package(overloaded)
char *CmdProcessor::creator_req_helper(int majorcode, int minorcode, char *data, int data_len, char *extra_data, int extra_data_len)
{
    PCMD_PACKAGE pPKG;
    char *serializedInfo;
    int serializedInfoLen;
    char *b64_encoded;
    
    pPKG = NULL;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    b64_encoded = NULL;
    //Create CmdPackage for requesting hw info
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    strcpy(pPKG->RcgCode, "TPMASCOMM");
    pPKG->CMD_MJ_CODE = majorcode;
    pPKG->CMD_MN_CODE = minorcode;
    
    if(data != NULL && data_len != 0)
    {
    	pPKG->DataBuf = (char*)malloc((data_len+1) * sizeof(char));
    	memset(pPKG->DataBuf, 0, data_len+1);
    	memcpy(pPKG->DataBuf, data, sizeof(char) * data_len);
    	pPKG->DataBufLen = data_len;
    }
    
    if(extra_data != NULL && extra_data_len != 0)
    {
    	pPKG->Extension = (char*)malloc((extra_data_len+1) * sizeof(char));
    	memset(pPKG->Extension, 0, extra_data_len+1);
    	memcpy(pPKG->Extension, extra_data, sizeof(char) * extra_data_len);
    	pPKG->ExtensionLen = extra_data_len;
    }

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);
    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    CmdPack_Free(pPKG);
    
    return b64_encoded;
}
/********************************************************************
*			TRN creator help function								*
********************************************************************/
char* CmdProcessor::creator_trans_exec_osstart_policy(char* data, int data_len, char* extra_data, int extra_data_len)
{
	PCMD_PACKAGE pPKG;
    char *serializedInfo;
    int serializedInfoLen;
    char *dataEncoded;
    int dataEncodedLen;
    char *b64_encoded = NULL;

    //Initialize CmdPackage
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    strcpy(pPKG->RcgCode, "TPMASCOMM");
    pPKG->CMD_MJ_CODE = CMD_MJ_TRN_EXEC_OSSTART_POLICY;
    pPKG->CMD_MN_CODE = CMD_MN_NULL;

    if(data != NULL && data_len != 0)
    {
        pPKG->DataBuf = (char*)malloc((data_len + 1) * sizeof(char));
        memset(pPKG->DataBuf, 0, (data_len + 1));
        memcpy(pPKG->DataBuf, data, data_len * sizeof(char));
        pPKG->DataBufLen = data_len;
    }
    if(extra_data != NULL && extra_data_len != 0)
    {
        pPKG->Extension = (char*)malloc((extra_data_len + 1) * sizeof(char));
        memset(pPKG->Extension, 0, (extra_data_len + 1));
        memcpy(pPKG->Extension, extra_data, extra_data_len * sizeof(char));
        pPKG->ExtensionLen = extra_data_len;
    }

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);

    //Free CmdPackage
    CmdPack_Free(pPKG);

    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    
    return b64_encoded;
}

char* CmdProcessor::creator_trans_pcinfo()
{
	return this->creator_trans_dmiinfo_helper(CMD_MN_TRN_PCINFO);
}

char* CmdProcessor::creator_trans_biosinfo()
{
	return this->creator_trans_dmiinfo_helper(CMD_MN_TRN_BIOSINFO);
}

char* CmdProcessor::creator_trans_mbinfo()
{
	return this->creator_trans_dmiinfo_helper(CMD_MN_TRN_MBINFO);
}

char* CmdProcessor::creator_trans_mminfo()
{
	return NULL;	//this function is not needed
}

char* CmdProcessor::creator_trans_mmnum()
{
	PCMD_PACKAGE pPKG;
	char *serializedInfo;
    int serializedInfoLen;
    int memnum;
    PPC_INFO pPcInfo;
    PBIOS_INFO pBiosInfo;
    PMOTHER_BOARD_INFO pMBInfo;
    
    char *b64_encoded;
    
    serializedInfo = NULL;
    serializedInfoLen = 0;
    memnum = 0;
    pPcInfo = NULL;
    pBiosInfo = NULL;
    pMBInfo = NULL;
    b64_encoded = NULL;
    
    //Fetch DMI info
    PrepareMachineInfo(&pBiosInfo, &pPcInfo, &pMBInfo);

    memnum = ___MMDEV_LIST_HEADER___->DATA_AREA.length;
    //Initialize package
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    pPKG->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pPKG->CMD_MN_CODE = CMD_MN_TRN_MEMNUM;
    pPKG->DataBuf = (int*)malloc(sizeof(int));
    memcpy(pPKG->DataBuf, &memnum, sizeof(int));
    pPKG->DataBufLen = sizeof(int);
    
    //Serialize package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);
    
    //Free CmdPackage
    CmdPack_Free(pPKG);
	b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    DestroyDMIInfo(pBiosInfo, pPcInfo, pMBInfo);
    
    return b64_encoded;
}

char* CmdProcessor::creator_trans_dmiinfo_helper(int dmi_minor_code)
{
	PCMD_PACKAGE pPKG;
	char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//hardware info serialized buffer
    char *serializedInfo;
    int serializedInfoLen;
    char *dataEncoded;
    int dataEncodedLen;
    char *b64_encoded = NULL;
    int MJ_CODE;
    int MN_CODE;
    
    PPC_INFO pPcInfo;
    int pcinfoserializelen;
    PBIOS_INFO pBiosInfo;
    int biosinfoserializelen;
    PMOTHER_BOARD_INFO pMBInfo;
    int mbinfoserializelen;
    
    //Initializing variables
    pPKG = NULL;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    dataEncoded = NULL;
    dataEncodedLen = 0;
    b64_encoded = NULL;
    MJ_CODE = CMD_MJ_NULL;
    MN_CODE = CMD_MN_NULL;
    pPcInfo = NULL;
    pcinfoserializelen = 0;
    pBiosInfo = NULL;
    biosinfoserializelen = 0;
    pMBInfo = NULL;
    mbinfoserializelen = 0;
    
    memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);
    
    //Fetch DMI info
    PrepareMachineInfo(&pBiosInfo, &pPcInfo, &pMBInfo);
    
    switch(dmi_minor_code)
    {
    	case CMD_MN_TRN_PCINFO:
    		//Serialize PC Info
    		SerializePCInfo(pPcInfo, hwSerializeBuffer, &pcinfoserializelen);
    		dataEncoded = base64_encode(hwSerializeBuffer, pcinfoserializelen);
    		MJ_CODE = CMD_MJ_TRN_HWINFO;
    		MN_CODE = CMD_MN_TRN_PCINFO;
    		break;
    	case CMD_MN_TRN_BIOSINFO:
    		SerializeBiosInfo(pBiosInfo, hwSerializeBuffer, &biosinfoserializelen);
    		dataEncoded = base64_encode(hwSerializeBuffer, biosinfoserializelen);
    		MJ_CODE = CMD_MJ_TRN_HWINFO;
    		MN_CODE = CMD_MN_TRN_BIOSINFO;
    		break;
    	case CMD_MN_TRN_MBINFO:
    		SerializeMBInfo(pMBInfo, hwSerializeBuffer, &mbinfoserializelen);
    		dataEncoded = base64_encode(hwSerializeBuffer, mbinfoserializelen);
    		MJ_CODE = CMD_MJ_TRN_HWINFO;
    		MN_CODE = CMD_MN_TRN_MBINFO;
    		break;
    	default:
    		break;
    }
    
    //Initialize CmdPackage
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    strcpy(pPKG->RcgCode, "TPMASCOMM");
    pPKG->CMD_MJ_CODE = MJ_CODE;
    pPKG->CMD_MN_CODE = MN_CODE;
    dataEncodedLen = strlen(dataEncoded);
    pPKG->DataBuf = (char*) malloc((dataEncodedLen + 1) * sizeof (char));
    memset(pPKG->DataBuf, 0, (dataEncodedLen + 1) * sizeof (char));
    memcpy(pPKG->DataBuf, dataEncoded, dataEncodedLen * sizeof (char));
    pPKG->DataBufLen = dataEncodedLen;
    //Serialize Package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);
    
    //Free useless memory
    DestroyDMIInfo(pBiosInfo, pPcInfo, pMBInfo);
    CmdPack_Free(pPKG);
	free(dataEncoded);
	
	//base64 encode package
    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    
    return b64_encoded;
}

char* CmdProcessor::creator_trans_cpuinfo()
{
	PCMD_PACKAGE pPKG;
    char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];
    char *serializedInfo;
    int serializedInfoLen;
    char *dataEncoded;
    int dataEncodedLen;
    char *b64_encoded = NULL;

    //Preparing for CPU info
    int cpuinfoserializelen;
    PCPU_INFO pcpuinfo;

	pPKG = NULL;
    pcpuinfo = NULL;
    cpuinfoserializelen = 0;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    dataEncodedLen = 0;

	pcpuinfo = (PCPU_INFO)malloc(sizeof(CPU_INFO));
    InitCPUStruct(pcpuinfo);
    FillCPUStruct(pcpuinfo);
    //Serialize CPU struct
    memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);

    SerializeCPUStruct(pcpuinfo, hwSerializeBuffer, &cpuinfoserializelen);
    //Encoding hwSerializeBuffer
    dataEncoded = base64_encode(hwSerializeBuffer, cpuinfoserializelen);
    //Free CPU struct
    FreeCPUStruct(pcpuinfo);

    //Initialize CmdPackage
    pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pPKG);
    strcpy(pPKG->RcgCode, "TPMASCOMM");
    pPKG->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pPKG->CMD_MN_CODE = CMD_MN_TRN_CPUINFO;
    dataEncodedLen = strlen(dataEncoded);
    pPKG->DataBuf = (char*) malloc((dataEncodedLen + 1) * sizeof (char));
    memset(pPKG->DataBuf, 0, (dataEncodedLen + 1) * sizeof (char));
    memcpy(pPKG->DataBuf, dataEncoded, dataEncodedLen * sizeof (char));
    pPKG->DataBufLen = dataEncodedLen;

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pPKG);

    //Free CmdPackage
    CmdPack_Free(pPKG);
	free(dataEncoded);

    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
    
    free(serializedInfo);
    
    return b64_encoded;
}

char* CmdProcessor::creator_trans_cpumd5hash()
{
    PCMD_PACKAGE pkg = NULL;
    char *serializedInfo;
    int serializedInfoLen;
    char *b64_encoded = NULL;

    PCPU_INFO pcpuinfo = NULL;
    int cpuMd5HashLen = 0;

    //Preparing for CPU info
    pcpuinfo = NULL;
    serializedInfo = NULL;
    serializedInfoLen = 0;

	pcpuinfo = (PCPU_INFO)malloc(sizeof(CPU_INFO));
    InitCPUStruct(pcpuinfo);
    FillCPUStruct(pcpuinfo);

    //Initialize CmdPackage
    pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pkg->CMD_MN_CODE = CMD_MN_TRN_CPUMD5HASH;
    pkg->DataBuf = (char*) malloc(40 * sizeof (char)); //buffer for md5 hash value
    memset(pkg->DataBuf, 0, 40);
    cpuMd5HashLen = strlen(pcpuinfo->md5_hash);
    memcpy((char*) (pkg->DataBuf), pcpuinfo->md5_hash, cpuMd5HashLen);
    pkg->DataBufLen = cpuMd5HashLen;

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
	b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	
	//Free CPU struct
    FreeCPUStruct(pcpuinfo);
    //Free CmdPackage
    CmdPack_Free(pkg);
	free(serializedInfo);
	
	return b64_encoded;
}

char* CmdProcessor::creator_trans_hddinfo() 
{
    PCMD_PACKAGE pkg = NULL;
    char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//Stores serialized hw info
    char *serializedInfo;
    int serializedInfoLen;
    char *dataEncoded;
    int dataEncodedLen;
    char *b64_encoded;

    int hddinfoserializelen;
    PHDD_INFO phddinfo;

    //Prepare for HDD info
    phddinfo = NULL;
    hddinfoserializelen = 0;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    dataEncodedLen = 0;
    b64_encoded = NULL;

    InitHDDStruct(&phddinfo);
    FillHDDStruct(phddinfo);

    //Serialize HDD struct
    memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);
    SerializeHDDStruct(phddinfo, hwSerializeBuffer, &hddinfoserializelen);
    //Encoding hwSerializeBuffer
    dataEncoded = base64_encode(hwSerializeBuffer, hddinfoserializelen);
    dataEncodedLen = strlen(dataEncoded);
    //Initialize CmdPackage
    pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pkg->CMD_MN_CODE = CMD_MN_TRN_HDDINFO;
    pkg->DataBuf = (char*) malloc((dataEncodedLen + 1) * sizeof (char));
    memset(pkg->DataBuf, 0, dataEncodedLen + 1);
    memcpy((char*) (pkg->DataBuf), dataEncoded, dataEncodedLen * sizeof(char));
    pkg->DataBufLen = dataEncodedLen;
    free(dataEncoded);

	//Serializing package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
    //Free HDD struct
    FreeHDDStruct(phddinfo);
    //Free CmdPackage
    CmdPack_Free(pkg);

	b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	
	free(serializedInfo);
	
	return b64_encoded;
}

char* CmdProcessor::creator_trans_hddmd5hash()
{
    PCMD_PACKAGE pkg = NULL;
    char *serializedInfo;
    int serializedInfoLen;
    int hddMd5HashLen;
    char *b64_encoded;

    PHDD_INFO phddinfo;
    
    //Preparing for HDD md5 hash
    phddinfo = NULL;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    b64_encoded = NULL;
    hddMd5HashLen = 0;

    InitHDDStruct(&phddinfo);
    FillHDDStruct(phddinfo);

    //Initialize CmdPackage
    pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pkg->CMD_MN_CODE = CMD_MN_TRN_HDDMD5HASH;
    pkg->DataBuf = (char*) malloc(40 * sizeof (char)); //buffer for md5 hash value
    memset(pkg->DataBuf, 0, 40);
    hddMd5HashLen = strlen(phddinfo->md5_hash);
    memcpy((char*) (pkg->DataBuf), phddinfo->md5_hash, hddMd5HashLen);
    pkg->DataBufLen = hddMd5HashLen;

    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
    //Free HDD struct
    FreeHDDStruct(phddinfo);
    //Free CmdPackage
    CmdPack_Free(pkg);
    
	b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	
	free(serializedInfo);
	
	return b64_encoded;
}

char* CmdProcessor::creator_trans_usbnum()
{
	PCMD_PACKAGE pkg;
	int usb_dev_num;			//USB device number
    char *serializedInfo;		//serialized package
    int serializedInfoLen;		//serialized package length
    char *b64_encoded;			//base64 encoded package
    
    pkg = NULL;
    usb_dev_num = 0;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    b64_encoded = NULL;
    
    //Initializing USB device list
    InitUDIList();
	CreateUDIList();
	
	//Get USB device number
	usb_dev_num = ___UDI_LIST_HEADER___->DATA_AREA.length;
	
	//Compose Package
	pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
	CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pkg->CMD_MN_CODE = CMD_MN_TRN_USBNUM;
    pkg->DataBuf = (int*)malloc(sizeof(int));
    memcpy(pkg->DataBuf, &usb_dev_num, sizeof(int));
    pkg->DataBufLen = sizeof(int);
    //Serialize package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
    //Free pkg after utilizing it
    CmdPack_Free(pkg);
    
    //Base64 encode
    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	free(serializedInfo);
	
	//Destroy USB device list
	DestroyUDIList();

	return b64_encoded;
}

char* CmdProcessor::creator_trans_usbinfo()
{
	return NULL;		//this function is not needed(see exec_req_usbinfo())
}

char* CmdProcessor::creator_trans_nicnum()
{
	PCMD_PACKAGE pkg;
	int nic_dev_num;			//NIC device number
    char *serializedInfo;		//serialized package
    int serializedInfoLen;		//serialized package length
    char *b64_encoded;			//base64 encoded package
    
    pkg = NULL;
    nic_dev_num = 0;
    serializedInfo = NULL;
    serializedInfoLen = 0;
    b64_encoded = NULL;
    
    //Initializing NIC device list
    InitNICList();
	CreateNICList();
	
	//Get NIC device number
	nic_dev_num = ___NIC_LIST_HEADER___->DATA_AREA.length;
	
	//Compose Package
	pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
	CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
    pkg->CMD_MN_CODE = CMD_MN_TRN_NICNUM;
    pkg->DataBuf = (int*)malloc(sizeof(int));
    memcpy(pkg->DataBuf, &nic_dev_num, sizeof(int));
    pkg->DataBufLen = sizeof(int);
    //Serialize package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
    //Free pkg after utilizing it
    CmdPack_Free(pkg);
    
    //Base64 encode
    b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	
	free(serializedInfo);
	
	//Destroy NIC device list
	DestroyNICList();
	
	return b64_encoded;
}

char* CmdProcessor::creator_trans_nicinfo()
{
	return NULL;	//this function is not needed(see exec_req_nicinfo())
}

char* CmdProcessor::creator_trans_kernel()
{
	return this->creator_trans_swinfo_helper(CMD_MN_TRN_KERNELINFO);
}

char* CmdProcessor::creator_trans_gcc()
{
	return this->creator_trans_swinfo_helper(CMD_MN_TRN_GCCINFO);
}

char* CmdProcessor::creator_trans_bash()
{
	return this->creator_trans_swinfo_helper(CMD_MN_TRN_BASHINFO);
}

char* CmdProcessor::creator_trans_openoffice()
{
	return this->creator_trans_swinfo_helper(CMD_MN_TRN_OPENOFFICEINFO);
}

char* CmdProcessor::creator_trans_firefox()
{
	return this->creator_trans_swinfo_helper(CMD_MN_TRN_FIREFOXINFO);
}

char* CmdProcessor::creator_trans_swinfo_helper(int sw_minor_code)
{
	PCMD_PACKAGE pkg = NULL;
    char swSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//Stores serialized hw info
    int swSerializeBufferLen;
    char *serializedInfo;
    int serializedInfoLen;
    char *dataEncoded;
    int dataEncodedLen;
    char *b64_encoded;
    
	CSoftInfo si;
	switch(sw_minor_code)
	{
		case CMD_MN_TRN_KERNELINFO:
			si.Get_SoftRecord(LINUXVER_BASE);
			break;
		case CMD_MN_TRN_GCCINFO:
			si.Get_SoftRecord(GCCVER_BASE);
			break;
		case CMD_MN_TRN_BASHINFO:
			si.Get_SoftRecord(BASHSHELL_BASE);
			break;
		case CMD_MN_TRN_OPENOFFICEINFO:
			si.Get_SoftRecord(OPENOFFICE_BASE);
			break;
		case CMD_MN_TRN_FIREFOXINFO:
			si.Get_SoftRecord(FIREFOX_BASE);
			break;
		default:
			break;
	}
	
	si.SerializeSoftInfo(&((si.p_softsave)->mainsoftinfo), swSerializeBuffer, &swSerializeBufferLen);
	//Encoding hwSerializeBuffer
    dataEncoded = base64_encode(swSerializeBuffer, swSerializeBufferLen);
    dataEncodedLen = strlen(dataEncoded);
    //Initialize CmdPackage
    pkg = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
    CmdPack_Init(pkg);
    strcpy(pkg->RcgCode, "TPMASCOMM");
    pkg->CMD_MJ_CODE = CMD_MJ_TRN_SWINFO;
    pkg->CMD_MN_CODE = sw_minor_code;
    pkg->DataBuf = (char*) malloc((dataEncodedLen + 1) * sizeof (char));
    memset(pkg->DataBuf, 0, dataEncodedLen + 1);
    memcpy((char*) (pkg->DataBuf), dataEncoded, dataEncodedLen * sizeof(char));
    pkg->DataBufLen = dataEncodedLen;
    free(dataEncoded);
    
    //Serializing package
    serializedInfo = CmdPack_Serialize(&serializedInfoLen, pkg);
    //Free CmdPackage
    CmdPack_Free(pkg);

	b64_encoded = base64_encode(serializedInfo, serializedInfoLen);
	
	free(serializedInfo);
	
	return b64_encoded;
}

/********************************************************************
*			REQ executor help function								*
********************************************************************/
void CmdProcessor::exec_req_commclose(PCMD_PACKAGE pack)
{
	if(pack == NULL)
        return;
    Agent::m_onClose();
    CmdPack_Free(pack);
}

void CmdProcessor::exec_req_exec_shell_cmd(PCMD_PACKAGE pack)
{
    char shell_cmd[4096] = {0};

    if(pack == NULL)
        return;

    memcpy(shell_cmd, (char*)pack->DataBuf, pack->DataBufLen * sizeof(char));
    system(shell_cmd);
    
    CmdPack_Free(pack);
}

void CmdProcessor::exec_req_exec_osstart_policy(PCMD_PACKAGE pack)
{
	char policy_str[1024] = {0};
        char hash_str[4096] = {0};
        OsstartPolicyOperator *pOpo = NULL;
        int opo_exec_status = APP_PLCY_STATUS_SUCCESS;
	
	//Extract policy string from the package
	//======================================
	if(pack == NULL)
		return;

        //extract the policy string
	memcpy(policy_str, (char*)pack->DataBuf, pack->DataBufLen * sizeof(char));
        //extract the hash string
        memcpy(hash_str, (char*)pack->Extension, pack->ExtensionLen * sizeof(char));
	CmdPack_Free(pack);

	//Apply osstart policy
        pOpo = new OsstartPolicyOperator(std::string(policy_str), std::string(hash_str));
        opo_exec_status = pOpo->ApplyPolicy();
        if(opo_exec_status != APP_PLCY_STATUS_SUCCESS)
        {
            printf("Osstart operation execution error\n");
        }
	
	//Send feedback to server
        char* fdbk_msg = NULL;
        char data_buf[1024] = {0};
        int data_buf_len = sizeof(int);

        memcpy(data_buf, &opo_exec_status, sizeof(int));
        fdbk_msg = this->creator_trans_exec_osstart_policy(data_buf, data_buf_len, NULL, 0);

        Agent::m_onSend(fdbk_msg);
        //Free out_msg
	free((void*)fdbk_msg);
        
	return;
}

void CmdProcessor::exec_req_pcinfo(PCMD_PACKAGE pack)
{
	char *out_msg;

    if(pack == NULL)
        return;

    out_msg = NULL;
    //Preparing for transportation of CPU info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_PCINFO);

    //Transport PC info
    Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_biosinfo(PCMD_PACKAGE pack)
{
	char *out_msg;

    if(pack == NULL)
        return;

    out_msg = NULL;
    //Preparing for transportation of CPU info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_BIOSINFO);

    //Transport PC info
    Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_mbinfo(PCMD_PACKAGE pack)
{
	char *out_msg;

    if(pack == NULL)
        return;

    out_msg = NULL;
    //Preparing for transportation of MB info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_MBINFO);

    //Transport PC info
    Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_mminfo(PCMD_PACKAGE pack)
{
	PCMD_PACKAGE pPKG;
	PDBLLIST_NODETYPE mmlist_hdr;
	//PDBLLIST_NODETYPE ptr;
	PMEM_DEVICE_INFO mminfo;
	char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//hardware info serialized buffer
	int serializedHwInfoLen;
	char *hwEncoded;
    int hwEncodedLen;
    char *packSerializeInfo;
    int packSerializeInfoLen;
    char *b64_encoded;
	char *out_msg;
	PPC_INFO pPC;
	PBIOS_INFO pBIOS;
	PMOTHER_BOARD_INFO pMB;
	
	if(pack == NULL)
		return;
	
	pPKG = NULL;
	mmlist_hdr = NULL;
	mminfo = NULL;
	serializedHwInfoLen = 0;
	hwEncoded = NULL;
	hwEncodedLen = 0;
	packSerializeInfo = NULL;
	packSerializeInfoLen = 0;
	b64_encoded= NULL;
	pPC = NULL;
	pBIOS = NULL;
	pMB = NULL;
	
	if(___MMDEV_LIST_PTR___ == NULL)
	{
		//Create Memory list
		PrepareMachineInfo(&pBIOS, &pPC, &pMB);
		//Record BIOS, PC, MotherBoard pointer
		___BIOS_PTR___ = pBIOS;
		___PC_PTR___ = pPC;
		___MB_PTR___ = pMB;
		//Get memory list header
		mmlist_hdr = (PDBLLIST_NODETYPE)___MMDEV_LIST_HEADER___;
		___MMDEV_LIST_PTR___ = mmlist_hdr->next;
	}
	
	if(___MMDEV_LIST_PTR___->nodetype == NTYPE_DATA)
	{
		mminfo = (PMEM_DEVICE_INFO)(___MMDEV_LIST_PTR___->DATA_AREA.data);
		memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);
		SerializeMMInfo(mminfo, hwSerializeBuffer, &serializedHwInfoLen);
		//base64 encode MM info
		hwEncoded = base64_encode(hwSerializeBuffer, serializedHwInfoLen);
		hwEncodedLen = strlen(hwEncoded);
		//Compose package
		pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
		CmdPack_Init(pPKG);
		strcpy(pPKG->RcgCode, "TPMASCOMM");
		pPKG->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
		pPKG->CMD_MN_CODE = CMD_MN_TRN_MEMINFO;
		pPKG->DataBuf = (char*) malloc((hwEncodedLen + 1) * sizeof (char));
		memset(pPKG->DataBuf, 0, (hwEncodedLen + 1) * sizeof (char));
    	memcpy(pPKG->DataBuf, hwEncoded, hwEncodedLen * sizeof (char));
    	pPKG->DataBufLen = hwEncodedLen;
    		
    	packSerializeInfo = CmdPack_Serialize(&packSerializeInfoLen, pPKG);
    		
    	//Free memory
    	CmdPack_Free(pPKG);
    	free(hwEncoded);
    		
    	b64_encoded = base64_encode(packSerializeInfo, packSerializeInfoLen);
    	free(packSerializeInfo);
    		
    	//Transport Memory info
    	Agent::m_onSend(b64_encoded);
    	usleep(50 * 1000);
    		
    	free(b64_encoded);
    		
    	//bzero variables
    	pPKG = NULL;
		serializedHwInfoLen = 0;
		hwEncoded = NULL;
		hwEncodedLen = 0;
		packSerializeInfo = NULL;
		packSerializeInfoLen = 0;
		
		___MMDEV_LIST_PTR___ = ___MMDEV_LIST_PTR___->next;
	}//if
	
	//If ___MMDEV_LIST_PTR___->next is tail node, release resource
	if(___MMDEV_LIST_PTR___->nodetype != NTYPE_DATA)
	{
		//Free Memory device list
		DestroyDMIInfo(___BIOS_PTR___, ___PC_PTR___, ___MB_PTR___);
		___MMDEV_LIST_PTR___ = NULL;
		usleep(50 * 1000);
	}
	
	CmdPack_Free(pack);
}

void CmdProcessor::exec_req_mmnum(PCMD_PACKAGE pack)
{
	char *out_msg;

    if(pack == NULL)
        return;

    out_msg = NULL;
    
    //Preparing for transportation of memory number
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_MEMNUM);

    //Transport MM num 
    Agent::m_onSend(out_msg);
    
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_cpuinfo(PCMD_PACKAGE pack)
{
	char *out_msg;

    if(pack == NULL)
        return;

    out_msg = NULL;
    //Preparing for transportation of CPU info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_CPUINFO);

    //Transport CPU info
    Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_cpumd5hash(PCMD_PACKAGE pack)
{
    if(pack == NULL)
        return;

    char *out_msg = NULL;
    //Preparing for transportation of CPU md5 hash
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_CPUMD5HASH);

    //Transport CPU md5 hash

    //Free out_msg
	CmdPack_Free(pack);
	free(out_msg);

	return;
}

void CmdProcessor:: exec_req_hddinfo(PCMD_PACKAGE pack)
{
    if(pack == NULL)
        return;

    char *out_msg = NULL;
    //Preparing for transportation of HDD info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_HDDINFO);

    //Transport HDD info
	Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

void CmdProcessor::exec_req_hddmd5hash(PCMD_PACKAGE pack)
{
    if(pack == NULL)
        return;

    char *out_msg = NULL;
    //Preparing for transportation of HDD info
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_HDDMD5HASH);

    //Transport HDD md5 hash

    //Free out_msg
	CmdPack_Free(pack);
	free(out_msg);

	return;
}

void CmdProcessor::exec_req_usbnum(PCMD_PACKAGE pack)
{
	if(pack == NULL)
        return;

    char *out_msg = NULL;
    //Preparing for transportation of USB number
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_USBNUM);

    //Transport USB number
    Agent::m_onSend(out_msg);

    //Free out_msg
	CmdPack_Free(pack);
	free(out_msg);

	return;
}

void CmdProcessor::exec_req_usbinfo(PCMD_PACKAGE pack)
{
	PCMD_PACKAGE pPKG;
	PDBLLIST_NODETYPE usblist_hdr;
	//PDBLLIST_NODETYPE ptr;
	PUSBINFO_NODE usbinfo;
	char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//hardware info serialized buffer
	int serializedHwInfoLen;
	char *hwEncoded;
    int hwEncodedLen;
    char *packSerializeInfo;
    int packSerializeInfoLen;
    char *b64_encoded;
	char *out_msg;
	
	if(pack == NULL)
		return;
	
	pPKG = NULL;
	usblist_hdr = NULL;
	usbinfo = NULL;
	serializedHwInfoLen = 0;
	hwEncoded = NULL;
	hwEncodedLen = 0;
	packSerializeInfo = NULL;
	packSerializeInfoLen = 0;
	b64_encoded= NULL;
	
	if(___UDI_PTR___ == NULL)
	{
		//Create USB device list
		InitUDIList();
		CreateUDIList();
		//Get USB device list header
		usblist_hdr = (PDBLLIST_NODETYPE)___UDI_LIST_HEADER___;
		___UDI_PTR___ = usblist_hdr->next;
	}
	
	if(___UDI_PTR___->nodetype == NTYPE_DATA)
	{
		usbinfo = (PUSBINFO_NODE)(___UDI_PTR___->DATA_AREA.data);
		memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);
		SerializeUDIInfo(usbinfo, hwSerializeBuffer, &serializedHwInfoLen);
		//base64 encode USB info
		hwEncoded = base64_encode(hwSerializeBuffer, serializedHwInfoLen);
		hwEncodedLen = strlen(hwEncoded);
		//Compose package
		pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
		CmdPack_Init(pPKG);
		strcpy(pPKG->RcgCode, "TPMASCOMM");
		pPKG->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
		pPKG->CMD_MN_CODE = CMD_MN_TRN_USBINFO;
		pPKG->DataBuf = (char*) malloc((hwEncodedLen + 1) * sizeof (char));
		memset(pPKG->DataBuf, 0, (hwEncodedLen + 1) * sizeof (char));
    	memcpy(pPKG->DataBuf, hwEncoded, hwEncodedLen * sizeof (char));
    	pPKG->DataBufLen = hwEncodedLen;
    		
    	packSerializeInfo = CmdPack_Serialize(&packSerializeInfoLen, pPKG);
    		
    	//Free memory
    	CmdPack_Free(pPKG);
    	free(hwEncoded);
    		
    	b64_encoded = base64_encode(packSerializeInfo, packSerializeInfoLen);
    	free(packSerializeInfo);
    		
    	//Transport Memory info
    	Agent::m_onSend(b64_encoded);
    	usleep(50 * 1000);
    		
    	free(b64_encoded);
    		
    	//bzero variables
    	pPKG = NULL;
		serializedHwInfoLen = 0;
		hwEncoded = NULL;
		hwEncodedLen = 0;
		packSerializeInfo = NULL;
		packSerializeInfoLen = 0;
		
		___UDI_PTR___ = ___UDI_PTR___->next;
	}
	
	//If ___UDI_PTR___->next is tail node, release resource
	if(___UDI_PTR___->nodetype != NTYPE_DATA)
	{
		//Free USB device list
		DestroyUDIList();
		___UDI_PTR___ = NULL;
		usleep(50 * 1000);
	}
	
	CmdPack_Free(pack);
}

void CmdProcessor::exec_req_nicnum(PCMD_PACKAGE pack)
{
	if(pack == NULL)
        return;

    char *out_msg = NULL;
    //Preparing for transportation of NIC number
    out_msg = this->CmdCreator(CMD_MJ_TRN_HWINFO, CMD_MN_TRN_NICNUM);

    //Transport NIC number
    Agent::m_onSend(out_msg);

    //Free out_msg
	CmdPack_Free(pack);
	free(out_msg);

	return;
}

void CmdProcessor::exec_req_nicinfo(PCMD_PACKAGE pack)
{
	PCMD_PACKAGE pPKG;
	PDBLLIST_NODETYPE niclist_hdr;
	//PDBLLIST_NODETYPE ptr;
	PNIC_INFO nicinfo;
	char hwSerializeBuffer[HW_STRUCT_SERIALIZE_SIZE];	//hardware info serialized buffer
	int serializedHwInfoLen;
	char *hwEncoded;
    int hwEncodedLen;
    char *packSerializeInfo;
    int packSerializeInfoLen;
    char *b64_encoded;
	char *out_msg;
	
	if(pack == NULL)
		return;
	
	pPKG = NULL;
	niclist_hdr = NULL;
	nicinfo = NULL;
	serializedHwInfoLen = 0;
	hwEncoded = NULL;
	hwEncodedLen = 0;
	packSerializeInfo = NULL;
	packSerializeInfoLen = 0;
	b64_encoded= NULL;
	
	if(___NIC_LIST_PTR___ == NULL)
	{
		//Create NIC device list
		InitNICList();
		CreateNICList();
		//Get NIC device list header
		niclist_hdr = (PDBLLIST_NODETYPE)___NIC_LIST_HEADER___;
		___NIC_LIST_PTR___ = niclist_hdr->next;
	}
	
	if(___NIC_LIST_PTR___->nodetype == NTYPE_DATA)
	{
		nicinfo = (PNIC_INFO)(___NIC_LIST_PTR___->DATA_AREA.data);
		memset(hwSerializeBuffer, 0, HW_STRUCT_SERIALIZE_SIZE);
		SerializeNICInfo(nicinfo, hwSerializeBuffer, &serializedHwInfoLen);
		//base64 encode NIC info
		hwEncoded = base64_encode(hwSerializeBuffer, serializedHwInfoLen);
		hwEncodedLen = strlen(hwEncoded);
		//Compose package
		pPKG = (PCMD_PACKAGE)malloc(sizeof(CMD_PACKAGE));
		CmdPack_Init(pPKG);
		strcpy(pPKG->RcgCode, "TPMASCOMM");
		pPKG->CMD_MJ_CODE = CMD_MJ_TRN_HWINFO;
		pPKG->CMD_MN_CODE = CMD_MN_TRN_NICINFO;
		pPKG->DataBuf = (char*) malloc((hwEncodedLen + 1) * sizeof (char));
		memset(pPKG->DataBuf, 0, (hwEncodedLen + 1) * sizeof (char));
    	memcpy(pPKG->DataBuf, hwEncoded, hwEncodedLen * sizeof (char));
    	pPKG->DataBufLen = hwEncodedLen;
    		
    	packSerializeInfo = CmdPack_Serialize(&packSerializeInfoLen, pPKG);
    		
    	//Free memory
    	CmdPack_Free(pPKG);
    	free(hwEncoded);
    		
    	b64_encoded = base64_encode(packSerializeInfo, packSerializeInfoLen);
    	free(packSerializeInfo);
    		
    	//Transport Memory info
    	Agent::m_onSend(b64_encoded);
    	usleep(50 * 1000);
    		
    	free(b64_encoded);
    		
    	//bzero variables
    	pPKG = NULL;
		serializedHwInfoLen = 0;
		hwEncoded = NULL;
		hwEncodedLen = 0;
		packSerializeInfo = NULL;
		packSerializeInfoLen = 0;
		
		___NIC_LIST_PTR___ = ___NIC_LIST_PTR___->next;
	}
	
	//If ___NIC_LIST_PTR___->next is tail node, release resource
	if(___NIC_LIST_PTR___->nodetype != NTYPE_DATA)
	{
		//Free NIC device list
		DestroyNICList();
		___NIC_LIST_PTR___ = NULL;
		
		usleep(50 * 1000);
	}
	
	CmdPack_Free(pack);
}

void CmdProcessor::exec_req_swinfo(PCMD_PACKAGE pack, int minor_code)
{
	if(pack == NULL)
        return;

    char *out_msg = NULL;
    int trans_minor_code = 0;
    //Preparing for transportation of Software info
    switch(minor_code)
    {
    	case CMD_MN_REQ_KERNELINFO:
    		trans_minor_code = CMD_MN_TRN_KERNELINFO;
    		break;
    	case CMD_MN_REQ_GCCINFO:
    		trans_minor_code = CMD_MN_TRN_GCCINFO;
    		break;
    	case CMD_MN_REQ_BASHINFO:
    		trans_minor_code = CMD_MN_TRN_BASHINFO;
    		break;
    	case CMD_MN_REQ_OPENOFFICEINFO:
    		trans_minor_code = CMD_MN_TRN_OPENOFFICEINFO;
    		break;
    	case CMD_MN_REQ_FIREFOXINFO:
    		trans_minor_code = CMD_MN_TRN_FIREFOXINFO;
    		break;
    	default:
    		break;
    }
    out_msg = this->CmdCreator(CMD_MJ_TRN_SWINFO, trans_minor_code);

    //Transport Software info
	Agent::m_onSend(out_msg);
    //Free out_msg
	CmdPack_Free(pack);
	free((void*)out_msg);

	return;
}

/********************************************************************
*			TRN executor help function								*
********************************************************************/
void* CmdProcessor::exec_trans_exec_osstart_policy(PCMD_PACKAGE pack)
{
    char* exec_status = NULL;
    //Get the status of osstart policy execution
    //==========================================
    if(pack == NULL)
        return NULL;
    
    exec_status = (char*)malloc(128 * sizeof(char));
    memset(exec_status, 0, 128 * sizeof(char));
    memcpy(exec_status, (char*)pack->DataBuf, pack->DataBufLen * sizeof(char));

    //Free package
    CmdPack_Free(pack);

    return (void*)exec_status;
}

void* CmdProcessor::exec_trans_pcinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to PC info structure and return it
	PPC_INFO pcinfo = NULL;
	char *decodedPCStr = NULL;
	
	decodedPCStr=base64_decode((char*)pack->DataBuf);
	
	InitPCInfo(&pcinfo);
	UnserializePCInfo(&pcinfo, decodedPCStr);
	
	free(decodedPCStr);
	CmdPack_Free(pack);
	return (void*)pcinfo;
}

void* CmdProcessor::exec_trans_biosinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to BIOS info structure and return it
	PBIOS_INFO biosinfo = NULL;
	char *decodedBIOSStr = NULL;
	
	decodedBIOSStr=base64_decode((char*)pack->DataBuf);
	
	InitBiosInfo(&biosinfo);
	UnserializeBiosInfo(&biosinfo, decodedBIOSStr);
	
	free(decodedBIOSStr);
	CmdPack_Free(pack);
	return (void*)biosinfo;
}

void* CmdProcessor::exec_trans_mbinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to Mother Board info structure and return it
	PMOTHER_BOARD_INFO mbinfo = NULL;
	char *decodedMBStr = NULL;
	
	decodedMBStr=base64_decode((char*)pack->DataBuf);
	
	InitMotherBoardInfo(&mbinfo);
	UnserializeMBInfo(&mbinfo, decodedMBStr);
	
	free(decodedMBStr);
	CmdPack_Free(pack);
	return (void*)mbinfo;
}

void* CmdProcessor::exec_trans_mminfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to Memory info structure and return it
	PMEM_DEVICE_INFO mminfo = NULL;
	char *decodedMMStr = NULL;
	
	decodedMMStr=base64_decode((char*)pack->DataBuf);
	
	InitMemDevInfo(&mminfo);
	UnserializeMMInfo(&mminfo, decodedMMStr);
	
	free(decodedMMStr);
	CmdPack_Free(pack);
	return (void*)mminfo;
}

void* CmdProcessor::exec_trans_mmnum(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to memory number and return it
	int* memnum = NULL;
	memnum = (int*)malloc(sizeof(int));
	
	memcpy(memnum, (int*)pack->DataBuf, sizeof(int));
	
	CmdPack_Free(pack);
	return (void*)memnum;
}

void* CmdProcessor::exec_trans_cpuinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to CPU structure and return it
	PCPU_INFO cpuinfo = NULL;
	char *decodedCPUStr = NULL;
	
	cpuinfo = (PCPU_INFO)malloc(sizeof(CPU_INFO));
	
	decodedCPUStr=base64_decode((char*)pack->DataBuf);
	
	InitCPUStruct(cpuinfo);
	UnserializeCPUStruct(&cpuinfo, decodedCPUStr);
	
	free(decodedCPUStr);
	CmdPack_Free(pack);
	return (void*)cpuinfo;
}

void* CmdProcessor::exec_trans_hddinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to HDD structure and return it
	PHDD_INFO hddinfo = NULL;
	char *decodedHDDStr = NULL;
	
	decodedHDDStr=base64_decode((char*)pack->DataBuf);
	
	InitHDDStruct(&hddinfo);
	UnserializeHDDStruct(&hddinfo, decodedHDDStr);
	
	free(decodedHDDStr);
	CmdPack_Free(pack);
	return (void*)hddinfo;
}

void* CmdProcessor::exec_trans_usbnum(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to usb number and return it
	int* usbnum = NULL;
	usbnum = (int*)malloc(sizeof(int));
	
	memcpy(usbnum, (int*)pack->DataBuf, sizeof(int));
	
	CmdPack_Free(pack);
	return (void*)usbnum;
}

void* CmdProcessor::exec_trans_usbinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to USB info structure and return it
	PUSBINFO_NODE usbinfo = NULL;
	char *decodedUSBStr = NULL;
	
	decodedUSBStr=base64_decode((char*)pack->DataBuf);
	
	init_udi_node(&usbinfo);
	UnserializeUDIInfo(&usbinfo, decodedUSBStr);
	
	free(decodedUSBStr);
	CmdPack_Free(pack);
	return (void*)usbinfo;
}

void* CmdProcessor::exec_trans_nicnum(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to nic number and return it
	int* nicnum = NULL;
	nicnum = (int*)malloc(sizeof(int));
	
	memcpy(nicnum, (int*)pack->DataBuf, sizeof(int));
	
	CmdPack_Free(pack);
	return (void*)nicnum;
}

void* CmdProcessor::exec_trans_nicinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to NIC info structure and return it
	PNIC_INFO nicinfo = NULL;
	char *decodedNICStr = NULL;
	
	decodedNICStr=base64_decode((char*)pack->DataBuf);
	
	init_nic_node(&nicinfo);
	UnserializeNICInfo(&nicinfo, decodedNICStr);
	
	free(decodedNICStr);
	CmdPack_Free(pack);
	return (void*)nicinfo;
}

void* CmdProcessor::exec_trans_swinfo(PCMD_PACKAGE pack)
{
	if(pack == NULL)
		return NULL;
	
	//Unpack the pack to Kernel structure and return it
	CSoftInfo* psi = new CSoftInfo();
	char *decodedKernelStr = NULL;
	
	decodedKernelStr=base64_decode((char*)pack->DataBuf);
	
	psi->UnserializeSoftStruct(&((psi->p_softsave)->mainsoftinfo), decodedKernelStr);
	
	free(decodedKernelStr);
	CmdPack_Free(pack);
	
	return (void*)psi;
}
