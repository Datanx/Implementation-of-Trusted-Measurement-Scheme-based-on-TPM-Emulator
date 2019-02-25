#ifndef _AGENT_H
#define _AGENT_H

#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>
#include <time.h>
#include <queue>
#include <string>
#include "MulticastAgent.hpp"
#include "MulticastServer.hpp"

#include "errno.hpp"

#ifndef _AGENT_SERVER_PORT
#define _AGENT_SERVER_PORT 18887
#endif

#ifndef _AGENT_SERVER_MAXREQ
#define _AGENT_SERVER_MAXREQ 20
#endif

#ifndef _AGENT_SERVER_STOP
#define _AGENT_SERVER_STOP 0
#endif

#ifndef _AGENT_SERVER_START
#define _AGENT_SERVER_START 1
#endif

#ifndef _AGENT_SERVER_BUSY
#define _AGENT_SERVER_BUSY 2
#endif

#ifndef _AGENT_SERVER_MSGEND
#define _AGENT_SERVER_MSGEND '\0'
#endif

#ifndef _AGENT_SERVER_LOGFILE
#define _AGENT_SERVER_LOGFILE "/var/Agent/Agent.log"
#endif

#ifndef _AGENT_SERVER_LOGPATH
#define _AGENT_SERVER_LOGPATH "/var/Agent"
#endif

#ifndef _AGENT_SERVER_LOGBUF_MAXLENGTH
#define _AGENT_SERVER_LOGBUF_MAXLENGTH 4096
#endif

//Definition of Receive processing function type
typedef void*(*RcvProcFunc)(void *msg);

class Agent
{
public:
	Agent();
	virtual ~Agent();
	
public:
	static void* m_onRcvProc;
private:
	static int m_Status;
	static int m_ErrNo;
	static int m_AgentSocketfd;
    static struct sockaddr_in m_AgentSocketAddr;
	static int m_ClientSocketfd;
    static struct sockaddr_in m_ClientSocketAddr;
    static std::queue<std::string> m_AgentLog;
    static std::string m_logfile;
    
    static pthread_t m_AcceptThread;
    static pthread_t m_ReceiveThread;
    static pthread_mutex_t m_ClientSocketfdSendMutex;
	static pthread_mutex_t m_ClientSocketfdReceiveMutex;
	static pthread_mutex_t m_AgentLogMutex;
	static bool m_logstatus;
    
public:
	static int m_getAgentStatus();
	static int m_getAgentLastErrNo();
	static bool m_startAgent();
	static bool m_stopAgent();
	static bool m_onSend(char*);
	static bool m_onClose();
	static bool m_setLog(std::string);
	static bool m_getLog(std::string&);
	static bool m_saveLog();
	static bool m_getStrIPPORT(struct sockaddr_in, std::string&);
	
private:
	static void* m_onAccept(void*);
	static void* m_onReceive(void*);
};

#endif
