#include "PolicyUpdater.h"
#include "NTCSDBConfig.h"

/*****************************************************
*Function: Constructor 1(Update policy string).
*Parameters:
****Param 1(IN): hw/sw name-hash list.
****Param 2(IN): Machine or domain name.
****Param 3(IN): Specifies machine or domain.
****Param 4(IN): Service name.
****Param 5(IN): Policy string.
****Param 6(IN): Software policy identifier string(specifies which software policy to be used).
****Param 7(IN): A logic string for certain service policy.
****Param 8(IN): Addition for policy data table.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PolicyUpdater::PolicyUpdater(
	std::list<HWSW_NAME_HASH_LIST_NODE> &hwsw_name_hash_list, 
	std::string md_name, 
	std::string m_or_d, 
	std::string srv_name, 
	std::string policy_str, 
	std::string sw_ident_str, 
	std::string logic_str, 
	std::string addition
):m_hwsw_name_hash_list(hwsw_name_hash_list)
{
	m_machine_domain_name = md_name;
	m_machine_or_domain = m_or_d;
	m_service_name = srv_name;
	m_policy_str = policy_str;
	m_sw_ident_str = sw_ident_str;
	m_logic_str = logic_str;
	m_addition = addition;
	
	m_mem_ptr = 1;	//default to 1(null hash value)
	m_usb_ptr = 1;
	m_nic_ptr = 1;

        m_mem_hash_list.assign("0");
        m_nic_hash_list.assign("0");
        m_usb_hash_list.assign("0");
}

/*****************************************************
*Function: Constructor 2(Update policy script).
*Parameters:
****Param 1(IN): hw/sw name-hash list.
****Param 2(IN): Machine or domain name.
****Param 3(IN): Specifies machine or domain.
****Param 4(IN): Policy script text.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PolicyUpdater::PolicyUpdater(
	std::list<HWSW_NAME_HASH_LIST_NODE> &hwsw_name_hash_list, 
	std::string md_name, 
	std::string m_or_d, 
	std::string scriptText
):m_hwsw_name_hash_list(hwsw_name_hash_list)
{
	m_machine_domain_name = md_name;
	m_machine_or_domain = m_or_d;
	m_script_text = scriptText;
}

/*****************************************************
*Function: Update policy in database.
*Parameters: No parameter.
*Return: Update execution result.
*Remark: Nothing.
*****************************************************/
bool PolicyUpdater::UpdatePolicy()
{
	PHWSW_NAME_HASH_LIST_NODE node = NULL;
	std::string null_hash("00000000000000000000000000000000");
	std::string hash_pc(null_hash), hash_bios(null_hash), hash_mb(null_hash), hash_cpu(null_hash), hash_mem(null_hash), hash_hdd(null_hash), hash_usb(null_hash), hash_nic(null_hash);
	std::string token;
	
	TokenTaker tt(m_logic_str);
	while(tt.GetNextToken(token) != GETTOKEN_SOURCESCANFIN)
	{
		ExString::esToUpper(token);
		std::string ident(token);
		if(ident.compare("AND")==0 || ident.compare("OR")==0 || ident.compare("NOT")==0)
			continue;
		//printf("ident: %s\n", ident.c_str());
		if((node = this->get_hash_list_node_by_name(ident)) != NULL)
		{
			if(ident.find("PC") != std::string::npos)
				hash_pc = node->m_hash;
			if(ident.find("BIOS") != std::string::npos)
				hash_bios = node->m_hash;
			if(ident.find("MB") != std::string::npos)
				hash_mb = node->m_hash;
			if(ident.find("CPU") != std::string::npos)
				hash_cpu = node->m_hash;
			if(ident.find("HDD") != std::string::npos)
				hash_hdd = node->m_hash;
			if(ident.find("MEM") != std::string::npos)
			{
				if(hash_mem.compare(null_hash)==0)
				{
					hash_mem="";
					m_mem_ptr = 0;
				}
				m_mem_ptr++;
				hash_mem += node->m_hash;
                                if(this->m_service_name == "osstart")
                                {
                                    if(m_mem_hash_list == "0")
                                        m_mem_hash_list = "";
                                    m_mem_hash_list = "|" + node->m_hash + m_mem_hash_list;
                                }
			}
			if(ident.find("USB") != std::string::npos)
			{
				if(hash_usb.compare(null_hash)==0)
				{
					hash_usb="";
					m_usb_ptr = 0;
				}
				m_usb_ptr++;
				hash_usb += node->m_hash;
                                if(this->m_service_name == "osstart")
                                {
                                    if(m_usb_hash_list == "0")
                                        m_usb_hash_list = "";
                                    m_usb_hash_list = "|" + node->m_hash + m_usb_hash_list;
                                }
			}
			if(ident.find("NIC") != std::string::npos)
			{
				if(hash_nic.compare(null_hash)==0)
				{
					hash_nic="";
					m_nic_ptr = 0;
				}
				m_nic_ptr++;
				hash_nic += node->m_hash;
                                if(this->m_service_name == "osstart")
                                {
                                    if(m_nic_hash_list == "0")
                                        m_nic_hash_list = "";
                                    m_nic_hash_list = "|" + node->m_hash + m_nic_hash_list;
                                }
			}
		}//if
	}//while

        //create memory, nic & usb hash string which will be used for osstart
        if(this->m_service_name == "osstart")
        {
            if(m_mem_hash_list != "0") this->m_mem_hash_list.erase(0, 1);
            if(m_nic_hash_list != "0") this->m_nic_hash_list.erase(0, 1);
            if(m_usb_hash_list != "0") this->m_usb_hash_list.erase(0, 1);

            this->m_hash_string = this->m_mem_hash_list + "$" + this->m_nic_hash_list + "$" + this->m_usb_hash_list;
        }
	
	//Construct the hash set
	char buf[8]={0};
	sprintf(buf, "%02d", m_mem_ptr);
	std::string mem_num(buf);
	memset(buf, 0, 8);
	sprintf(buf, "%02d", m_usb_ptr);
	std::string usb_num(buf);
	memset(buf, 0, 8);
	sprintf(buf, "%02d", m_nic_ptr);
	std::string nic_num(buf);
	m_hash_set = "01" + hash_pc + "01" + hash_bios + "01" + hash_mb + "01" + hash_cpu + mem_num + hash_mem + "01" + hash_hdd + usb_num + hash_usb + nic_num + hash_nic;
	
	//Update the database
	char sql_stmt[1024] = {0};
	std::string domain_of_machine("");	//machine's domain, useless when m_machine_or_domain is DOMAIN
	m_machine_or_domain=="MACHINE" ? domain_of_machine="DEFAULT_DOMAIN" : domain_of_machine="";
	
	mysqlpp::Connection conn(false);
	if(conn.connect(DBNAME, HOST, USER, PWD))
	{
		//printf("SQL: %s\n", sql_stmt);
		mysqlpp::Query query = conn.query();
                //Execute Update
                memset(sql_stmt, 0, 1024);
                sprintf(sql_stmt,
                    "CALL SP_UPDATEPOLICY (\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\', \'%s\')",
                    m_machine_domain_name.c_str(),
                    m_machine_or_domain.c_str(),
                    domain_of_machine.c_str(),
                    m_service_name.c_str(),
                    m_policy_str.c_str(),
                    m_sw_ident_str.c_str(),
                    m_hash_set.c_str(),
                    m_addition.c_str());
                if(!query.exec(std::string(sql_stmt)))
                {
                    printf("call SP_UPDATEPOLICY error!\n");
                    return false;
                }

                //If current update is osstart service, apply osstart policy(reverse policy)
                if(this->m_service_name == "osstart")
                {
                    char* msg = NULL;
                    int osstart_exec_status = APP_PLCY_STATUS_SUCCESS;
                    std::string error_message("");
                    CmdProcessor cp;
                    AgentConnector ac(this->m_target_ip_addr.c_str());
                    
                    char data_buf[1024] = {0};
                    char extra_data_buf[4096] = {0};

                    memcpy(data_buf, m_policy_str.c_str(), m_policy_str.size() * sizeof(char));
                    memcpy(extra_data_buf, m_hash_string.c_str(), m_hash_string.size() * sizeof(char));

                    msg = cp.CmdCreator(
                            CMD_MJ_REQ_EXEC_OSSTART_POLICY,
                            CMD_MN_NULL,
                            data_buf,
                            m_policy_str.size(),
                            extra_data_buf,
                            m_hash_string.size()
                    );

                    //printf("OSSTART DATA: %s\n", data_buf);
                    //printf("OSSTART EXTRA DATA %s\n", extra_data_buf);

                    ac.m_openAgent();
                    osstart_exec_status = ac.m_setOSStart(msg);
                    ac.m_closeAgent();

                    if(msg != NULL) free(msg);

                    if(osstart_exec_status != APP_PLCY_STATUS_SUCCESS)
                    {
                        switch(osstart_exec_status)
                        {
                            case APP_PLCY_STATUS_FILEOPENFAILED:
                                error_message = "OSSTART POLICY EXECUTION ERROR: Agent file write failed";
                                break;
                            default:
                                error_message = "OSSTART POLICY EXECUTION ERROR: Other errors";
                                break;
                        }
                        MainLogger::m_setLog(error_message);
                        return false;
                    }
                    else
                    {
                        MainLogger::m_setLog("OSSTART POLICY EXECUTION SUCCESS!");
                    }
                }

                return true;
	}//if
        else
        {
            printf("Failed to connect to DB\n");
        }
	
	return false;
}

