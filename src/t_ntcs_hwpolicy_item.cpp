#include "t_ntcs_hwpolicy_item.hpp"

t_ntcs_hwpolicy_item::t_ntcs_hwpolicy_item(const t_ntcs_hwpolicy_item& orig)
{
	np_domainName = orig.np_domainName;
	np_machineId = orig.np_machineId;
	np_serviceName = orig.np_serviceName;
	np_policyStr = orig.np_policyStr;
	np_identStr = orig.np_identStr;
	np_swIdentStr = orig.np_swIdentStr;
	np_hashPc = orig.np_hashPc;
	np_hashBios = orig.np_hashBios;
	np_hashMb = orig.np_hashMb;
	np_hashCpu = orig.np_hashCpu;
	np_hashMem = orig.np_hashMem;
	np_hashHdd = orig.np_hashHdd;
	np_hashUsb = orig.np_hashUsb;
	np_hashNic = orig.np_hashNic;
	np_Reserve1 = orig.np_Reserve1;
	np_Reserve2 = orig.np_Reserve2;
	np_Reserve3 = orig.np_Reserve3;
	np_Reserve4 = orig.np_Reserve4;
	np_Reserve5 = orig.np_Reserve5;
	np_Reserve6 = orig.np_Reserve6;
	np_addition = orig.np_addition;
}

t_ntcs_hwpolicy_item& t_ntcs_hwpolicy_item::operator=(const t_ntcs_hwpolicy_item& orig)
{
	np_domainName = orig.np_domainName;
	np_machineId = orig.np_machineId;
	np_serviceName = orig.np_serviceName;
	np_policyStr = orig.np_policyStr;
	np_identStr = orig.np_identStr;
	np_swIdentStr = orig.np_swIdentStr;
	np_hashPc = orig.np_hashPc;
	np_hashBios = orig.np_hashBios;
	np_hashMb = orig.np_hashMb;
	np_hashCpu = orig.np_hashCpu;
	np_hashMem = orig.np_hashMem;
	np_hashHdd = orig.np_hashHdd;
	np_hashUsb = orig.np_hashUsb;
	np_hashNic = orig.np_hashNic;
	np_Reserve1 = orig.np_Reserve1;
	np_Reserve2 = orig.np_Reserve2;
	np_Reserve3 = orig.np_Reserve3;
	np_Reserve4 = orig.np_Reserve4;
	np_Reserve5 = orig.np_Reserve5;
	np_Reserve6 = orig.np_Reserve6;
	np_addition = orig.np_addition;
}

t_ntcs_hwpolicy_item::t_ntcs_hwpolicy_item()
{
}

t_ntcs_hwpolicy_item::~t_ntcs_hwpolicy_item()
{
}
