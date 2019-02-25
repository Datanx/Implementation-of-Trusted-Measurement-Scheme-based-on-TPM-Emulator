/* 
 * File:   MainServer.cpp
 * Author: jjdengjj
 */

#include "MainServer.hpp"

unsigned int MainServer::m_MainServerMaxReq = 0;
unsigned int MainServer::m_MainServerPort = 0;
int MainServer::m_MainServerSocketfd;
struct sockaddr_in MainServer::m_MainServerSocketAddr = {0};
pthread_t MainServer::m_MainServerThread;
int MainServer::m_MainServerStatus = 0;
int MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SUCCESS;

MainServer::MainServer() {
}

MainServer::~MainServer() {
}

int MainServer::m_getMainServerLastErrorNO()
{
    return MainServer::m_MainServerErrorNO;
}

int MainServer::m_getMainServerStatus()
{
    return MainServer::m_MainServerStatus;
}

bool MainServer::m_Start()
{
    if(MainServer::m_MainServerStatus == _MAINSERVER_SERVER_START)
    {
        return false;
    }

    MainClient::m_Start();
    MainEvent::m_start();

    MainServer::m_initMainServerSocket(0,0);

    MainServer::m_MainServerSocketfd = socket(AF_INET,SOCK_STREAM,0);
    if(MainServer::m_MainServerSocketfd == -1)
    {
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SOCKETFDCREATE;
        return false;
    }

    int flags = fcntl(MainServer::m_MainServerSocketfd, F_GETFL, 0);
    if(fcntl(MainServer::m_MainServerSocketfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SOCKETFDNONBLOCK;
        return false;
    }

    MainServer::m_MainServerSocketAddr.sin_family = AF_INET;
    MainServer::m_MainServerSocketAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    MainServer::m_MainServerSocketAddr.sin_port = htons(MainServer::m_MainServerPort);
    bzero(&(MainServer::m_MainServerSocketAddr.sin_zero), 8);

    if(bind(MainServer::m_MainServerSocketfd, (struct sockaddr*)&(MainServer::m_MainServerSocketAddr), sizeof(struct sockaddr)) == -1)
    {
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SOCKETBIND;
        return false;
    }

    if(listen(MainServer::m_MainServerSocketfd, MainServer::m_MainServerMaxReq) == -1)
    {
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SOCKETLISTEN;
        return false;
    }

    if(pthread_create(&(MainServer::m_MainServerThread), NULL, MainServer::m_onAccept, NULL) !=0)
    {
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SERVERLISTENTHREADCREATE;
        return false;
    }

    MainServer::m_MainServerStatus = _MAINSERVER_SERVER_START;
    
    return true;
}

bool MainServer::m_Stop()
{
    if(MainServer::m_MainServerStatus == _MAINSERVER_SERVER_STOP)
    {
        return false;
    }

    if(pthread_cancel(MainServer::m_MainServerThread) !=0)
        MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_CANCELACCEPTTHREAD;
    pthread_join(MainServer::m_MainServerThread, NULL);

    MainEvent::m_stop();
    MainClient::m_Stop();
    
    close(MainServer::m_MainServerSocketfd);

    MainServer::m_MainServerStatus = _MAINSERVER_SERVER_STOP;
    return true;
}

bool MainServer::m_initMainServerSocket(int port, int maxreq)
{
    if(port <= 0)
        MainServer::m_MainServerPort = _MAINSERVER_SERVER_PORT;
    else
        MainServer::m_MainServerPort = port;
    
    if(maxreq <=0)
        MainServer::m_MainServerMaxReq = _MAINSERVER_SERVER_MAXREQ;
    else
        MainServer::m_MainServerMaxReq = maxreq;

    return true;
}

void* MainServer::m_onAccept(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int clientfd = -1;
    struct sockaddr_in clientaddr = {0};
    socklen_t size = sizeof(struct sockaddr);
    while(true)
    {
        clientfd = accept(MainServer::m_MainServerSocketfd, (struct sockaddr*)&clientaddr, &size);
        if(clientfd != -1)
        {
            MainClient::m_addSubClient(clientfd, clientaddr);
            clientfd = -1;
            memset(&clientaddr, 0, sizeof(struct sockaddr_in));
        }
        else
            MainServer::m_MainServerErrorNO = _ERRORNO_MAINSERVER_SERVERACCEPTCLIENT;
        pthread_testcancel();
        usleep(100*1000);
    }
}
