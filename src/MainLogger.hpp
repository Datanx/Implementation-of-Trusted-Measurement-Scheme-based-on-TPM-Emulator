/* 
 * File:   MainLogger.hpp
 * Author: jjdengjj
 *
 * Created on November 23, 2009, 9:47 AM
 */

#ifndef _MAINLOGGER_HPP
#define	_MAINLOGGER_HPP

#ifndef _NTCFRAMEWORK_LOGFILE
#define _NTCFRAMEWORK_LOGFILE "/var/NTCF/NTCF.log"
#endif

#ifndef _NTCFRAMEWORK_LOGPATH
#define _NTCFRAMEWORK_LOGPATH "/var/NTCF"
#endif

#ifndef _NTCFRAMEWORK_LOGBUF_MAXLENGTH
#define _NTCFRAMEWORK_LOGBUF_MAXLENGTH 4096
#endif

#include <unistd.h>
#include <string>
#include <string.h>
#include <queue>
#include <pthread.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

class MainLogger {
public:
    MainLogger();
    MainLogger(const MainLogger& orig);
    virtual ~MainLogger();
private:
    static std::queue<std::string> m_logQueue;
    static pthread_mutex_t m_logMutex;
    static std::string m_logFile;
    static bool status;
public:
    static bool m_setLog(std::string);
    static bool m_getLog(std::string&);
    static bool m_saveLog();
};

#endif	/* _MAINLOGGER_HPP */

