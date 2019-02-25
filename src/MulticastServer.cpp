/* 
 * File:   MulticastServer.cpp
 * Author: jjdengjj
 * 
 * Created on December 2, 2009, 10:37 AM
 */

#include "MulticastServer.hpp"

int MulticastServer::m_sockfd;
struct sockaddr_in MulticastServer::m_addrinServer;
struct sockaddr_in MulticastServer::m_addrinMulticast;
struct ip_mreq MulticastServer::m_mreq;
pthread_t MulticastServer::m_threadTimer;
pthread_t MulticastServer::m_threadSend;
pthread_t MulticastServer::m_threadRecv;
std::map<std::string, TMC> MulticastServer::m_mapOnline;
pthread_mutex_t MulticastServer::m_mapOnlineMutex;
bool MulticastServer::m_Status = false;
struct timeval MulticastServer::m_timevalSend;
std::string MulticastServer::m_timeStamp;
pthread_mutex_t MulticastServer::m_timeStampMutex;

MulticastServer::MulticastServer() {
}

MulticastServer::MulticastServer(const MulticastServer& orig) {
}

MulticastServer::~MulticastServer() {
}

bool MulticastServer::m_Start() {
    if (MulticastServer::m_Status)
        return false;

    memset(&MulticastServer::m_addrinServer, 0, sizeof (struct sockaddr_in));
    MulticastServer::m_addrinServer.sin_family = AF_INET;
    MulticastServer::m_addrinServer.sin_port = htons(_MULTICAST_PORT);
    MulticastServer::m_addrinServer.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&MulticastServer::m_addrinMulticast, 0, sizeof (struct sockaddr_in));
    MulticastServer::m_addrinMulticast.sin_family = AF_INET;
    MulticastServer::m_addrinMulticast.sin_port = htons(_MULTICAST_PORT);
    inet_pton(AF_INET, _MULTICAST_ADDRESS, &(MulticastServer::m_addrinMulticast.sin_addr));

    MulticastServer::m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memcpy(&MulticastServer::m_mreq.imr_interface, &(MulticastServer::m_addrinServer.sin_addr), sizeof (struct in_addr));
    memcpy(&MulticastServer::m_mreq.imr_multiaddr, &(MulticastServer::m_addrinMulticast.sin_addr), sizeof (struct in_addr));
    if (setsockopt(MulticastServer::m_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &MulticastServer::m_mreq, sizeof (struct ip_mreq)) == -1)
        return false;
    if (bind(MulticastServer::m_sockfd, (struct sockaddr*) &(MulticastServer::m_addrinServer), sizeof (struct sockaddr_in)) == -1)
        return false;


    int flags = fcntl(MulticastServer::m_sockfd, F_GETFL, 0);
    if (fcntl(MulticastServer::m_sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
        return false;

    pthread_mutex_init(&MulticastServer::m_mapOnlineMutex, NULL);
    pthread_mutex_init(&MulticastServer::m_timeStampMutex, NULL);


    if (pthread_create(&(MulticastServer::m_threadSend), NULL, &(MulticastServer::m_onSend), NULL) != 0)
        return false;
    if (pthread_create(&(MulticastServer::m_threadRecv), NULL, &(MulticastServer::m_onRecv), NULL) != 0)
        return false;
    if (pthread_create(&(MulticastServer::m_threadTimer), NULL, &(MulticastServer::m_onTimer), NULL) != 0)
        return false;

    MulticastServer::m_Status = true;

    return true;
}

bool MulticastServer::m_Stop() {
    if (!MulticastServer::m_Status)
        return false;

    if(pthread_cancel(MulticastServer::m_threadSend) == 0)
        pthread_join(MulticastServer::m_threadSend, NULL);
    if(pthread_cancel(MulticastServer::m_threadRecv) == 0)
        pthread_join(MulticastServer::m_threadRecv, NULL);
    if(pthread_cancel(MulticastServer::m_threadTimer) == 0)
        pthread_join(MulticastServer::m_threadTimer, NULL);

    pthread_mutex_destroy(&MulticastServer::m_mapOnlineMutex);
    pthread_mutex_destroy(&MulticastServer::m_timeStampMutex);

	if (setsockopt(MulticastServer::m_sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &MulticastServer::m_mreq, sizeof (struct ip_mreq)) == -1)
        return false;
    close(MulticastServer::m_sockfd);

    MulticastServer::m_mapOnline.clear();

    MulticastServer::m_Status = false;

    return true;
}

void* MulticastServer::m_onTimer(void*) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int count = 0;
    while(true)
    {
        if(count > 2)
        {
            std::list<std::string> remove;
            pthread_mutex_lock(&MulticastServer::m_mapOnlineMutex);
            std::map<std::string, TMC>::iterator mit = MulticastServer::m_mapOnline.begin();
            while(mit != MulticastServer::m_mapOnline.end())
            {
                mit->second.COUNT += 2;
                if(mit->second.COUNT > 6)
                    remove.push_back(mit->first);
                mit++;
            }
            std::list<std::string>::iterator lit = remove.begin();
            while(lit != remove.end() )
            {
                MulticastServer::m_mapOnline.erase(*lit);
                lit++;
            }
            pthread_mutex_unlock(&MulticastServer::m_mapOnlineMutex);
            count = 0;
        }
        pthread_testcancel();
        usleep(1000 * 1000);
        count++;
    }

}

