#include "Agent.hpp"

void* Agent::m_onRcvProc = NULL;
int Agent::m_Status = _AGENT_SERVER_STOP;
int Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SUCCESS;
int Agent::m_AgentSocketfd;
struct sockaddr_in Agent::m_AgentSocketAddr;
int Agent::m_ClientSocketfd;
struct sockaddr_in Agent::m_ClientSocketAddr;
std::queue<std::string> Agent::m_AgentLog;
std::string Agent::m_logfile;

pthread_t Agent::m_AcceptThread;
pthread_t Agent::m_ReceiveThread;
pthread_mutex_t Agent::m_ClientSocketfdSendMutex;
pthread_mutex_t Agent::m_ClientSocketfdReceiveMutex;
pthread_mutex_t Agent::m_AgentLogMutex;
bool Agent::m_logstatus = false;

Agent::Agent(){
}

Agent::~Agent(){
}

int Agent::m_getAgentStatus()
{
	return Agent::m_Status;
}

int Agent::m_getAgentLastErrNo()
{
	return Agent::m_ErrNo;
}

bool Agent::m_startAgent()
{
	if(!Agent::m_logstatus)
	{
		pthread_mutex_init(&Agent::m_AgentLogMutex, NULL);
		Agent::m_logstatus = true;
	}
	if(Agent::m_Status != _AGENT_SERVER_STOP)
	{
		Agent::m_setLog(std::string("Agent: Agent Start Failed"));
		return false;
	}
		
	Agent::m_AgentSocketfd = socket(AF_INET,SOCK_STREAM,0);
	if(Agent::m_AgentSocketfd == -1)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETFD;
		return false;
	}
	Agent::m_setLog(std::string("<b><font color=\"green\">Agent: Socket has been Opened</font></b>"));
	
	int flags = fcntl(Agent::m_AgentSocketfd, F_GETFL, 0);
    if(fcntl(Agent::m_AgentSocketfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETFD_NONBLOCK ;
        return false;
    }
    
	Agent::m_AgentSocketAddr.sin_family = AF_INET;
    Agent::m_AgentSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    Agent::m_AgentSocketAddr.sin_port = htons(_AGENT_SERVER_PORT);
    bzero(&(Agent::m_AgentSocketAddr.sin_zero), 8);
    
    if(bind(Agent::m_AgentSocketfd, (struct sockaddr*)&(Agent::m_AgentSocketAddr), sizeof(struct sockaddr)) == -1)
    {
        Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETBIND;
        return false;
    }
    
    char* ip;
    ip = inet_ntoa(Agent::m_AgentSocketAddr.sin_addr);
    int port;
    port = ntohs(Agent::m_AgentSocketAddr.sin_port);   
    char * c_addr;
    c_addr = (char*)malloc(22*sizeof(char));
    bzero(c_addr, 22);
    sprintf(c_addr, "%s:%d", ip, port);
    std::string addr = std::string(c_addr);
    free(c_addr);
    Agent::m_setLog(std::string("<b><font color=\"green\">Agent: IP address and Port has been Binded: </font></b>") + "<b><i><font color=\"black\">"+addr+"</font></i></b>");

    if(listen(Agent::m_AgentSocketfd, _AGENT_SERVER_MAXREQ) == -1)
    {
        Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETLISTEN;
        return false;
    }
    
    if(pthread_create(&(Agent::m_AcceptThread), NULL, Agent::m_onAccept, NULL) !=0)
    {
        Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETACCEPT_THREADCREATE;
        return false;
    }
    Agent::m_setLog(std::string("<b><font color=\"green\">Agent: Accept Thread has been Created: Waiting for Connection</font></b>"));

    Agent::m_Status = _AGENT_SERVER_START;
    Agent::m_setLog(std::string("<b><font color=\"green\">Agent: Network Services has been Started: Running at </font></b>") + "<b><i><font color=\"black\">"+ addr+"</font></i></b>");
    Agent::m_setLog(std::string("<font color=\"black\">=====================================================</font>"));
    
    MulticastAgent::m_Start();
    return true; 
}

