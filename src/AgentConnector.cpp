/* 
 * File:   Connector.cpp
 * Author: jjdengjj
 * 
 * Created on November 21, 2009, 7:55 PM
 */

#include "AgentConnector.hpp"

AgentConnector::AgentConnector(const char* ip) {
    bzero(m_strIP, 16);
    strcpy(m_strIP, ip);
    m_agentStatus = 0;
}
AgentConnector::~AgentConnector(){
}

bool AgentConnector::m_openAgent()
{
    if(m_agentStatus != 0)
        return false;
    
    m_agentSocketfd = socket(AF_INET, SOCK_STREAM, 0);
    m_agentAddrin.sin_family=AF_INET;
    m_agentAddrin.sin_port=htons(_CONNECTOR_AGENTPORT_);
    inet_aton(m_strIP, &(m_agentAddrin.sin_addr));
    bzero(&(m_agentAddrin.sin_zero),8);
    if(connect(m_agentSocketfd, (struct sockaddr *)&m_agentAddrin, sizeof(struct sockaddr)) !=0 )
        return false;
    else
    {
        m_agentStatus = 1;
        return true;
    }
}

bool AgentConnector::m_closeAgent()
{
    if(m_agentStatus != 1)
        return false;

    char* reqMSG = NULL;
    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_COMMCLOSE, CMD_MN_NULL);
    m_sendAgent(reqMSG);
    free(reqMSG);
   
    if(close(m_agentSocketfd)!=0)
        return false;
    else
        return true;
}

int AgentConnector::m_sendAgent(char* sendMSG)
{
    if(sendMSG == NULL)
        return 0;
    bzero(m_bufSend, _CONNECTOR_BUFLENGTH_);
    memcpy(m_bufSend, sendMSG, strlen(sendMSG)+1);
    return send(m_agentSocketfd, m_bufSend, strlen(sendMSG)+1, 0);
}

int AgentConnector::m_recvAgent(char** recvMSG)
{
    if(recvMSG == NULL)
        return 0;
    int length = 0;
    int rlen = 0;
    bzero(m_bufRecv, _CONNECTOR_BUFLENGTH_);
    do
    {
        rlen = recv(m_agentSocketfd, m_bufRecv+length, _CONNECTOR_BUFLENGTH_, 0);
        length += rlen;
    }while(m_bufRecv[length-1] != '\0');
    *recvMSG = (char*)malloc(length*sizeof(char));
    memcpy(*recvMSG, m_bufRecv, length);
    return length;
}

bool AgentConnector::m_getCPUInfo(PCPU_INFO CPU)
{
   if(m_agentStatus !=1 )
       return false;

   char* reqMSG = NULL;
   char* rtnMSG = NULL;
   PCPU_INFO pCPU = NULL;

   reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_CPUINFO);
   m_sendAgent(reqMSG);
   free(reqMSG);
   usleep(50*1000);

   m_recvAgent(&rtnMSG);
   pCPU = (PCPU_INFO)m_cmdProcessor.CmdParser(rtnMSG);
   free(rtnMSG);
   memcpy(CPU, pCPU, sizeof(CPU_INFO));
   FreeCPUStruct(pCPU);
   
   return true;
}

bool AgentConnector::m_getMBInfo(PMOTHER_BOARD_INFO MB)
{
    if(m_agentStatus !=1 )
        return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;
    PMOTHER_BOARD_INFO pMB = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MBINFO);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    pMB = (PMOTHER_BOARD_INFO)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);
    memcpy(MB, pMB, sizeof(MOTHER_BOARD_INFO));
    FreeMotherBoardInfo(pMB);

    return true;
}

bool AgentConnector::m_getHDDInfo(PHDD_INFO HDD)
{
    if(m_agentStatus !=1 )
       return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;
    PHDD_INFO pHDD = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_HDDINFO);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    pHDD = (PHDD_INFO)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);
    memcpy(HDD, pHDD, sizeof(HDD_INFO));
    FreeHDDStruct(pHDD);

   return true;
}

bool AgentConnector::m_getBIOSInfo(PBIOS_INFO BIOS)
{
    if(m_agentStatus !=1 )
        return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;
    PBIOS_INFO pBIOS = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_BIOSINFO);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    pBIOS = (PBIOS_INFO)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);
    memcpy(BIOS, pBIOS, sizeof(BIOS_INFO));
    FreeBiosInfo(pBIOS);

    return true;
}

bool AgentConnector::m_getPCInfo(PPC_INFO PC)
{
    if(m_agentStatus !=1 )
       return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;
    PPC_INFO pPC = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_PCINFO);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    pPC = (PPC_INFO)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);
    memcpy(PC, pPC, sizeof(PC_INFO));
    FreePCInfo(pPC);

    return true;
}

