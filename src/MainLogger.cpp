/* 
 * File:   MainLogger.cpp
 * Author: jjdengjj
 * 
 * Created on November 23, 2009, 9:47 AM
 */

#include "MainLogger.hpp"
std::queue<std::string> MainLogger::m_logQueue;
pthread_mutex_t MainLogger::m_logMutex;
std::string MainLogger::m_logFile;
bool MainLogger::status = false;

MainLogger::MainLogger() {
}

MainLogger::MainLogger(const MainLogger& orig) {
}

MainLogger::~MainLogger() {
}

bool MainLogger::m_setLog(std::string log)
{
	if(!MainLogger::status)
	{
		pthread_mutex_init(&MainLogger::m_logMutex, NULL);
		MainLogger::status = true;
	}
	pthread_mutex_lock(&MainLogger::m_logMutex);
	
	time_t now;
	struct tm *timenow;
	char timestr[255] = {0};
	time(&now);
	timenow = localtime(&now);
	strftime(timestr, 255, "%Z %Y-%m-%d %H:%M:%S -- ", timenow);
	std::string logitem = std::string(timestr) + log;
	
	MainLogger::m_logQueue.push(logitem);
	if((MainLogger::m_logFile.size() + logitem.size()) >= _NTCFRAMEWORK_LOGBUF_MAXLENGTH)
		MainLogger::m_saveLog();
	MainLogger::m_logFile = MainLogger::m_logFile + logitem + std::string("\r\n");
	
	pthread_mutex_unlock(&MainLogger::m_logMutex);
	
	return true;
}

bool MainLogger::m_getLog(std::string& log)
{
	if(!MainLogger::status)
	{
		pthread_mutex_init(&MainLogger::m_logMutex, NULL);
		MainLogger::status = true;
	}
	int rt = false;
	
	pthread_mutex_lock(&MainLogger::m_logMutex);
	
	if(!MainLogger::m_logQueue.empty())
	{
		log = MainLogger::m_logQueue.front();
		MainLogger::m_logQueue.pop();
		rt = true;
	}
	
	pthread_mutex_unlock(&MainLogger::m_logMutex);
	
	return rt;
}

bool MainLogger::m_saveLog()
{
	DIR* dir = opendir(_NTCFRAMEWORK_LOGPATH);
	if(dir == NULL)
		mkdir(_NTCFRAMEWORK_LOGPATH, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	else
		closedir(dir);
				
	int logfd = open(_NTCFRAMEWORK_LOGFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if(logfd != -1)
	{
		write(logfd, MainLogger::m_logFile.c_str(), strlen(MainLogger::m_logFile.c_str()));
		close(logfd);
		MainLogger::m_logFile.clear();
		return true;
	}
	else
		return false;
}