bool Agent::m_stopAgent()
{
	if(Agent::m_logstatus)
	{
		pthread_mutex_destroy(&Agent::m_AgentLogMutex);
		Agent::m_logstatus = false;
	}
	if(Agent::m_Status != _AGENT_SERVER_START)
	{
		Agent::m_setLog(std::string("Agent: Agent Stop Failed"));
		return false;
	}
		
	Agent::m_onClose();

	if(pthread_cancel(Agent::m_AcceptThread) != 0)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETACCEPT_THREADCANCEL;
		return false;
	}
	pthread_join(Agent::m_AcceptThread, NULL);
	Agent::m_setLog(std::string("<b><font color=\"red\">") + std::string("Agent: Accept Thread has been Canceled") + std::string("<b></font>"));
	
	if(close(Agent::m_AgentSocketfd) != 0)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETFDCLOSE;
		return false;
	}
	Agent::m_setLog(std::string("<b><font color=\"red\">") + std::string("Agent: Socket has been Closed") + std::string("<b></font>"));
	Agent::m_setLog(std::string("<b><font color=\"red\">") + std::string("Agent: Network Services has been Stoped") + std::string("<b></font>"));
	Agent::m_setLog(std::string("<font color=\"black\">=====================================================</font>"));
	Agent::m_Status = _AGENT_SERVER_STOP;
        MulticastAgent::m_Start();
	return true;
}

void* Agent::m_onAccept(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    while(true)
    {
        int clientfd;
        struct sockaddr_in clientaddr = {0};
        socklen_t size = sizeof(struct sockaddr);
        clientfd = accept(Agent::m_AgentSocketfd, (struct sockaddr*)&clientaddr, &size);
        if(clientfd != -1)
		{
    		std::string straddr;
    		Agent::m_getStrIPPORT(clientaddr, straddr);
		while(true)
		{
			if(Agent::m_Status == _AGENT_SERVER_START)
			{
				Agent::m_ClientSocketfd = clientfd;
				Agent::m_ClientSocketAddr = clientaddr;
				int flags = fcntl(Agent::m_ClientSocketfd, F_GETFL, 0);
    				if(fcntl(Agent::m_ClientSocketfd, F_SETFL, flags | O_NONBLOCK) == -1)
    				{
        				Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETFD_NONBLOCK ;
        				return false;
    				}
				Agent::m_Status = _AGENT_SERVER_BUSY;
				Agent::m_setLog(std::string("Agent: Connection from ") + straddr + std::string(" has been Accepted: Agent is busy now") );
				pthread_mutex_init(&Agent::m_ClientSocketfdSendMutex, NULL);
				pthread_mutex_init(&Agent::m_ClientSocketfdReceiveMutex, NULL);
				if(pthread_create(&(Agent::m_ReceiveThread), NULL, Agent::m_onReceive, Agent::m_onRcvProc) !=0)
    				{
    					pthread_mutex_destroy(&Agent::m_ClientSocketfdSendMutex);
    					pthread_mutex_destroy(&Agent::m_ClientSocketfdReceiveMutex);
        				Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETRECEIVE_THREADCREATE;
    				}
    				Agent::m_setLog(std::string("Agent: Receive Thread has been Created for ") + straddr + std::string(" : Wait for Data Transfer"));
				break;
			}		
			else
			{
				usleep(50*1000);
				Agent::m_setLog(std::string("Agent: Connection from ") + straddr + std::string(" has been Refused: Agent is processing other connection request now"));
				Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETBUSY;
			}
		}
		}
        else
            Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETACCEPT;
        pthread_testcancel();
        usleep(100*1000);
    }
}

