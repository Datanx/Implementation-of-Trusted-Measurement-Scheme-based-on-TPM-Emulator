#include "t_ntcs_hwpolicy.hpp"
#include "t_ntcs_swpolicy_row.hpp"
#include "t_ntcs_hwpolicy_row.hpp"

t_ntcs_hwpolicy::t_ntcs_hwpolicy()
{
}

t_ntcs_hwpolicy::t_ntcs_hwpolicy(HWCollection* hw, std::string np_serviceName)
{
	m_np_machineId = std::string(hw->getBIOS().md5_hash);
	m_np_serviceName = np_serviceName;
	m_hw = hw;
	mysqlpp::Connection Conn(false);
	if(Conn.connect(DBNAME, HOST, USER, PWD))
	{
		//Query HWPolicy from Database
		std::string strsql = std::string("select * from t_ntcs_hwpolicy where np_machineId = '") + m_np_machineId + 
					std::string("' and np_serviceName = '") + m_np_serviceName + std::string("'");
		mysqlpp::Query query = Conn.query(strsql.c_str());
		std::vector<t_ntcs_hwpolicy_row> reshw;
		query.storein(reshw);
		std::vector<t_ntcs_hwpolicy_row>::iterator itor;
		for(itor = reshw.begin(); itor != reshw.end(); itor++)
		{
			t_ntcs_hwpolicy_item item;
			item.np_autoId = (*itor).np_autoId;
			item.np_domainName = (*itor).np_domainName;
			item.np_machineId = (*itor).np_machineId;
			item.np_serviceName = (*itor).np_serviceName;
			item.np_policyStr = (*itor).np_policyStr;
			item.np_identStr = (*itor).np_identStr;
			item.np_swIdentStr = (*itor).np_swIdentStr;
			item.np_hashPc = (*itor).np_hashPc;
			item.np_hashBios = (*itor).np_hashBios;
			item.np_hashMb = (*itor).np_hashMb;
			item.np_hashCpu = (*itor).np_hashCpu;
			item.np_hashMem = (*itor).np_hashMem;
			item.np_hashHdd = (*itor).np_hashHdd;
			item.np_hashUsb = (*itor).np_hashUsb;
			item.np_hashNic = (*itor).np_hashNic;
			item.np_Reserve1 = (*itor).np_Reserve1;
			item.np_Reserve2 = (*itor).np_Reserve2;
			item.np_Reserve3 = (*itor).np_Reserve3;
			item.np_Reserve4 = (*itor).np_Reserve4;
			item.np_Reserve5 = (*itor).np_Reserve5;
			item.np_Reserve6 = (*itor).np_Reserve6;
			item.np_addition = (*itor).np_addition;
			m_t_ntcs_hwpolicy_item_vector.push_back(item);
		}
		
		Conn.disconnect();
	}
}

t_ntcs_hwpolicy::~t_ntcs_hwpolicy()
{
	m_hw = NULL;
}

