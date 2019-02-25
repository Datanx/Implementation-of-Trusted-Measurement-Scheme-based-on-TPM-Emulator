#ifndef _t_ntcs_hwpolicy_item_h
#define _t_ntcs_hwpolicy_item_h

#include <string>

class t_ntcs_hwpolicy_item
{
public:
	int np_autoId;
	std::string np_domainName;
	std::string np_machineId;
	std::string np_serviceName;
	std::string np_policyStr;
	std::string np_identStr;
	std::string np_swIdentStr;
	std::string np_hashPc;
	std::string np_hashBios;
	std::string np_hashMb;
	std::string np_hashCpu;
	std::string np_hashMem;
	std::string np_hashHdd;
	std::string np_hashUsb;
	std::string np_hashNic;
	std::string np_Reserve1;
	std::string np_Reserve2;
	std::string np_Reserve3;
	std::string np_Reserve4;
	std::string np_Reserve5;
	std::string np_Reserve6;
	std::string np_addition;
public:
	t_ntcs_hwpolicy_item(const t_ntcs_hwpolicy_item& orig);
	t_ntcs_hwpolicy_item& operator=(const t_ntcs_hwpolicy_item& orig);
	t_ntcs_hwpolicy_item();
	~t_ntcs_hwpolicy_item();
};

#endif
