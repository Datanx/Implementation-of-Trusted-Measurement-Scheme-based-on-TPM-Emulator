/* 
 * File:   Connector.hpp
 * Author: jjdengjj
 *
 * Created on November 21, 2009, 7:55 PM
 */

#ifndef _CONNECTOR_HPP
#define	_CONNECTOR_HPP

#define _CONNECTOR_BUFLENGTH_ 4096
#define _CONNECTOR_AGENTPORT_ 18887

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <list>
#include <iterator>
#include "CmdProcessor.h"
#include "OsstartPolicyOperator.h"
extern "C"{
    #include "CmdPackage.h"
    #include "cpuinfo.h"
    #include "hdinfo.h"
    #include "dmi_info.h"
    #include "base64.h"
    #include "usbinfo.h"
    #include "SoftInfo.h"
}

class AgentConnector {
public:
    AgentConnector(const char*);
    virtual ~AgentConnector();
private:
    int  m_agentStatus;
    char m_bufRecv[_CONNECTOR_BUFLENGTH_];
    char m_bufSend[_CONNECTOR_BUFLENGTH_];
    char m_strIP[16];
    int  m_agentSocketfd;
    struct sockaddr_in m_agentAddrin;
    CmdProcessor m_cmdProcessor;

public:
    bool m_openAgent();
    bool m_closeAgent();
    
    bool m_getCPUInfo(PCPU_INFO);
    bool m_getMBInfo(PMOTHER_BOARD_INFO);
    bool m_getBIOSInfo(PBIOS_INFO);
    bool m_getMEMInfo(std::list<PMEM_DEVICE_INFO>&);
    static bool m_freeMEMInfo(std::list<PMEM_DEVICE_INFO>&);
    bool m_getHDDInfo(PHDD_INFO);
    bool m_getUSBInfo(std::list<PUSBINFO_NODE>&);
    static bool m_freeUSBInfo(std::list<PUSBINFO_NODE>&);
    bool m_getNICInfo(std::list<PNIC_INFO>&);
    static bool m_freeNICInfo(std::list<PNIC_INFO>&);
    bool m_getPCInfo(PPC_INFO);

    bool m_getSWInfo(PTMainSoftInfo,unsigned int);
    int m_setOSStart(char*); //Set OSStart Policy to Agent
    bool m_setShellCmd(char*);  //Execute shell command at agent

private:
    int m_sendAgent(char*);
    int m_recvAgent(char**);
};

#endif	/* _CONNECTOR_HPP */