/*****************************************************
*Function: Update policy script in database.
*Parameters: No parameter.
*Return: Update execution result.
*Remark: Nothing.
*****************************************************/
bool PolicyUpdater::UpdateScript()
{
	//Update script
	char sql_stmt[1024] = {0};

	sprintf(sql_stmt, 
                "CALL SP_UPDATESCRIPT (\'%s\',\'%s\',\'%s\')",
                m_machine_domain_name.c_str(),
                m_machine_or_domain.c_str(),
                m_script_text.c_str());
	mysqlpp::Connection conn(false);
	if(conn.connect(DBNAME, HOST, USER, PWD))
	{
		//printf("SQL: %s\n", sql_stmt);
		mysqlpp::Query query = conn.query();
		if(query.exec(std::string(sql_stmt)))
		{
			return true;
			//printf("SQL exec OK\n");
		}
	}
	
	return false;
}

/*****************************************************
*Function: Get hw/sw name-hash list node by token name.
*Parameters:
****Param 1: token(IDENTIFIER).
*Return: hw/sw name-hash list node.
*Remark: Nothing.
*****************************************************/
PHWSW_NAME_HASH_LIST_NODE PolicyUpdater::get_hash_list_node_by_name(std::string token)
{
	std::list<HWSW_NAME_HASH_LIST_NODE>::iterator it;
	
	for(it=m_hwsw_name_hash_list.begin(); it != m_hwsw_name_hash_list.end(); it++)
	{
		if((*it).m_name.compare(token) == 0)
			return &(*it);
	}
	
	return NULL;
}

/*****************************************************
*Function: Set the ip address to be updated
*Parameters:
****Param 1: IP address.
*Return: void.
*Remark: Nothing.
*****************************************************/
void PolicyUpdater::SetTargetIP(std::string ip)
{
    this->m_target_ip_addr = ip;
}
