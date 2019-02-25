/* 
 * File:   MulticastAgent.cpp
 * Author: jjdengjj
 * 
 * Created on December 3, 2009, 11:42 AM
 */

#include "MulticastAgent.hpp"
#include "MulticastServer.hpp"

int MulticastAgent::m_sockfd;
struct sockaddr_in MulticastAgent::m_addrinAgent;
struct sockaddr_in MulticastAgent::m_addrinMulticast;
struct ip_mreq MulticastAgent::m_mreq;
bool MulticastAgent::m_Status = false;
pthread_t MulticastAgent::m_threadRun;

MulticastAgent::MulticastAgent() {
}

MulticastAgent::MulticastAgent(const MulticastAgent& orig) {
}

MulticastAgent::~MulticastAgent() {
}

bool MulticastAgent::m_Start()
{
    if (MulticastAgent::m_Status)
        return false;

    memset(&MulticastAgent::m_addrinAgent, 0, sizeof (struct sockaddr_in));
    MulticastAgent::m_addrinAgent.sin_family = AF_INET;
    MulticastAgent::m_addrinAgent.sin_port = htons(_MULTICAST_PORT);
    MulticastAgent::m_addrinAgent.sin_addr.s_addr = htonl(INADDR_ANY);

    memset(&MulticastAgent::m_addrinMulticast, 0, sizeof (struct sockaddr_in));
    MulticastAgent::m_addrinMulticast.sin_family = AF_INET;
    MulticastAgent::m_addrinMulticast.sin_port = htons(_MULTICAST_PORT);
    inet_pton(AF_INET, _MULTICAST_ADDRESS, &(MulticastAgent::m_addrinMulticast.sin_addr));

    MulticastAgent::m_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memcpy(&MulticastAgent::m_mreq.imr_interface, &(MulticastAgent::m_addrinAgent.sin_addr), sizeof (struct in_addr));
    memcpy(&MulticastAgent::m_mreq.imr_multiaddr, &(MulticastAgent::m_addrinMulticast.sin_addr), sizeof (struct in_addr));
   	if (setsockopt(MulticastAgent::m_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &MulticastAgent::m_mreq, sizeof (struct ip_mreq)) == -1)
        return false;
   if(bind(MulticastAgent::m_sockfd, (struct sockaddr*) &(MulticastAgent::m_addrinAgent), sizeof (struct sockaddr_in)) == -1)
        return false;

    int flags = fcntl(MulticastAgent::m_sockfd, F_GETFL, 0);
    if (fcntl(MulticastAgent::m_sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
        return false;

    if (pthread_create(&(MulticastAgent::m_threadRun), NULL, &(MulticastAgent::m_onRun), NULL) != 0)
        return false;

    MulticastAgent::m_Status = true;
}

bool MulticastAgent::m_Stop()
{
    if (!MulticastAgent::m_Status)
        return false;
    
    if(pthread_cancel(MulticastAgent::m_threadRun) == 0)
        pthread_join(MulticastAgent::m_threadRun, NULL);
	
	if (setsockopt(MulticastAgent::m_sockfd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &MulticastAgent::m_mreq, sizeof (struct ip_mreq)) == -1)
        return false;
    close(MulticastAgent::m_sockfd);
    
    MulticastAgent::m_Status = false;
}

void* MulticastAgent::m_onRun(void*)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    char buf[4096] = {0};
    char bufRecv[512] = {0};
    unsigned int socklen = sizeof (struct sockaddr_in);
    int length = 0;

    while(true)
    {
        bzero(bufRecv, 512*sizeof(char));
        int len = recvfrom(MulticastAgent::m_sockfd, bufRecv, 512, 0, (struct sockaddr*) &(MulticastAgent::m_addrinMulticast), &socklen);
        if(len > 0)
        {
            memcpy(buf+length, bufRecv, len*sizeof(char));
            length += len;
        }
        if(length >0 && buf[length-1] == '\0')
        {
            NTCFPackage rpkg;
            rpkg.set_Base64Str(buf);
            if(rpkg.get_MajorCode() == _NCSFPC_MJ_CMD_REQ_)
            {
                char* iphost = NULL;
                MulticastServer::m_getIPHost(&iphost);              
                NTCFPackage spkg;
                spkg.set_MajorCode(_NCSFPC_MJ_CMD_RTN_);
                spkg.set_MinorCode(_NCSFPC_MN_TYPE_MULTICAST_);
                spkg.set_DataInfo(iphost);
                free(iphost);
                char* timestamp;
                rpkg.get_EtraInfo(&timestamp);
                spkg.set_EtraInfo(timestamp);
                free(timestamp);
                char* msg;
                spkg.get_Base64Str(&msg);
                sendto(MulticastAgent::m_sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr*) &(MulticastAgent::m_addrinMulticast), sizeof (struct sockaddr_in));
                free(msg);
            }
            bzero(buf, 4096);
            length = 0;
        }
        pthread_testcancel();
        usleep(50 * 1000);
    }
}
