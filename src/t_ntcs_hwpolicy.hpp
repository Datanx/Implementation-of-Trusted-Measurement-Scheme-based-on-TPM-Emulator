#ifndef _t_ntcs_hwpolicy_h
#define _t_ntcs_hwpolicy_h

#include <vector>
#include <iterator>
#include <string>
#include <mysql++.h>
#include <ssqls.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include "t_ntcs_hwpolicy_item.hpp"
#include "HWCollection.hpp"
#include "NTCSDBConfig.h"
#include "PolicyOperator.h"
extern "C"{
#include "lib_StringAdd.h"
}

class t_ntcs_hwpolicy
{
public:
	std::string m_np_machineId;
	std::string m_np_serviceName;
	HWCollection* m_hw;
	std::vector<t_ntcs_hwpolicy_item> m_t_ntcs_hwpolicy_item_vector;
public:
	t_ntcs_hwpolicy();
	t_ntcs_hwpolicy(HWCollection* hw, std::string np_serviceName);
	~t_ntcs_hwpolicy();
	int Validate(); //0 is OK 
};

#endif
