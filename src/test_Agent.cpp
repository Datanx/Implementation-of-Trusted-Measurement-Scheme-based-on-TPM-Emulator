#include "Agent.hpp"
#include <unistd.h>
#include <string>
#include "CmdProcessor.h"

void* onReceiveProc(void* msg)
{
	CmdProcessor cp;
	cp.CmdParser((char*)msg);
}

int main()
{
	std::string str;
	Agent::m_onRcvProc=(void*)&(onReceiveProc);
	Agent::m_startAgent();
	while(true)
	{
		if(Agent::m_getLog(str))
			printf("%s\n", str.c_str());
		usleep(75 * 1000);
	}
	Agent::m_stopAgent();
}
