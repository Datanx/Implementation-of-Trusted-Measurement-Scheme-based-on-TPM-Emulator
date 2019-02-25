/* 
 * File:   MainServer.hpp
 * Author: jjdengjj
 */

#ifndef _MAINSERVER_HPP
#define	_MAINSERVER_HPP

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <fcntl.h>

#include "errorno.hpp"
#include "MainClient.hpp"
#include "SubClient.hpp"
#include "MainEvent.hpp"
#include "SubEvent.hpp"

#ifndef _MAINSERVER_SERVER_PORT
#define _MAINSERVER_SERVER_PORT 18888
#endif

#ifndef _MAINSERVER_SERVER_MAXREQ
#define _MAINSERVER_SERVER_MAXREQ 20
#endif

#ifndef _MAINSERVER_SERVER_STOP
#define _MAINSERVER_SERVER_STOP 0
#endif

#ifndef _MAINSERVER_SERVER_START
#define _MAINSERVER_SERVER_START 1
#endif

class MainServer {
private:
    static unsigned int m_MainServerMaxReq;
    static unsigned int m_MainServerPort;
    static int m_MainServerSocketfd;
    static struct sockaddr_in m_MainServerSocketAddr;
    static pthread_t m_MainServerThread;
    static int m_MainServerStatus;
    static int m_MainServerErrorNO;

public:
    MainServer();
    virtual ~MainServer();

    static int m_getMainServerLastErrorNO();
    static int m_getMainServerStatus();
    static bool m_Start();
    static bool m_Stop();

private:
    static bool m_initMainServerSocket(int, int);
    static void* m_onAccept(void* arg);
};

#endif	/* _MAINSERVER_HPP */

