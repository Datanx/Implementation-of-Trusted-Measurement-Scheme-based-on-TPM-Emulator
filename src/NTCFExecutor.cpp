/* 
 * File:   NCSFExecutor.cpp
 * Author: jjdengjj
 * 
 * Created on November 24, 2009, 2:58 PM
 */

#include "NTCFExecutor.hpp"

std::map<std::string, void*(*)(SubEvent)> NTCFExecutor::mapFunc;
bool NTCFExecutor::Status = false;

NTCFExecutor::NTCFExecutor() {
}

NTCFExecutor::NTCFExecutor(const NTCFExecutor& orig) {
}

NTCFExecutor::~NTCFExecutor() {
}

void NTCFExecutor::Exec(SubEvent event)
{
    SubEvent e = event;
    if(!NTCFExecutor::Status)
        NTCFExecutor::Init();
    char str_key[9] = {0};
    NTCFPackage pkg = e.m_getEventPKG();
    sprintf(str_key, "%#08d", pkg.get_MajorCode());
    std::string key = std::string(str_key);
    if(NTCFExecutor::mapFunc.find(key) != NTCFExecutor::mapFunc.end())
    {
        Func Proc = NTCFExecutor::mapFunc[key];
        Proc(e);
    }
}

void NTCFExecutor::Init()
{
    char str_keyREQ[9] = {0};
    sprintf(str_keyREQ, "%#08d", _NCSFPC_MJ_CMD_REQ_);
    std::string keyREQ = std::string(str_keyREQ);
    NTCFExecutor::mapFunc[keyREQ] = &NTCFExecutor::FuncREQ;

    char str_keyRTN[9] = {0};
    sprintf(str_keyRTN, "%#08d", _NCSFPC_MJ_COM_CLS_);
    std::string keyRTN = std::string(str_keyRTN);
    NTCFExecutor::mapFunc[keyRTN] = &NTCFExecutor::FuncRTN;

    char str_keyCLS[9] = {0};
    sprintf(str_keyCLS, "%#08d", _NCSFPC_MJ_COM_CLS_);
    std::string keyCLS = std::string(str_keyCLS);
    NTCFExecutor::mapFunc[keyCLS] = &NTCFExecutor::FuncCLS;

    NTCFExecutor::Status = true;
}