void* MulticastServer::m_onSend(void*) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    int scount = 6;
    
    char* iphost = NULL;
    MulticastServer::m_getIPHost(&iphost);

    NTCFPackage pkg;
    pkg.set_MajorCode(_NCSFPC_MJ_CMD_REQ_);
    pkg.set_MinorCode(_NCSFPC_MN_TYPE_MULTICAST_);
    pkg.set_DataInfo(iphost);
    free(iphost);

    while (true)
    {
        scount++;
        if (scount > 5) 
        {
            struct tm *timenow;
            time_t timestamp;
            char timestr[255] = {0};
            time(&timestamp);
            timenow = localtime(&timestamp);
            strftime(timestr, 255, "%Z %Y-%m-%d %H:%M:%S", timenow);
            pkg.set_EtraInfo(timestr);
            pthread_mutex_lock(&MulticastServer::m_timeStampMutex);
            MulticastServer::m_timeStamp = std::string(timestr);
            pthread_mutex_unlock(&MulticastServer::m_timeStampMutex);
            char* msg;
            pkg.get_Base64Str(&msg);
            sendto(MulticastServer::m_sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr*) &(MulticastServer::m_addrinMulticast), sizeof (struct sockaddr_in));
            gettimeofday(&MulticastServer::m_timevalSend, NULL);
            free(msg);
            scount = 0;
        }
        pthread_testcancel();
        usleep(1000 * 1000);
    }
}

