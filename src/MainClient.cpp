/* 
 * File:   MainClient.cpp
 * Author: jjdengjj
 */

#include "MainClient.hpp"
#include "errorno.hpp"

std::map<std::string, SubClient> MainClient::m_MainClientMap;
int MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_SUCCESS;
pthread_mutex_t MainClient::m_MainClientMapMutex;
int MainClient::m_MainClientStatus = _MAINCLIENT_STOP;
std::queue<CMSG> MainClient::m_queueSend;
pthread_mutex_t MainClient::m_queueSendMutex;
std::queue<CMSG> MainClient::m_queueRecv;
pthread_mutex_t MainClient::m_queueRecvMutex;
pthread_t MainClient::m_sendThread;
CID MainClient::m_Para;
pthread_mutex_t MainClient::m_ParaMutex;

MainClient::MainClient() {
}

MainClient::~MainClient() {
}

bool MainClient::m_Start()
{
    if(MainClient::m_MainClientStatus == _MAINCLIENT_START)
        return false;

    MainClient::m_initMainClient();

    if(pthread_create(&MainClient::m_sendThread, NULL, &(MainClient::m_onSend), NULL) !=0)
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_SENDTHREADCREATE;
        return false;
    }

    MainClient::m_MainClientStatus = _MAINCLIENT_START;
    return true;
}

bool MainClient::m_Stop()
{
    if(MainClient::m_MainClientStatus == _MAINCLIENT_STOP)
        return false;

    if(pthread_cancel(MainClient::m_sendThread) !=0)
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_CANCELSENDTHREAD;
    pthread_join(MainClient::m_sendThread, NULL);
    pthread_mutex_lock(&MainClient::m_queueSendMutex);
    while(!MainClient::m_queueSend.empty())
        MainClient::m_queueSend.pop();
    pthread_mutex_unlock(&MainClient::m_queueSendMutex);
    pthread_mutex_destroy(&MainClient::m_queueSendMutex);

    pthread_mutex_lock(&MainClient::m_MainClientMapMutex);

    std::map<std::string, SubClient>::iterator  mit = MainClient::m_MainClientMap.begin();
    while(mit != MainClient::m_MainClientMap.end())
    {
        if(pthread_cancel(mit->second.m_getSubClientThread()) !=0)
            MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_CANCELRECEIVETHREAD;
        else
        {
            pthread_join(mit->second.m_getSubClientThread(), NULL);
            close(mit->second.m_getSubClientfd());
        }
        mit++;
    }
    MainClient::m_MainClientMap.clear();

    pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
    pthread_mutex_destroy(&MainClient::m_MainClientMapMutex);

    pthread_mutex_lock(&MainClient::m_queueRecvMutex);
    while(!MainClient::m_queueRecv.empty())
        MainClient::m_queueRecv.pop();
    pthread_mutex_unlock(&MainClient::m_queueRecvMutex);
    pthread_mutex_destroy(&MainClient::m_queueRecvMutex);

    pthread_mutex_destroy(&MainClient::m_ParaMutex);

    MainClient::m_MainClientStatus = _MAINCLIENT_STOP;
    
    return true;
}

int MainClient::m_getMainClientLastErrorNO()
{
    return MainClient::m_MainClientErrorNO;
}

int MainClient::m_getMainClientStatus()
{
    return MainClient::m_MainClientStatus;
}

void MainClient::m_initMainClient()
{
    pthread_mutex_init(&MainClient::m_MainClientMapMutex, NULL);
    pthread_mutex_init(&MainClient::m_queueSendMutex, NULL);
    pthread_mutex_init(&MainClient::m_queueRecvMutex, NULL);
    pthread_mutex_init(&MainClient::m_ParaMutex, NULL);
}