void* NTCFExecutor::FuncREQ(SubEvent event)
{
    SubEvent e = event;
    char* IP = NULL;
    NTCFPackage opkg = e.m_getEventPKG();
    opkg.get_DataInfo(&IP);
    std::string log = std::string("NTCF Server Receive Validation Request from IP:\"");
    log += event.m_getEventID();
    log += std::string("\"");
    MainLogger::m_setLog(log);
    HWCollection HW = HWCollection(IP);
    free(IP);

    CMSG rtn;
    NTCFPackage pkg;

    rtn.id = e.m_getEventID();
    pkg.set_MajorCode(_NCSFPC_MJ_CMD_RTN_);
    pkg.set_MinorCode(opkg.get_MinorCode());

    if(HW.Update())
    {
	bool rt = false;
        log = std::string("Agent IP:\"") + HW.getID() + std::string("\" is being Validated");
        MainLogger::m_setLog(log);
        
	/*
	//Get Service Policy
	SQLConnector conn;
	ServerPolicy SP;
	switch(opkg.get_MinorCode())
	{
	case _NCSFPC_MN_TYPE_HTTP_:
		SP = conn.GetServerPolicy(std::string("HTTP"));
		if(SP.getID() == -1)
		{
			SP.setHPOLICY("11111111111");
			SP.setSPOLICY("0");
		}
		break;
	default:
		SP.setHPOLICY("11111111111");
		SP.setSPOLICY("0");
		break;
		
	}

	//Get Client Policy
	ClientPolicy CP;
	CP = conn.GetClientPolicy(std::string(HW.getMB().md5_hash));
	
	if(CP.getID() == -1)
		rt = false;
	
	//Validate PCSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_PC))
	{
		bool pcrt = false;
		if(CP.getPCSN() == std::string(HW.getPC().md5_hash))
			pcrt = true;
		rt = rt && pcrt;
	}

	//Validate MBSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_MB))
	{
		bool mbrt = false;
		if(CP.getMBSN() == std::string(HW.getMB().md5_hash))
			mbrt =  true;
		rt = rt && mbrt;
	}

	//Validate BIOSSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_BIOS))
	{
		bool biosrt = false;
		if(CP.getBIOSSN() == std::string(HW.getBIOS().md5_hash))
			biosrt = true;
		rt = rt && biosrt;
	}

	//Validate CPUSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_CPU))
	{
		bool cpurt = false;
		if(CP.getCPUSN() ==  std::string(HW.getCPU().md5_hash))
			cpurt = true;
		rt = rt && cpurt;
	}

	//Validate HDDSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_HDD))
	{
		bool hddrt = false;
		if(CP.getHDDSN() == std::string(HW.getHDD().md5_hash))
			hddrt = true;
		rt = rt && hddrt;
	}

	//Validate MEMSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_MEM))
	{
		if(SP.getHPOLICY(POLICY_HARDWARE_MEME))
		{
			int count = CP.getMEMSNNum();
			bool memrt = true;
			for(int i = 0; i < count; i++)
			{
				if(!HW.findMEMSN(CP.getMEMSN(i)))
				{
					memrt = false;
					break;
				}
			}
			rt =  rt && memrt;
		}
		else
		{
			int count = CP.getMEMSNNum();
			bool memrt = false;
			for(int i = 0; i < count; i++)
			{
				if(HW.findMEMSN(CP.getMEMSN(i)))
				{
					memrt = true;
					break;
				}
			}
			rt =  rt && memrt;
		}
	}

	//Validate NICSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_NIC))
	{
		if(SP.getHPOLICY(POLICY_HARDWARE_NICE))
		{
			int count = CP.getNICSNNum();
			bool nicrt = true;
			for(int i = 0; i < count; i++)
			{
				if(!HW.findNICSN(CP.getNICSN(i)))
				{
					nicrt = false;
					break;
				}
			}
			rt =  rt && nicrt;
		}
		else
		{
			int count = CP.getNICSNNum();
			bool nicrt = false;
			for(int i = 0; i < count; i++)
			{
				if(HW.findNICSN(CP.getNICSN(i)))
				{
					nicrt = true;
					break;
				}
			}
			rt =  rt && nicrt;
		}
	}

	//Validate USBSN
	if(rt && SP.getHPOLICY(POLICY_HARDWARE_USB))
	{
		if(SP.getHPOLICY(POLICY_HARDWARE_USBE))
		{
			int count = CP.getUSBSNNum();
			bool usbrt = true;
			for(int i = 0; i < count; i++)
			{
				if(!HW.findUSBSN(CP.getUSBSN(i)))
				{
					usbrt = false;
					break;
				}
			}
			rt =  rt && usbrt;
		}
		else
		{
			int count = CP.getUSBSNNum();
			bool usbrt = false;
			for(int i = 0; i < count; i++)
			{
				if(HW.findUSBSN(CP.getUSBSN(i)))
				{
					usbrt = true;
					break;
				}
			}
			rt =  rt && usbrt;
		}
	}*/
	
	switch(opkg.get_MinorCode())
	{
	case _NCSFPC_MN_TYPE_HTTP_:
		t_ntcs_hwpolicy hwp = t_ntcs_hwpolicy(&HW, std::string("http"));
		if(hwp.Validate() == 0)
			rt = true;
		else
			rt = false;
		break;	
	}

	//Format result
	if(rt)
	{	
	        pkg.set_DataInfo( _NCSFPC_RTN_SUCCESS_);
        	log = std::string("Result of Validation to IP:\"") + HW.getID() + std::string("\" is SUCCESS");
        	MainLogger::m_setLog(log);
	}
	else
	{
	        pkg.set_DataInfo( _NCSFPC_RTN_FAILED_);
        	log = std::string("Result of Validation to IP:\"") + HW.getID() + std::string("\" is FAILED");
        	MainLogger::m_setLog(log);
	}

    }
    else
    {
        pkg.set_DataInfo( _NCSFPC_RTN_CLOSE_);
        log = std::string("Result of Validation to IP:\"") + HW.getID() + std::string("\" is FAILED");;
        MainLogger::m_setLog(log);
    }

    char* MSG = NULL;
    pkg.get_Base64Str(&MSG);
    rtn.msg = std::string(MSG);
    free(MSG);

    log = std::string("NTCF Server Send Validation Response to IP:\"");
    log += event.m_getEventID();
    log += std::string("\"");
    MainLogger::m_setLog(log);

    MainClient::m_setSend(rtn);

}

void* NTCFExecutor::FuncRTN(SubEvent event)
{
    return NULL;
}

void* NTCFExecutor::FuncCLS(SubEvent event)
{
    MainClient::m_delSubClient(event.m_getEventID());
}
