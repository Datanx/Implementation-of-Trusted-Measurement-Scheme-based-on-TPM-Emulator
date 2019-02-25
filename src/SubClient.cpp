/* 
 * File:   SubClient.cpp
 * Author: jjdengjj
 * 
 * Created on November 5, 2009, 7:32 PM
 */

#include "SubClient.hpp"

SubClient::SubClient(){
}

SubClient::SubClient(const SubClient& orig){
    this->m_SubClientfd = orig.m_SubClientfd;
    this->m_SubClientSocketAddr = orig.m_SubClientSocketAddr;
    this->m_SubClientThread = orig.m_SubClientThread;
}

SubClient::SubClient(int clientfd, struct sockaddr_in clientsocketaddr, pthread_t clientthread) {
    this->m_SubClientfd = clientfd;
    this->m_SubClientSocketAddr = clientsocketaddr;
    this->m_SubClientThread = clientthread;
}

SubClient::~SubClient() {
}

int SubClient::m_getSubClientfd()
{
    return this->m_SubClientfd;
}

struct sockaddr_in SubClient::m_getSubClientSocketAddr()
{
    return this->m_SubClientSocketAddr;
}

pthread_t SubClient::m_getSubClientThread()
{
    return this->m_SubClientThread;
}

SubClient& SubClient::operator =(const SubClient& orig)
{
    this->m_SubClientfd = orig.m_SubClientfd;
    this->m_SubClientSocketAddr = orig.m_SubClientSocketAddr;
    this->m_SubClientThread = orig.m_SubClientThread;
}
