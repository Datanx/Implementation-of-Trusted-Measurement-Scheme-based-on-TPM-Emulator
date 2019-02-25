#include "PolicyScriptInterpreter.h"

extern FILE* yyin;

extern "C"
{
	int yyparse(void);
}

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1(IN): A string indicates the script file name if the ss_type is SS_FILE. If the ss_type is SS_TEXT, the parameter represents the script text.
****Param 2(IN): Integer value specifies the script source type.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PSI::PSI(string& s, int ss_type)
{
	initVars();
	
	if(ss_type == SS_FILE)
		processFromFile(s);
	else if(ss_type == SS_TEXT)
		processFromText(s);
}

/*****************************************************
*Function: Interpret the policy script.
*Parameters: No parameter.
*Return: Script interpretation result.(The return values must be one of the 5 constants: PSI_SUCCESS, PSI_FILEOPENFAILED, PSI_SYNTAXERROR, PSI_TOKENNOTFOUNDINCURRENTHWSWLIST, PSI_OTHERERROR)
*Remark: Nothing.
*****************************************************/
int PSI::Interpret()
{	
	int ret = PSI_SUCCESS;
	
	//Initialize global variables
	gPsiArrayIndex = 0;
	memset(gMachineName, 0, PSI_BUFFER_LEN);
	memset(gMachineOrDomain, 0, PSI_BUFFER_LEN);
	for(int i=0; i<PSI_BUFFER_LEN; i++)
	{
		memset(gPsiArray[i].srv_name, 0, PSI_BUFFER_LEN);
		memset(gPsiArray[i].plcy_str, 0, PSI_BUFFER_LEN);
	}
	
	if(m_fd)
	{
		ret = yyparse();
		
		if(ret == 0)
		{
			m_machineName = string(gMachineName);
			m_machineOrDomain = string(gMachineOrDomain);
			m_psiArrayIndex = gPsiArrayIndex;
			for(int i=0; i<gPsiArrayIndex; i++)
			{
				strcpy(m_psiArray[i].srv_name, gPsiArray[i].srv_name);
				strcpy(m_psiArray[i].plcy_str, gPsiArray[i].plcy_str);
				//printf("Service Name: %s\tPolicy String: %s\n", gPsiArray[i].srv_name, gPsiArray[i].plcy_str);
			}
			fclose(yyin);
			return PSI_SUCCESS;
		}
		else
		{
			fclose(yyin);
			return PSI_SYNTAXERROR;
		}
	}
	fclose(yyin);
	
	return PSI_FILEOPENFAILED;
}

/*****************************************************
*Function: Check whether the tokens(IDENTIFIER) appeared in the script exists in the hw/sw name-hash list.
*Parameters:
****Param 1(IN): hw/sw name-hash list whose nodes contain the name & hash value of hw/sw.
****Param 2(OUT): First found invalid token, which does not exist in hw/sw name-hash list.
*Return: Check result.(The return values must be one of the 3 constants: PSI_SUCCESS, PSI_TOKENNOTFOUNDINCURRENTHWSWLIST, PSI_OTHERERROR)
*Remark: Nothing.
*****************************************************/
int PSI::CheckTokenExistence(std::list<HWSW_NAME_HASH_LIST_NODE> hwsw_name_hash_list, std::string& first_find_invalid_token)
{
	if(hwsw_name_hash_list.size() == 0)
		return PSI_OTHERERROR;
		
	std::string token("");
	for(int i=0; i<m_psiArrayIndex; i++)
	{
		std::string policy_str(m_psiArray[i].plcy_str);
                if(policy_str.compare("np") == 0 || policy_str.compare("NP") == 0)
                    continue;
		//printf("policy string: %s\n", policy_str.c_str());
		TokenTaker tt(policy_str);
		while(tt.GetNextToken(token) != GETTOKEN_SOURCESCANFIN)
		{
			ExString::esToUpper(token);
			if(token=="AND" || token=="OR" || token=="NOT" || token=="(" || token==")")
				continue;
			if(this->get_hash_list_node_by_name(hwsw_name_hash_list, token) == NULL)
			{
				first_find_invalid_token = token;
				return PSI_TOKENNOTFOUNDINCURRENTHWSWLIST;
			}
		}//while
	}
	
	return PSI_SUCCESS;
}

/*****************************************************
*Function: Process script from script file.
*Parameters:
****Param 1(IN): Script file name.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PSI::processFromFile(string& filename)
{
	yyin = fopen(filename.c_str(), "r");
	if(yyin)
	{
		m_fd = yyin;
	}
}

/*****************************************************
*Function: Process script from script text.
*Parameters:
****Param 1(IN): Script text.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PSI::processFromText(string& script)
{
	m_fd = fopen("/var/Agent/tmp_psi_script", "w");
	if(m_fd)
	{
		fprintf(m_fd, "%s", script.c_str());
		fclose(m_fd);
		yyin = fopen("/var/Agent/tmp_psi_script", "r");
	}
}

/*****************************************************
*Function: Initilize the class members.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PSI::initVars()
{
	m_fd = NULL;
	m_machineName = "";
	for(int i = 0; i<PSI_BUFFER_LEN; i++)
	{
		memset(m_psiArray[i].srv_name, 0, PSI_BUFFER_LEN);
		memset(m_psiArray[i].plcy_str, 0, PSI_BUFFER_LEN);
	}
	m_psiArrayIndex = 0;
	m_scriptSrc = SS_TEXT;
}

/*****************************************************
*Function: Get name-hash list node by name(token, IDENTIFIER) in script.
*Parameters:
****Param 1(IN): hw/sw name-hash list whose nodes contain the name & hash value of hw/sw.
****Param 2(IN): The token to be found.
*Return: The found node.
*Remark: Nothing.
*****************************************************/
PHWSW_NAME_HASH_LIST_NODE PSI::get_hash_list_node_by_name(std::list<HWSW_NAME_HASH_LIST_NODE> hwsw_name_hash_list, std::string token)
{
	std::list<HWSW_NAME_HASH_LIST_NODE>::iterator it;
	
	for(it=hwsw_name_hash_list.begin(); it != hwsw_name_hash_list.end(); it++)
	{
		if((*it).m_name.compare(token) == 0)
			return &(*it);
	}
	
	return NULL;
}
