/* 
 * File:   MulticastAgent.hpp
 * Author: jjdengjj
 *
 * Created on December 3, 2009, 11:42 AM
 */

#ifndef _MULTICASTAGENT_HPP
#define	_MULTICASTAGENT_HPP

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
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
#include "MULTICAST.hpp"
#include "NTCFPACKAGECODE.hpp"
#include "NTCFPackage.hpp"

class MulticastAgent {
public:
    MulticastAgent();
    MulticastAgent(const MulticastAgent& orig);
    virtual ~MulticastAgent();
private:
    static int m_sockfd;
    static struct sockaddr_in m_addrinAgent;
    static struct sockaddr_in m_addrinMulticast;
    static struct ip_mreq m_mreq;
    static bool m_Status;
    static pthread_t m_threadRun;

public:
    static bool m_Start();
    static bool m_Stop();
private:
    static void* m_onRun(void*);
};

#endif	/* _MULTICASTAGENT_HPP */

