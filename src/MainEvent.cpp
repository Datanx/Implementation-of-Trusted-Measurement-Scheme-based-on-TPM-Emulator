/* 
 * File:   MainEvent.cpp
 * Author: jjdengjj
 */

#include "MainEvent.hpp"

std::queue<SubEvent> MainEvent::m_queueEvent;
pthread_mutex_t MainEvent::m_queueEventMutex;
int MainEvent::m_status = 0;
pthread_t MainEvent::m_threadTranEvent;
pthread_t MainEvent::m_threadProcEvent[_MAINEVENT_PROCNUM];
std::map<std::string, std::string> MainEvent::mapIP;
pthread_mutex_t MainEvent::mapIPMutex;

MainEvent::MainEvent() {
}

MainEvent::~MainEvent() {
}

void MainEvent::m_start()
{
    if(MainEvent::m_status != 0)
        return;
    pthread_mutex_init(&MainEvent::m_queueEventMutex, NULL);
    pthread_mutex_init(&MainEvent::mapIPMutex, NULL);
    pthread_create(&MainEvent::m_threadTranEvent, NULL, &(MainEvent::on_tranEvent), NULL);
    for(int i = 0; i < _MAINEVENT_PROCNUM; i++)
        pthread_create(&MainEvent::m_threadProcEvent[i], NULL, &(MainEvent::on_procEvent), NULL);
    MainEvent::m_status = 1;
}

void MainEvent::m_stop()
{
    if(MainEvent::m_status != 1)
        return;
    pthread_cancel(MainEvent::m_threadTranEvent);
    pthread_join(MainEvent::m_threadTranEvent, NULL);

    for(int i = 0; i < _MAINEVENT_PROCNUM; i++)
    {
        pthread_cancel(MainEvent::m_threadProcEvent[i]);
        pthread_join(MainEvent::m_threadProcEvent[i], NULL);
    }

    pthread_mutex_lock(&MainEvent::m_queueEventMutex);
    while(MainEvent::m_queueEvent.size() != 0)
        MainEvent::m_queueEvent.pop();
    pthread_mutex_unlock(&MainEvent::m_queueEventMutex);
    pthread_mutex_destroy(&MainEvent::m_queueEventMutex);

    pthread_mutex_lock(&MainEvent::mapIPMutex);
    while(MainEvent::m_queueEvent.size() != 0)
        MainEvent::mapIP.clear();
    pthread_mutex_unlock(&MainEvent::mapIPMutex);
    pthread_mutex_destroy(&MainEvent::mapIPMutex);
    
    MainEvent::m_status = 0;
}

void* MainEvent::on_tranEvent(void* argc)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    while(true)
    {
        CMSG cmsg;
        if(MainClient::m_getRecv(cmsg))
        {
            SubEvent tmp = SubEvent(cmsg);
            pthread_mutex_lock(&MainEvent::m_queueEventMutex);
            MainEvent::m_queueEvent.push(tmp);
            pthread_mutex_unlock(&MainEvent::m_queueEventMutex);
        }
        pthread_testcancel();
        usleep(100*1000);
    }
    
}

void* MainEvent::on_procEvent(void* argc)
{
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    SubEvent tmp;
    bool doexec = false;

    while(true)
    {        
        pthread_mutex_lock(&MainEvent::m_queueEventMutex);
        if(MainEvent::m_queueEvent.size() != 0)
        {
            tmp = MainEvent::m_queueEvent.front();
            MainEvent::m_queueEvent.pop();
            doexec = true;
        }
        else
            doexec = false;
        pthread_mutex_unlock(&MainEvent::m_queueEventMutex);
        if(doexec)
        {
            while(true)
            {
                bool isexist = true;
                pthread_mutex_lock(&MainEvent::mapIPMutex);
                if(MainEvent::mapIP.find(tmp.m_getEventID()) == MainEvent::mapIP.end())
                {
                    MainEvent::mapIP[tmp.m_getEventID()] = tmp.m_getEventID();
                    isexist = false;
                }
                pthread_mutex_unlock(&MainEvent::mapIPMutex);
                if(isexist)
                    usleep(50*1000);
                else
                    break;
            }
            NTCFExecutor::Exec(tmp);
            pthread_mutex_lock(&MainEvent::mapIPMutex);
            MainEvent::mapIP.erase(tmp.m_getEventID());
            pthread_mutex_unlock(&MainEvent::mapIPMutex);
        }
        pthread_testcancel();
        usleep(100*1000);
    }
}
