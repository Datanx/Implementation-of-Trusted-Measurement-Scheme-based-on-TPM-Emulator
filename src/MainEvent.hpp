/* 
 * File:   MainEvent.hpp
 * Author: jjdengjj
 */

#ifndef _MAINEVENT_HPP
#define	_MAINEVENT_HPP

#define _MAINEVENT_PROCNUM 5

#include <queue>
#include <pthread.h>
#include "SubEvent.hpp"
#include "MainClient.hpp"
#include "NTCFExecutor.hpp"

class MainEvent {
public:
    MainEvent();
    virtual ~MainEvent();
    
private:
    static std::queue<SubEvent> m_queueEvent;
    static pthread_mutex_t m_queueEventMutex;
    static int m_status;
    static pthread_t m_threadTranEvent;
    static pthread_t m_threadProcEvent[_MAINEVENT_PROCNUM];

    static std::map<std::string, std::string> mapIP;
    static pthread_mutex_t mapIPMutex;

public:
    static void m_start();
    static void m_stop();

private:
    static void* on_tranEvent(void*);
    static void* on_procEvent(void*);
};

#endif	/* _MAINEVENT_HPP */