void* Agent::m_onReceive(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    char bufSMSG[4096] = {0};//Store MSG buffer
    char bufRMSG[256] = {0};//Receive MSG buffer
    bool isIgnore = false;
    int sizeMSG = 0;
    int sizeRMSG = 0;
    int sizeSCount = 0;
    int sizeRCount = 0;
    while(true)
    {
    	if(pthread_mutex_trylock(&Agent::m_ClientSocketfdReceiveMutex) != EPERM)
    	{
        	sizeRMSG = recv(Agent::m_ClientSocketfd, (void*)bufRMSG, 256, 0);
        	pthread_mutex_unlock(&Agent::m_ClientSocketfdReceiveMutex);
        	for(sizeRCount = 0; sizeRCount <sizeRMSG; sizeRCount++)
        	{
            	//The Length of the MSG is Out of the Boundary
            	if(sizeSCount > 4096)
            	{
                	isIgnore = true;
                	bzero(bufSMSG, 4096);
                	sizeSCount = 0;
            	}
            	if(bufRMSG[sizeRCount] != _AGENT_SERVER_MSGEND) // If we don't count the end of the MSG, then copy it.
            	{
                	if(!isIgnore)
                	{
                    	bufSMSG[sizeSCount] = bufRMSG[sizeRCount];
                    	sizeSCount++;
                	}
            	}
            	else // If we count the end of the MSG, then post it.
            	{
                	sizeMSG = strlen(bufSMSG);
                	char MSG[4096] = {0};
					char buf[64] = {0};
                	memcpy(MSG, bufSMSG, (sizeMSG+1));
                	// POST the MSG
                	bzero(buf, 64);
                	sizeSCount = 0;
                	std::string straddr;
                	Agent::m_getStrIPPORT(Agent::m_ClientSocketAddr, straddr);
					sprintf(buf, "%d", strlen(MSG));
					Agent::m_setLog(std::string("<font color=\"orange\">") + std::string("Agent &lt;-- Server ") + straddr + std::string(" : ") + std::string(buf) +std::string(" bytes received.") + std::string("</font>"));
                	RcvProcFunc procFunc = (RcvProcFunc)arg;
                	pthread_t proct;
                	if(pthread_create(&(proct), NULL, procFunc, (void*)MSG) != 0)
                		Agent::m_setLog(std::string("<b><font color=\"red\">Create Process thread create failed</font></b>"));
                	else
                		pthread_join(proct, NULL);
                	isIgnore = false;
                	bzero(bufSMSG, 4096);
            	}
        	}
        }
        bzero(bufRMSG, 256);
        sizeRCount = 0;
        pthread_testcancel();     
        usleep(100*1000);
    }
}

bool Agent::m_onSend(char* msg)
{
	if(Agent::m_Status != _AGENT_SERVER_BUSY)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKET_NONCLIENT;
		return false;
	}
	if(msg == NULL)
	{
		Agent::m_setLog(std::string("Agent: MSG is NULL "));
		return false;
	}
	int lenM = strlen(msg) + 1;
	int lenS = 0;
	char buf[64]={0};
	bzero(buf, 64);
	std::string straddr;
    Agent::m_getStrIPPORT(Agent::m_ClientSocketAddr, straddr);
	sprintf(buf, "%d", lenM);
	Agent::m_setLog(std::string("<font color=\"#0084d7\">") + std::string("Agent -&gt; Server ") + straddr + std::string(" : ") + std::string(buf) + std::string(" bytes sent.") + std::string("</font>"));
	pthread_mutex_lock(&Agent::m_ClientSocketfdSendMutex);
	lenS = send(Agent::m_ClientSocketfd, msg, lenM, 0);
	usleep(50*1000);
	pthread_mutex_unlock(&Agent::m_ClientSocketfdSendMutex);
	if(lenS != lenM)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETSEND;
		return false;
	}
	return true;
}

