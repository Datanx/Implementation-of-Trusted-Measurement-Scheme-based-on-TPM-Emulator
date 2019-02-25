/* 
 * File:   SubClient.hpp
 * Author: jjdengjj
 */

#ifndef _SUBCLIENT_HPP
#define	_SUBCLIENT_HPP

#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>

class SubClient {
private:
    int m_SubClientfd;
    struct sockaddr_in m_SubClientSocketAddr;
    pthread_t m_SubClientThread;

public:
    SubClient();
    SubClient(const SubClient& orig);
    SubClient(int, struct sockaddr_in, pthread_t);
    virtual ~SubClient();

    SubClient& operator=(const SubClient& orig);
    int m_getSubClientfd();
    struct sockaddr_in m_getSubClientSocketAddr();
    pthread_t m_getSubClientThread();

};

#endif	/* _SUBCLIENT_HPP */