int t_ntcs_hwpolicy::Validate()
{
	int rt = 0;
	std::vector<t_ntcs_hwpolicy_item>::iterator itor;
	for(itor = m_t_ntcs_hwpolicy_item_vector.begin(); itor != m_t_ntcs_hwpolicy_item_vector.end();itor++)
	{
		rt = 0;
		char tmp[69] = {0};
		memcpy(tmp, (*itor).np_policyStr.c_str(), 69);
		
		//Validate PC Info
		if(rt == 0)
		{
		switch(tmp[PLCY_POS_PC]-'0')
		{
			case PLCY_CHKTYPE_IGNORE:
				break;
			case PLCY_CHKTYPE_CHECK:
				if(strcmp(m_hw->getPC().md5_hash, (*itor).np_hashPc.c_str()) != 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_CHECKNOT:
				if(strcmp(m_hw->getPC().md5_hash, (*itor).np_hashPc.c_str()) == 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_UNDEF:
				break;
		}
		}
		
		//Validate MB Info
		if(rt == 0)
		{
		switch(tmp[PLCY_POS_MB]-'0')
		{
			case PLCY_CHKTYPE_IGNORE:
				break;
			case PLCY_CHKTYPE_CHECK:
				if(strcmp(m_hw->getMB().md5_hash, (*itor).np_hashMb.c_str()) != 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_CHECKNOT:
				if(strcmp(m_hw->getMB().md5_hash, (*itor).np_hashMb.c_str()) == 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_UNDEF:
				break;
		}
		}
		
		//Validate BIOS Info
		if(rt == 0)
		{
		switch(tmp[PLCY_POS_BIOS]-'0')
		{
			case PLCY_CHKTYPE_IGNORE:
				break;
			case PLCY_CHKTYPE_CHECK:
				if(strcmp(m_hw->getBIOS().md5_hash, (*itor).np_hashBios.c_str()) != 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_CHECKNOT:
				if(strcmp(m_hw->getBIOS().md5_hash, (*itor).np_hashBios.c_str()) == 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_UNDEF:
				break;
		}
		}
		
		//Validate CPU Info
		if(rt == 0)
		{
		switch(tmp[PLCY_POS_CPU]-'0')
		{
			case PLCY_CHKTYPE_IGNORE:
				break;
			case PLCY_CHKTYPE_CHECK:
				if(strcmp(m_hw->getCPU().md5_hash, (*itor).np_hashCpu.c_str()) != 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_CHECKNOT:
				if(strcmp(m_hw->getCPU().md5_hash, (*itor).np_hashCpu.c_str()) == 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_UNDEF:
				break;
		}
		}
		
		//Validate HDD Info
		if(rt == 0)
		{
		switch(tmp[PLCY_POS_HDD]-'0')
		{
			case PLCY_CHKTYPE_IGNORE:
				break;
			case PLCY_CHKTYPE_CHECK:
				if(strcmp(m_hw->getHDD().md5_hash, (*itor).np_hashHdd.c_str()) != 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_CHECKNOT:
				if(strcmp(m_hw->getHDD().md5_hash, (*itor).np_hashHdd.c_str()) == 0)
					rt = 1;
				break;
			case PLCY_CHKTYPE_UNDEF:
				break;
		}
		}
		
		//Validate MEM Info
		if(rt == 0)
		{
		
		char* MEM[10] = {NULL};
		int count = (*itor).np_hashMem.length() / 32;
		for(int i = 0; i < count; i++)
		{
			MEM[i] = (char*)malloc(33*sizeof(char));
			memset(MEM[i], 0, 33);
			memcpy(MEM[i], (*itor).np_hashMem.c_str()+i*32*sizeof(char), 32);
		}
		for(int i = 0; i < count; i++)
		{
			switch(tmp[PLCY_POS_MEM+i]-'0')
			{
				case PLCY_CHKTYPE_IGNORE:
					break;
				case PLCY_CHKTYPE_CHECK:
					if((!m_hw->findMEMSN(std::string(MEM[i]))))
						rt = 1;
					break;
				case PLCY_CHKTYPE_CHECKNOT:
					if(m_hw->findMEMSN(std::string(MEM[i])))
						rt = 1;
					break;
				case PLCY_CHKTYPE_UNDEF:
					break;
			}
			if(rt != 0)
				break;
		}		
		for(int i = 0; i < count; i++)
		{
			if(MEM[i] != NULL)
				free(MEM[i]);
		}		
		}
		
		//Validate USB Info
		if(rt == 0)
		{
		
		char* USB[20] = {NULL};
		int count = (*itor).np_hashUsb.length() / 32;
		for(int i = 0; i < count; i++)
		{
			USB[i] = (char*)malloc(33*sizeof(char));
			memset(USB[i], 0, 33);
			memcpy(USB[i], (*itor).np_hashUsb.c_str()+i*32*sizeof(char),32);
		}
		for(int i = 0; i < count; i++)
		{
			switch(tmp[PLCY_POS_USB+i]-'0')
			{
				case PLCY_CHKTYPE_IGNORE:
					break;
				case PLCY_CHKTYPE_CHECK:
					if((!m_hw->findUSBSN(std::string(USB[i]))))
						rt = 1;
					break;
				case PLCY_CHKTYPE_CHECKNOT:
					if(m_hw->findUSBSN(std::string(USB[i])))
						rt = 1;
					break;
				case PLCY_CHKTYPE_UNDEF:
					break;
			}
			if(rt != 0)
				break;
		}
		for(int i = 0; i < count; i++)
		{
			if(USB[i] != NULL)
				free(USB[i]);
		}		
		}
		
		//Validate NIC Info
		if(rt == 0)
		{
		
		char* NIC[10] = {NULL};
		int count = (*itor).np_hashNic.length() / 32;
		for(int i = 0; i < count; i++)
		{
			NIC[i] = (char*)malloc(33*sizeof(char));
			memset(NIC[i], 0, 33);
			memcpy(NIC[i], (*itor).np_hashNic.c_str()+i*32*sizeof(char),32);
		}
		for(int i = 0; i < count; i++)
		{
			switch(tmp[PLCY_POS_NIC+i]-'0')
			{
				case PLCY_CHKTYPE_IGNORE:
					break;
				case PLCY_CHKTYPE_CHECK:
					if((!m_hw->findNICSN(std::string(NIC[i]))))
						rt = 1;
					break;
				case PLCY_CHKTYPE_CHECKNOT:
					if(m_hw->findNICSN(std::string(NIC[i])))
						rt = 1;
					break;
				case PLCY_CHKTYPE_UNDEF:
					break;
			}
			if(rt != 0)
				break;
		}	
		for(int i = 0; i < count; i++)
		{
			if(NIC[i] != NULL)
				free(NIC[i]);
		}		
		}
		
		//Validate SWPolicy
		
		//Query SWPolicy from Database
		mysqlpp::Connection Conn(false);
		if(Conn.connect(DBNAME, HOST, USER, PWD))
		{
			std::string strsql = std::string("select nswp_autoId,nswp_identStr,nswp_Kernel,nswp_Gcc,nswp_Bash,nswp_Openoffice,nswp_Firefox,nswp_description from t_ntcs_swpolicy where nswp_autoId = ") + (*itor).np_swIdentStr;
			mysqlpp::Query query = Conn.query(strsql.c_str());
			std::vector<t_ntcs_swpolicy_row> ressw;
			query.storein(ressw);
			if(ressw.size() == 1)
			{
				char trim[256] = {0};
				
				//Validate Kernel Info
				memset(trim, 0, 256);
				switch(tmp[PLCY_POS_KERNEL]-'0')
				{
					case PLCY_CHKTYPE_IGNORE:
						break;
					case PLCY_CHKTYPE_CHECK:
						RTrim(m_hw->getKERNEL().version, strlen(m_hw->getKERNEL().version), trim);
						if(ressw[0].nswp_Kernel.compare(std::string(trim)) != 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_CHECKNOT:
						RTrim(m_hw->getKERNEL().version, strlen(m_hw->getKERNEL().version), trim);
						if(ressw[0].nswp_Kernel.compare(std::string(trim)) == 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_UNDEF:
						break;
				}
				
				//Validate Gcc Info
				memset(trim, 0, 256);
				switch(tmp[PLCY_POS_GCC]-'0')
				{
					case PLCY_CHKTYPE_IGNORE:
						break;
					case PLCY_CHKTYPE_CHECK:
						RTrim(m_hw->getGCC().version, strlen(m_hw->getGCC().version), trim);
						if(ressw[0].nswp_Gcc.compare(std::string(trim)) != 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_CHECKNOT:
						RTrim(m_hw->getGCC().version, strlen(m_hw->getGCC().version), trim);
						if(ressw[0].nswp_Gcc.compare(std::string(trim)) == 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_UNDEF:
						break;
				}
				
				//Validate Bash Info
				memset(trim, 0, 256);
				switch(tmp[PLCY_POS_BASH]-'0')
				{
					case PLCY_CHKTYPE_IGNORE:
						break;
					case PLCY_CHKTYPE_CHECK:
						RTrim(m_hw->getBASH().version, strlen(m_hw->getBASH().version), trim);
						if(ressw[0].nswp_Bash.compare(std::string(trim)) != 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_CHECKNOT:
						RTrim(m_hw->getBASH().version, strlen(m_hw->getBASH().version), trim);
						if(ressw[0].nswp_Bash.compare(std::string(trim)) == 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_UNDEF:
						break;
				}
				
				//Validate OpenOffice Info
				memset(trim, 0, 256);
				switch(tmp[PLCY_POS_OPENOFFICE]-'0')
				{
					case PLCY_CHKTYPE_IGNORE:
						break;
					case PLCY_CHKTYPE_CHECK:
						RTrim(m_hw->getOPENOFFICE().version, strlen(m_hw->getOPENOFFICE().version), trim);
						if(ressw[0].nswp_Openoffice.compare(std::string(trim)) != 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_CHECKNOT:
						RTrim(m_hw->getOPENOFFICE().version, strlen(m_hw->getOPENOFFICE().version), trim);
						if(ressw[0].nswp_Openoffice.compare(std::string(trim)) == 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_UNDEF:
						break;
				}
				
				//Validate Firefox Info
				memset(trim, 0, 256);
				switch(tmp[PLCY_POS_FIREFOX]-'0')
				{
					case PLCY_CHKTYPE_IGNORE:
						break;
					case PLCY_CHKTYPE_CHECK:
						RTrim(m_hw->getFIREFOX().version, strlen(m_hw->getFIREFOX().version), trim);
						if(ressw[0].nswp_Firefox.compare(std::string(trim)) != 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_CHECKNOT:
						RTrim(m_hw->getFIREFOX().version, strlen(m_hw->getFIREFOX().version), trim);
						if(ressw[0].nswp_Firefox.compare(std::string(trim)) == 0)
							rt = 1;
						break;
					case PLCY_CHKTYPE_UNDEF:
						break;
				}
			}
			else
				rt == 1;
			Conn.disconnect();
		}
		else
			rt == 1;
		
		if(rt == 0)
			break;
	}
	
	return rt;
}
