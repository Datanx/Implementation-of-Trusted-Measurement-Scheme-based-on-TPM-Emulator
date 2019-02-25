#ifndef	POLICYSCRIPTINTERPRETER_H
#define POLICYSCRIPTINTERPRETER_H

#include <string>
#include <string.h>
#include <list>
#include "plcyScript.h"
#include "PolicyUpdater.h"
using namespace std;

#define PSI_SUCCESS			0					//Interpret successfully
#define	PSI_FILEOPENFAILED	1					//Open script file failed
#define PSI_SYNTAXERROR		2					//Script syntax error
#define PSI_TOKENNOTFOUNDINCURRENTHWSWLIST	3	//Identifier appears in script cannot be found in hw/sw list
#define PSI_OTHERERROR		99					//Other error

#define SS_FILE				1	//Script source: file
#define SS_TEXT				2	//Script source: text

class PSI
{
private:
	void initVars();
	void processFromFile(string& filename);
	void processFromText(string& script);
	PHWSW_NAME_HASH_LIST_NODE get_hash_list_node_by_name(std::list<HWSW_NAME_HASH_LIST_NODE> hwsw_name_hash_list, std::string token);
public:
	PSI(string& s, int ss_type);
	int Interpret();
	int CheckTokenExistence(std::list<HWSW_NAME_HASH_LIST_NODE> hwsw_name_hash_list, std::string& first_find_invalid_token);
public:
	string m_machineName;
	string m_machineOrDomain;
	SRV_PLCY m_psiArray[PSI_BUFFER_LEN];
	int m_psiArrayIndex;
	FILE *m_fd;
	int m_scriptSrc;
};

#endif
