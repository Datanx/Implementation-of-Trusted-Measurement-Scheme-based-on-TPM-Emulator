/* 
 * File:   MainClient.hpp
 * Author: jjdengjj
 */

#ifndef _MAINCLIENT_HPP
#define	_MAINCLIENT_HPP

#include <map>
#include <string>
#include <vector>
#include <iterator>
#include <queue>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include "SubClient.hpp"
#include "CMSG.hpp"

#ifndef _MAINCILENT_CAPACITY
#define _MAINCLIENT_CAPACITY 256
#endif

#ifndef _MAINCLIENT_STOP
#define _MAINCLIENT_STOP 0
#endif

#ifndef _MAINCLIENT_START
#define _MAINCLIENT_START 1
#endif

#ifndef _MAINCLIENT_CD
#define _MAINCLIENT_CD
typedef struct ClientID
{
    std::string id;
    int fd;
}CID;
#endif

#ifndef _MAINCLIENT_MSGEND
#define _MAINCLIENT_MSGEND '\0'
#endif

class MainClient {
private:
    static std::map<std::string, SubClient> m_MainClientMap;
    static int m_MainClientErrorNO;
    static pthread_mutex_t m_MainClientMapMutex;
    static int m_MainClientStatus;
    static pthread_t m_sendThread;
    static std::queue<CMSG> m_queueSend;
    static pthread_mutex_t m_queueSendMutex;
    static std::queue<CMSG> m_queueRecv;
    static pthread_mutex_t m_queueRecvMutex;
    static CID m_Para;
    static pthread_mutex_t m_ParaMutex;

public:
    MainClient();
    virtual ~MainClient();

    static bool m_Start();
    static bool m_Stop();
    static int m_getMainClientLastErrorNO();
    static int m_getMainClientStatus();
    static bool m_addSubClient(int, struct sockaddr_in);
    static bool m_delSubClient(std::string);
    static bool m_getSubClient(std::string, SubClient&);
    static bool m_getSubClientKey(std::vector<std::string>&);
    static bool m_getRecv(CMSG&);
    static bool m_setSend(CMSG);
    
private:
    static void* m_onRecv(void* arg);
    static void* m_onSend(void* arg);
    static bool m_setRecv(CMSG);
    static bool m_getSend(CMSG&);
    static void m_initMainClient();
};
#endif	/* _MAINCLIENT_HPP */
