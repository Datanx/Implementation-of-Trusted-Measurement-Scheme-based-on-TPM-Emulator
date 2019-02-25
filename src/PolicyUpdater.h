#ifndef	POLICYUPDATER_H
#define POLICYUPDATER_H

#include <string.h>
#include <string>
#include <iostream>
#include <list>
#include <stdio.h>
#include <mysql++.h>
#include "TokenTaker.h"
#include "ExString.h"
#include "CmdProcessor.h"
#include "AgentConnector.hpp"
#include "OsstartPolicyOperator.h"
#include "MainLogger.hpp"

typedef struct _HWSW_NAME_HASH_LIST_NODE
{
	std::string		m_name;
	std::string		m_hash;
}HWSW_NAME_HASH_LIST_NODE, *PHWSW_NAME_HASH_LIST_NODE;

class PolicyUpdater
{
public:
	//used for update policy
	PolicyUpdater(std::list<HWSW_NAME_HASH_LIST_NODE> &hwsw_name_hash_list, std::string md_name, std::string m_or_d, std::string srv_name, std::string policy_str, std::string sw_ident_str, std::string logic_str, std::string addition);
	//used for update script
	PolicyUpdater(std::list<HWSW_NAME_HASH_LIST_NODE> &hwsw_name_hash_list, std::string md_name, std::string m_or_d, std::string scriptText);
	bool UpdatePolicy();
	bool UpdateScript();
        void SetTargetIP(std::string ip);
private:
	std::list<HWSW_NAME_HASH_LIST_NODE> &m_hwsw_name_hash_list;
	std::string m_machine_domain_name;
	std::string m_machine_or_domain;
	std::string m_service_name;
	std::string m_policy_str;
	std::string m_sw_ident_str;
	std::string m_logic_str;
	std::string m_addition;
	std::string m_script_text;
	std::string m_hash_set;

        std::string m_mem_hash_list;
        std::string m_nic_hash_list;
        std::string m_usb_hash_list;
        std::string m_hash_string;

        std::string m_target_ip_addr;   //the ip address of machine, used for osstart policy application
		
	int m_mem_ptr;
	int m_usb_ptr;
	int m_nic_ptr;
private:
	PHWSW_NAME_HASH_LIST_NODE get_hash_list_node_by_name(std::string token);
};

#endif