bool AgentConnector::m_getMEMInfo(std::list<PMEM_DEVICE_INFO>& lm)
{
    if(m_agentStatus !=1 )
       return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMNUM);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    int *num = NULL;
    num = (int*)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);

    for(int i = 0; i < *num; i++)
    {
        PMEM_DEVICE_INFO tmp = NULL;
        reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_MEMINFO);
        m_sendAgent(reqMSG);
        free(reqMSG);
        usleep(50*1000);
        m_recvAgent(&rtnMSG);
        tmp = (PMEM_DEVICE_INFO)m_cmdProcessor.CmdParser(rtnMSG);
        free(rtnMSG);
        lm.push_back(tmp);
    }

    free(num);
    
    return true;
}

bool AgentConnector::m_getUSBInfo(std::list<PUSBINFO_NODE>& lu)
{
    if(m_agentStatus !=1 )
       return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBNUM);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    int *num = NULL;
    num = (int*)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);

    for(int i = 0; i < *num; i++)
    {
        PUSBINFO_NODE tmp = NULL;
        reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_USBINFO);
        m_sendAgent(reqMSG);
        free(reqMSG);
        usleep(50*1000);
        m_recvAgent(&rtnMSG);
        tmp = (PUSBINFO_NODE)m_cmdProcessor.CmdParser(rtnMSG);
        free(rtnMSG);
        lu.push_back(tmp);
    }

    free(num);

    return true;
}

bool AgentConnector::m_getNICInfo(std::list<PNIC_INFO>& ln)
{
    if(m_agentStatus !=1 )
       return false;

    char* reqMSG = NULL;
    char* rtnMSG = NULL;

    reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICNUM);
    m_sendAgent(reqMSG);
    free(reqMSG);
    usleep(50*1000);

    m_recvAgent(&rtnMSG);
    int *num = NULL;
    num = (int*)m_cmdProcessor.CmdParser(rtnMSG);
    free(rtnMSG);

    for(int i = 0; i < *num; i++)
    {
        PNIC_INFO tmp = NULL;
        reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_HWINFO, CMD_MN_REQ_NICINFO);
        m_sendAgent(reqMSG);
        free(reqMSG);
        usleep(50*1000);
        m_recvAgent(&rtnMSG);
        tmp = (PNIC_INFO)m_cmdProcessor.CmdParser(rtnMSG);
        free(rtnMSG);
        ln.push_back(tmp);
    }

    free(num);

    return true;
}

bool AgentConnector::m_getSWInfo(PTMainSoftInfo swi,unsigned int minor_code)
{   
   if(m_agentStatus !=1 )
       return false;

   char* reqMSG = NULL;
   char* rtnMSG = NULL;

   CSoftInfo *psi = NULL;
   reqMSG = m_cmdProcessor.CmdCreator(CMD_MJ_REQ_SWINFO, minor_code);
   m_sendAgent(reqMSG);
   free(reqMSG);
   usleep(50*1000);

   m_recvAgent(&rtnMSG);
   psi = (CSoftInfo *)m_cmdProcessor.CmdParser(rtnMSG);
   free(rtnMSG);
   strcpy(swi->md5_hash,psi->p_softsave->mainsoftinfo.md5_hash);
   strcpy(swi->softname,psi->p_softsave->mainsoftinfo.softname);
   strcpy(swi->version,psi->p_softsave->mainsoftinfo.version);
   
   delete psi;
   
   return true;
}

int AgentConnector::m_setOSStart(char* pPolicyStr)
{
   if(m_agentStatus !=1 || pPolicyStr == NULL)
       return false;

   char* reqMSG = NULL;
   char* rtnMSG = NULL;
   int* osstart_exec_status = NULL;
   int ret = APP_PLCY_STATUS_SUCCESS;

   //send the osstart request msg to agent
   reqMSG = (char*)malloc(sizeof(char)*(strlen(pPolicyStr)+1));
   memset(reqMSG, 0, sizeof(char)*(strlen(pPolicyStr)+1));
   memcpy(reqMSG, pPolicyStr, sizeof(char)*strlen(pPolicyStr));
   m_sendAgent(reqMSG);
   free(reqMSG);
   usleep(50*1000);

   m_recvAgent(&rtnMSG);

   osstart_exec_status = (int*)m_cmdProcessor.CmdParser(rtnMSG);
   ret = *osstart_exec_status;
   
   //printf("OSSTART EXEC STATUS: %d\n", *osstart_exec_status);

   free(osstart_exec_status);
   free(rtnMSG);

   return ret;
}

bool AgentConnector::m_setShellCmd(char* msg)
{
    if(m_agentStatus !=1 || msg == NULL)
       return false;

    char* reqMSG = NULL;
    reqMSG = m_cmdProcessor.CmdCreator(
                    CMD_MJ_REQ_EXEC_SHELL_CMD,
                    CMD_MN_NULL,
                    msg,
                    strlen(msg),
                    NULL,
                    0
    );

    m_sendAgent(reqMSG);
    
    free(reqMSG);
}
