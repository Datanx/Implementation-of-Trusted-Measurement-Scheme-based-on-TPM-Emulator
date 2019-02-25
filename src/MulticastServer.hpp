/* 
 * File:   MulticastServer.hpp
 * Author: jjdengjj
 *
 * Created on December 2, 2009, 10:37 AM
 */

#ifndef _MULTICASTSERVER_HPP
#define	_MULTICASTSERVER_HPP
#define ETH_NAME "eth"
#define WTH_NAME "wlan"

#include <unistd.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <map>
#include <string>
#include <list>
#include <iterator>
#include "MULTICAST.hpp"
#include "NTCFPACKAGECODE.hpp"
#include "NTCFPackage.hpp"

typedef struct MC
{
    char IP[255];
    char HOSTNAME[255];
    long MIRCSEC;
    int COUNT;
}TMC;

class MulticastServer {
public:
    MulticastServer();
    MulticastServer(const MulticastServer& orig);
    virtual ~MulticastServer();

private:
    static int m_sockfd;
    static struct sockaddr_in m_addrinServer;
    static struct sockaddr_in m_addrinMulticast;
    static struct ip_mreq m_mreq;

    static pthread_t m_threadTimer;
    static pthread_t m_threadSend;
    static pthread_t m_threadRecv;

    static std::map<std::string, TMC> m_mapOnline;
    static pthread_mutex_t m_mapOnlineMutex;
    static bool m_Status;
    static struct timeval m_timevalSend;
    static std::string m_timeStamp;
    static pthread_mutex_t m_timeStampMutex;

public:
    static bool m_Start();
    static bool m_Stop();
    static bool m_getOnline(std::map<std::string, TMC>&);
    static void m_getIPHost(char**);

private:
    static void* m_onTimer(void*);
    static void* m_onSend(void*);
    static void* m_onRecv(void*);
};

#endif	/* _MULTICASTSERVER_HPP */