bool MainClient::m_addSubClient(int clientfd, struct sockaddr_in clientaddr)
{
    if(MainClient::m_MainClientStatus != _MAINCLIENT_START)
        return false;
    
    if(MainClient::m_MainClientMap.size() >= _MAINCLIENT_CAPACITY)
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_OUTOFCAPACITY;
        close(clientfd);
        return false;
    }
    pthread_mutex_lock(&MainClient::m_MainClientMapMutex);
    
    char* ip;
    ip = inet_ntoa(clientaddr.sin_addr);
    int port;
    port = ntohs(clientaddr.sin_port);   
    char * c_key;
    c_key = (char*)malloc(22*sizeof(char));
    bzero(c_key, 22);
    sprintf(c_key, "%s:%d", ip, port);
    std::string key = std::string(c_key);
    free(c_key);

    pthread_t clientthread;
    pthread_mutex_lock(&MainClient::m_ParaMutex);
    MainClient::m_Para.id = key;
    MainClient::m_Para.fd = clientfd;
    if(pthread_create(&clientthread, NULL, &(MainClient::m_onRecv), (void*)&(MainClient::m_Para)) !=0)
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_RECEIVETHREADCREATE;
        pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
        pthread_mutex_unlock(&MainClient::m_ParaMutex);
        return false;
    }

    int flags = fcntl(clientfd, F_GETFL, 0);
    if(fcntl(clientfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_SOCKETFDNONBLOCK;
        pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
        return false;
    }

    SubClient sc = SubClient(clientfd, clientaddr, clientthread);
    MainClient::m_MainClientMap[key] = sc;

    pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
    return true;
}

bool MainClient::m_delSubClient(std::string key)
{
    if(MainClient::m_MainClientStatus != _MAINCLIENT_START)
        return false;
    
    pthread_mutex_lock(&MainClient::m_MainClientMapMutex);

    std::map<std::string, SubClient>::iterator mit = MainClient::m_MainClientMap.find(key);
    if(mit == MainClient::m_MainClientMap.end())
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLEINT_NOSUCHCLIENT;
        pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
        return false;
    }

    SubClient sc = MainClient::m_MainClientMap[key];
    if(pthread_cancel(sc.m_getSubClientThread()) !=0)
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLIENT_CANCELRECEIVETHREAD;
        pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
        return false;
    }
    pthread_join(sc.m_getSubClientThread(), NULL);
    close(sc.m_getSubClientfd());
    MainClient::m_MainClientMap.erase(key);

    pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
    
    return true;
}

bool MainClient::m_getSubClient(std::string key, SubClient& sc)
{
    if(MainClient::m_MainClientStatus != _MAINCLIENT_START)
        return false;

    pthread_mutex_lock(&MainClient::m_MainClientMapMutex);

    std::map<std::string, SubClient>::iterator mit = MainClient::m_MainClientMap.find(key);
    if(mit == MainClient::m_MainClientMap.end())
    {
        MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLEINT_NOSUCHCLIENT;
        pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
        return false;
    }

    sc = MainClient::m_MainClientMap[key];

    pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
    
    return true;
}

bool MainClient::m_getSubClientKey(std::vector<std::string>& vkey)
{
    if(MainClient::m_MainClientStatus != _MAINCLIENT_START)
        return false;

    vkey.empty();

    pthread_mutex_lock(&MainClient::m_MainClientMapMutex);

    std::map<std::string, SubClient>::iterator mit = MainClient::m_MainClientMap.begin();
    while(mit != MainClient::m_MainClientMap.end())
    {
        vkey.push_back(mit->first);
        mit++;
    }

    pthread_mutex_unlock(&MainClient::m_MainClientMapMutex);
    return true;
}

bool MainClient::m_getRecv(CMSG& recv)
{
    bool rt = false;
    pthread_mutex_lock(&MainClient::m_queueRecvMutex);
    if(MainClient::m_queueRecv.size() == 0)
    {
        rt = false;
    }
    else
    {
        recv = MainClient::m_queueRecv.front();
        MainClient::m_queueRecv.pop();
        rt = true;
    }
    pthread_mutex_unlock(&MainClient::m_queueRecvMutex);
    return rt;
}

