/* 
 * File:   CmdProcessor.h
 * Author: root
 *
 * Created on November 15, 2009, 10:14 AM
 */

#ifndef _CMDPROCESSOR_H
#define	_CMDPROCESSOR_H

//#ifdef __cplusplus
extern "C"{
    #include "cpuinfo.h"
    #include "Defs.h"
    #include "base64.h"
    #include "dmi_info.h"
    #include "hdinfo.h"
    #include "nicinfo.h"
    #include "usbinfo.h"
    #include "HWInfo.h"
    #include "CmdPackage.h"
}
//#endif
#include "SoftInfo.h"
#include "ExString.h"
#include "OsstartPolicyOperator.h"

class CmdProcessor {
public:
    CmdProcessor();
    CmdProcessor(const CmdProcessor& orig);
    char* CmdCreator(int CMDMJCODE, int CMDMNCODE);
    char* CmdCreator(int CMDMJCODE, int CMDMNCODE, char *data, int data_len, char *extra_data, int extra_data_len);
    void* CmdParser(char *in_msg);
    void* CmdExecutor(PCMD_PACKAGE pack);
    virtual ~CmdProcessor();
private:
	//REQ cmd creator for communication control
	char* creator_req_commclose();
        //REQ cmd creator for delete specific dir/file at agent
        char* creator_req_exec_shell_cmd(char *data, int data_len, char *extra_data, int extra_data_len);
	//REQ cmd creator for execute osstart policy
	char* creator_req_exec_osstart_policy(char *data, int data_len, char *extra_data, int extra_data_len);
    //REQ cmd creator help functions
    //Create hardware info request package returned as encoded string
    char* creator_req_pcinfo();
    char* creator_req_biosinfo();
    char* creator_req_mbinfo();
    char* creator_req_mminfo();
    char* creator_req_mmnum();
    char* creator_req_cpuinfo();
    char* creator_req_cpumd5hash();
    char* creator_req_hddinfo();
    char* creator_req_hddmd5hash();
    char* creator_req_usbnum();
    char* creator_req_usbinfo();
    char* creator_req_nicnum();
    char* creator_req_nicinfo();
    char* creator_req_kernel();
    char* creator_req_gcc();
    char* creator_req_bash();
    char* creator_req_openoffice();
    char* creator_req_firefox();
    char* creator_req_helper(int majorcode, int minorcode);
    char* creator_req_helper(int majorcode, int minorcode, char *data, int data_len, char *extra_data, int extra_data_len);

    //TRN cmd creator help functions
    //Create response package of osstart policy execution, which return as encoded string
    char* creator_trans_exec_osstart_policy(char* data, int data_len, char* extra_data, int extra_data_len);
    //Create hardware info package returned as encoded string
    char* creator_trans_pcinfo();
    char* creator_trans_biosinfo();
    char* creator_trans_mbinfo();
    char* creator_trans_mminfo();
    char* creator_trans_mmnum();
    char* creator_trans_dmiinfo_helper(int dmi_minor_code);
    char* creator_trans_cpuinfo();
    char* creator_trans_cpumd5hash();
    char* creator_trans_hddinfo();
    char* creator_trans_hddmd5hash();
    char* creator_trans_usbnum();
    char* creator_trans_usbinfo();
    char* creator_trans_nicnum();
    char* creator_trans_nicinfo();
    char* creator_trans_kernel();
    char* creator_trans_gcc();
    char* creator_trans_bash();
    char* creator_trans_openoffice();
    char* creator_trans_firefox();
    char* creator_trans_swinfo_helper(int sw_minor_code);

	//REQ cmd executor help functions for communication control
	void exec_req_commclose(PCMD_PACKAGE pack);
        //REQ cmd executor help functions for deleting specific dir/file at agent
        void exec_req_exec_shell_cmd(PCMD_PACKAGE pack);
	//REQ cmd executor help function for executing osstart policy
	void exec_req_exec_osstart_policy(PCMD_PACKAGE pack);
	//REQ cmd executor help functions
	//Preparing corresponding info and send specific package to server
	void exec_req_pcinfo(PCMD_PACKAGE pack);
	void exec_req_biosinfo(PCMD_PACKAGE pack);
	void exec_req_mbinfo(PCMD_PACKAGE pack);
	void exec_req_mminfo(PCMD_PACKAGE pack);
	void exec_req_mmnum(PCMD_PACKAGE pack);
    void exec_req_cpuinfo(PCMD_PACKAGE pack);
    void exec_req_cpumd5hash(PCMD_PACKAGE pack);
    void exec_req_hddinfo(PCMD_PACKAGE pack);
    void exec_req_hddmd5hash(PCMD_PACKAGE pack);
    void exec_req_usbnum(PCMD_PACKAGE pack);
    void exec_req_usbinfo(PCMD_PACKAGE pack);
    void exec_req_nicnum(PCMD_PACKAGE pack);
    void exec_req_nicinfo(PCMD_PACKAGE pack);
    void exec_req_swinfo(PCMD_PACKAGE pack, int minor_code);
    
    //TRN cmd executor help functions
    //Get the execution status of osstart policy
    void* exec_trans_exec_osstart_policy(PCMD_PACKAGE pack);
    //Parsing the pack into corresponding data struct
    void* exec_trans_pcinfo(PCMD_PACKAGE pack);
    void* exec_trans_biosinfo(PCMD_PACKAGE pack);
    void* exec_trans_mbinfo(PCMD_PACKAGE pack);
    void* exec_trans_mminfo(PCMD_PACKAGE pack);
    void* exec_trans_mmnum(PCMD_PACKAGE pack);
    void* exec_trans_cpuinfo(PCMD_PACKAGE pack);
    void* exec_trans_hddinfo(PCMD_PACKAGE pack);
    void* exec_trans_usbnum(PCMD_PACKAGE pack);
    void* exec_trans_usbinfo(PCMD_PACKAGE pack);
    void* exec_trans_nicnum(PCMD_PACKAGE pack);
    void* exec_trans_nicinfo(PCMD_PACKAGE pack);
    void* exec_trans_swinfo(PCMD_PACKAGE pack);
};
#endif	/* _CMDPROCESSOR_H */