void* MulticastServer::m_onRecv(void*) {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    char buf[4096] = {0};
    char bufRecv[512] = {0};
    unsigned int socklen = sizeof (struct sockaddr_in);
    int length = 0;
    struct timeval timevalRecv;
    while(true)
    {
        bzero(bufRecv, 512*sizeof(char));
        int len = recvfrom(MulticastServer::m_sockfd, bufRecv, 512, 0, (struct sockaddr*) &(MulticastServer::m_addrinMulticast), &socklen);
        if(len > 0)
        {
            memcpy(buf+length, bufRecv, len*sizeof(char));
            length += len;
        }
        if(length >0 && buf[length-1] == '\0')
        {
            gettimeofday(&timevalRecv, NULL);
            NTCFPackage pkg;
            pkg.set_Base64Str(buf);
            if(pkg.get_MajorCode() == _NCSFPC_MJ_CMD_RTN_)
            {
                char* timestamp;
                pkg.get_EtraInfo(&timestamp);
                std::string recvstamp = std::string(timestamp);
                free(timestamp);
                pthread_mutex_lock(&MulticastServer::m_timeStampMutex);
                std::string sendstamp = MulticastServer::m_timeStamp;
                pthread_mutex_unlock(&MulticastServer::m_timeStampMutex);
                if(recvstamp == sendstamp)
                {
                    TMC mc;
                    memset(&mc, 0, sizeof(TMC));
                    mc.MIRCSEC = 1000000*(timevalRecv.tv_sec - MulticastServer::m_timevalSend.tv_sec) + timevalRecv.tv_usec - MulticastServer::m_timevalSend.tv_usec;
                    char* iphost;
                    pkg.get_DataInfo(&iphost);
                    int len = strlen(iphost);
                    int splash = 0;
                    for(int i = 0; i < len; i++)
                    {
                        if(iphost[i] == ':')
                        {
                            splash = i;
                            break;
                        }
                    }
                    memcpy(mc.IP, iphost, splash);
                    memcpy(mc.HOSTNAME, iphost+splash+1, strlen(iphost) - splash);
                    free(iphost);
                    long  a = mc.MIRCSEC;
                    std::string key = std::string(mc.IP);
                    mc.COUNT = 0;
                    pthread_mutex_lock(&MulticastServer::m_mapOnlineMutex);
                    MulticastServer::m_mapOnline[key] = mc;
                    pthread_mutex_unlock(&MulticastServer::m_mapOnlineMutex);
                }
            }
            bzero(buf, 4096);
            length = 0;
        }
        pthread_testcancel();
        usleep(50 * 1000);
    }
}

void MulticastServer::m_getIPHost(char** iphost)
{
    int sock;
    struct sockaddr_in   sin;
    struct ifreq   ifr;

    sock  = socket(AF_INET, SOCK_DGRAM, 0);
	for(int i = 0; i < 10; i++)
	{
		char* ENAME = (char*)malloc(5*sizeof(char));
		bzero(ENAME, 5);
		sprintf(ENAME, "%s%d", ETH_NAME, i);
    	strncpy(ifr.ifr_name,   ENAME,   IFNAMSIZ);
		free(ENAME);
    	ifr.ifr_name[IFNAMSIZ   -   1]   =   0;

    	if (ioctl(sock,   SIOCGIFADDR,   &ifr)   >=   0)
			goto HERE;
	}

	for(int i = 0; i < 10; i++)
	{
		char* WNAME = (char*)malloc(6*sizeof(char));
		bzero(WNAME, 6);
		sprintf(WNAME, "%s%d", WTH_NAME, i);
    	strncpy(ifr.ifr_name,   WNAME,   IFNAMSIZ);
		free(WNAME);
    	ifr.ifr_name[IFNAMSIZ   -   1]   =   0;

    	if (ioctl(sock,   SIOCGIFADDR,   &ifr)   >=   0)	
			goto HERE;
	}

HERE:
    memcpy(&sin,   &ifr.ifr_addr,   sizeof(sin));

    char* hostname = (char*)malloc(256*sizeof(char));
    bzero(hostname, 256);
    gethostname(hostname, 256*sizeof(char));
    char* ip = inet_ntoa(sin.sin_addr);
    int lenhost  = strlen(hostname);
    int lenip = strlen(ip);
    *iphost = (char*)malloc((lenhost+lenip+2)*sizeof(char));
    bzero(*iphost, (lenhost+lenip+2)*sizeof(char));
    sprintf(*iphost,   "%s:%s",  ip, hostname);
    free(hostname);
}

bool MulticastServer::m_getOnline(std::map<std::string, TMC>& map)
{
        pthread_mutex_lock(&MulticastServer::m_mapOnlineMutex);
        map = MulticastServer::m_mapOnline;
        pthread_mutex_unlock(&MulticastServer::m_mapOnlineMutex);
}