bool MainClient::m_setRecv(CMSG recv)
{
    pthread_mutex_lock(&MainClient::m_queueRecvMutex);
    MainClient::m_queueRecv.push(recv);
    pthread_mutex_unlock(&MainClient::m_queueRecvMutex);
    return true;
}

bool MainClient::m_getSend(CMSG& send)
{
    bool rt = false;
    pthread_mutex_lock(&MainClient::m_queueSendMutex);
    if(MainClient::m_queueSend.size() == 0)
    {
        rt = false;
    }
    else
    {
        send = MainClient::m_queueSend.front();
        MainClient::m_queueSend.pop();
        rt = true;
    }
    pthread_mutex_unlock(&MainClient::m_queueSendMutex);
    return rt;
}

bool MainClient::m_setSend(CMSG send)
{
    CMSG s;
    s.id = send.id;
    s.msg = send.msg;
    pthread_mutex_lock(&MainClient::m_queueSendMutex);
    MainClient::m_queueSend.push(s);
    pthread_mutex_unlock(&MainClient::m_queueSendMutex);
    return true;
}

void* MainClient::m_onRecv(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    CID argCD;
    argCD.fd = ((CID*)arg)->fd;
    argCD.id = ((CID*)arg)->id;
    pthread_mutex_unlock(&MainClient::m_ParaMutex);
    char bufSMSG[4096] = {0};//Store MSG buffer
    char bufRMSG[512] = {0};//Receive MSG buffer
    bool isIgnore = false;
    int sizeRMSG = 0;
    int sizeSCount = 0;
    int sizeRCount = 0;
    while(true)
    {
        sizeRMSG = recv(argCD.fd, (void*)bufRMSG, 512, 0);
        for(sizeRCount = 0; sizeRCount <sizeRMSG; sizeRCount++)
        {
            //The Length of the MSG is Out of the Boundary
            if(sizeSCount >4096)
            {
                isIgnore = true;
                bzero(bufSMSG, 4096);
                sizeSCount = 0;
            }
            if(bufRMSG[sizeRCount] != _MAINCLIENT_MSGEND) // If we don't count the end of the MSG, then copy it.
            {
                if(!isIgnore)
                {
                    bufSMSG[sizeSCount] = bufRMSG[sizeRCount];
                    sizeSCount++;
                }
            }
            else // If we count the end of the MSG, then post it.
            {
                char* MSG = (char*)malloc((sizeSCount+1)*sizeof(char));
                bzero(MSG, (sizeSCount+1));
                memcpy(MSG, bufSMSG, sizeSCount);
                CMSG tmp;
                tmp.id = argCD.id;
                tmp.msg = std::string(MSG);
                free(MSG);
                MainClient::m_setRecv(tmp);
                isIgnore = false;
                bzero(bufSMSG, 4096);
                sizeSCount = 0;
            }
        }
        bzero(bufRMSG, 512);
        sizeRCount = 0;
        pthread_testcancel();
        usleep(100*1000);
    }
}

void* MainClient::m_onSend(void* arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    CMSG MSG;
    SubClient SC;
    while(true)
    {
        if(MainClient::m_getSend(MSG))
        {
            if(MainClient::m_getSubClient(MSG.id, SC))
            {
                int fd = SC.m_getSubClientfd();
                int leng = MSG.msg.size()+1;
                char* m = (char*)malloc(leng*sizeof(char));
                memcpy(m, MSG.msg.c_str(), leng*sizeof(char));
                send(fd, m, leng, 0);
                free(m);
            }
            else
            {
                MainClient::m_MainClientErrorNO = _ERRORNO_MAINCLEINT_NOSUCHCLIENT;
            }
        }
        pthread_testcancel();
        usleep(100*1000);
    }
}