bool Agent::m_onClose()
{
	if(Agent::m_Status != _AGENT_SERVER_BUSY)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKET_NONCLIENT;
		return false;
	}
	pthread_mutex_lock(&Agent::m_ClientSocketfdReceiveMutex);
	pthread_mutex_lock(&Agent::m_ClientSocketfdSendMutex);
	if(pthread_cancel(Agent::m_ReceiveThread) != 0)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETRECEIVE_THREADCANCEL;
		return false;
	}
	pthread_join(Agent::m_ReceiveThread, NULL);
	std::string straddr;
    Agent::m_getStrIPPORT(Agent::m_ClientSocketAddr, straddr);
	Agent::m_setLog(std::string("<b><font color=\"black\">") + std::string("Agent: Receive Thread has been Canceled: ") + straddr + std::string("</b></font>"));
	if(close(Agent::m_ClientSocketfd) != 0)
	{
		Agent::m_ErrNo = _ERRNO_AGENT_SERVER_SOCKETFDCLOSE;
		return false;
	}
	Agent::m_setLog(std::string("<b><font color=\"black\">") + std::string("Agent: Client Socket has been Closed: ") + straddr + std::string("</b></font>"));
	Agent::m_setLog(std::string("<font color=\"black\">=====================================================</font>"));
	pthread_mutex_unlock(&Agent::m_ClientSocketfdReceiveMutex);
	pthread_mutex_unlock(&Agent::m_ClientSocketfdSendMutex);
	pthread_mutex_destroy(&Agent::m_ClientSocketfdReceiveMutex);
	pthread_mutex_destroy(&Agent::m_ClientSocketfdSendMutex);
	Agent::m_Status = _AGENT_SERVER_START;
}

bool Agent::m_setLog(std::string log)
{
	pthread_mutex_lock(&Agent::m_AgentLogMutex);
	
	time_t now;
	struct tm *timenow;
	char timestr[255] = {0};
	time(&now);
	timenow = localtime(&now);
	strftime(timestr, 255, "%Z %Y-%m-%d %H:%M:%S -- ", timenow);
	std::string logitem = std::string(timestr) + log;
	
	Agent::m_AgentLog.push(logitem);
	if((Agent::m_logfile.size() + logitem.size()) >= _AGENT_SERVER_LOGBUF_MAXLENGTH)
		Agent::m_saveLog();
	Agent::m_logfile = Agent::m_logfile + logitem + std::string("\r\n");
	
	
	pthread_mutex_unlock(&Agent::m_AgentLogMutex);
	
	return true;
}

bool Agent::m_getLog(std::string &log)
{
	int rt = false;
	
	pthread_mutex_lock(&Agent::m_AgentLogMutex);
	
	if(!Agent::m_AgentLog.empty())
	{
		log = Agent::m_AgentLog.front();
		Agent::m_AgentLog.pop();
		rt = true;
	}
	
	pthread_mutex_unlock(&Agent::m_AgentLogMutex);
	
	return rt;
}

bool Agent::m_saveLog()
{
	DIR* dir = opendir(_AGENT_SERVER_LOGPATH);
	if(dir == NULL)
		mkdir(_AGENT_SERVER_LOGPATH, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	else
		closedir(dir);
				
	int logfd = open(_AGENT_SERVER_LOGFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if(logfd != -1)
	{
		write(logfd, Agent::m_logfile.c_str(), strlen(Agent::m_logfile.c_str()));
		close(logfd);
		Agent::m_logfile.clear();
		return true;
	}
	else
		return false;
}

bool Agent::m_getStrIPPORT(struct sockaddr_in addr,std::string &straddr)
{
	char* ip;
    ip = inet_ntoa(addr.sin_addr);
    int port;
    port = ntohs(addr.sin_port);   
    char * c_addr;
    c_addr = (char*)malloc(22*sizeof(char));
    bzero(c_addr, 22);
    sprintf(c_addr, "%s:%d", ip, port);
    straddr = std::string(c_addr);
    free(c_addr);
    return true;
}
