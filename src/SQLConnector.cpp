/*
 * File:   SQLConnector.cpp
 * Author: jjdengjj
 *
 * Created on December 1, 2009, 2:51 PM
 */

#include "SQLConnector.hpp"
#include "CPROW.hpp"
#include "SPROW.hpp"
#include "UROW.hpp"
#include "SWROW.hpp"

SQLConnector::SQLConnector() {
    this->Conn = mysqlpp::Connection(false);
    this->Init();
}

SQLConnector::~SQLConnector() {
    if(this->Status)
        this->Conn.disconnect();
}

void SQLConnector::Init()
{
    // Read the config file to init the connection to the database
	char* chDB = NULL;
	char* chSERVER = NULL;
	char* chUSER = NULL;
	char* chPASS = NULL;

	char path[256] = "/usr/local/NTCSSERVER";
	//getcwd(path, 256);
	strcat(path, "/.config");
	int fd = open(path, O_RDONLY);
	if(fd != -1)
	{
		char filebuf[4096] = {0};
		int filecount = read(fd, filebuf, 4096*sizeof(char));
		char procbuf[4096] = {0};
		int proccount = 0;
		int point = 0;
		for(int i = 0; i < filecount; i++)
		{
			if(filebuf[i] != '\n' && filebuf[i] != EOF)
			{
				procbuf[proccount] = filebuf[i];
				proccount++;
			}
			else
			{
				switch(point)
				{
					case 0:
						chDB = (char*)malloc((proccount+1)*sizeof(char));
						bzero(chDB, (proccount+1)*sizeof(char));
						memcpy(chDB, procbuf, proccount*sizeof(char));
						bzero(procbuf, 4096*sizeof(char));
						proccount = 0;
						point++;
						break;
					case 1:
						chSERVER = (char*)malloc((proccount+1)*sizeof(char));
						bzero(chSERVER, (proccount+1)*sizeof(char));
						memcpy(chSERVER, procbuf, proccount*sizeof(char));
						bzero(procbuf, 4096*sizeof(char));
						proccount = 0;
						point++;
						break;
					case 2:
						chUSER = (char*)malloc((proccount+1)*sizeof(char));
						bzero(chUSER, (proccount+1)*sizeof(char));
						memcpy(chUSER, procbuf, proccount*sizeof(char));
						bzero(procbuf, 4096*sizeof(char));
						proccount = 0;
						point++;
						break;
					case 3:
						chPASS = (char*)malloc((proccount+1)*sizeof(char));
						bzero(chPASS, (proccount+1)*sizeof(char));
						memcpy(chPASS, procbuf, proccount*sizeof(char));
						bzero(procbuf, 4096*sizeof(char));
						proccount = 0;
						point++;
						break;
					default:
						bzero(procbuf, 4096*sizeof(char));
						proccount = 0;
						break;
				}
			}
		}
		close(fd);
	}

    bzero(this->DB, 255);
    bzero(this->SERVER, 255);
    bzero(this->USER, 255);
    bzero(this->PASS, 255);

	if(chDB != NULL)
	{
    	strcpy(this->DB, chDB);
		free(chDB);
	}
	else
		strcpy(this->DB,"NTCSDB");

	if(chSERVER != NULL)
	{
		char* ip = NULL;
		char* port = NULL;
		char* p = chSERVER;
		int split = 0;
		int length = strlen(chSERVER);
		while(*p != '\0' && *p != ':')
		{
			split++;
			p++;
		}
		if(length-split == 0)
		{
			strcpy(this->SERVER, chSERVER);
			this->PORT = 3306;
		}
		else if(length-split > 1)
		{
			ip = (char*)malloc((split+1)*sizeof(char));
			bzero(ip, (split+1)*sizeof(char));
			memcpy(ip, chSERVER, split*sizeof(char));
			strcpy(this->SERVER, ip);
			free(ip);

			port = (char*)malloc((length-split)*sizeof(char));
			bzero(port, (length-split)*sizeof(char));
			memcpy(port, chSERVER+split+1, (length-split-1)*sizeof(char));
			this->PORT = atoi(port);
			free(port);
		}
		else
		{
			ip = (char*)malloc((split+1)*sizeof(char));
			bzero(ip, (split+1)*sizeof(char));
			memcpy(ip, chSERVER, split*sizeof(char));
			strcpy(this->SERVER, ip);
			free(ip);
			this->PORT = 3306;
		}
		free(chSERVER);
	}
	else
	{
    	strcpy(this->SERVER,"192.168.12.254");
		this->PORT = 3306;
	}

	if(chUSER != NULL)
    {
		strcpy(this->USER, chUSER);
		free(chUSER);
	}
	else
		strcpy(this->USER,"root");

	if(chPASS != NULL)
	{
    	strcpy(this->PASS, chPASS);
		free(chPASS);
	}
	else
		strcpy(this->PASS,"111111");

    if(this->Conn.connect(this->DB, this->SERVER, this->USER, this->PASS, this->PORT))
        this->Status = true;
    else
        this->Status = false;
}

std::vector<ClientPolicy> SQLConnector::GetAllClientPolicy()
{
    std::vector<ClientPolicy> VCP;
    if(this->Status)
    {
        std::string strsql = std::string("SELECT * FROM CLIENTPOLICY");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<CPROW> res;
        query.storein(res);
        for(int i = 0; i < res.size(); i++)
        {
            ClientPolicy CP;
            CP.setID(res[i].ID);
            CP.setPCSN(std::string(res[i].PCSN));
            CP.setMBSN(std::string(res[i].MBSN));
            CP.setBIOSSN(std::string(res[i].BIOSSN));
            CP.setCPUSN(std::string(res[i].CPUSN));
            CP.setHDDSN(std::string(res[i].HDDSN));
            CP.setHPOLICY(std::string(res[i].HPOLICY));
            CP.setSPOLICY(std::string(res[i].SPOLICY));

            char* charTMP = (char*)malloc(33*sizeof(char));
            bzero(charTMP, 33*sizeof(char));

            std::string strMEM = std::string(res[i].MEMSN);
            int LenMEM = strMEM.size();
            if(LenMEM != 0 && LenMEM % 32 == 0)
            {
                char* charMEM = (char*)malloc((LenMEM+1)*sizeof(char));
                bzero(charMEM, (LenMEM+1)*sizeof(char));
                memcpy(charMEM, strMEM.c_str(), LenMEM*sizeof(char));
                for(int j = 0; j < LenMEM / 32; j++)
                {
                    bzero(charTMP, 33*sizeof(char));
                    memcpy(charTMP, charMEM+(j*32), 32);
                    CP.addMEMSN(std::string(charTMP));
                }
                free(charMEM);
            }

            std::string strNIC = std::string(res[i].NICSN);
            int LenNIC = strNIC.size();
            if(LenNIC != 0 && LenNIC % 32 == 0)
            {
                char* charNIC = (char*)malloc((LenNIC+1)*sizeof(char));
                bzero(charNIC, (LenNIC+1)*sizeof(char));
                memcpy(charNIC, strNIC.c_str(), LenNIC*sizeof(char));
                for(int j = 0; j < LenNIC / 32; j++)
                {
                    bzero(charTMP, 33*sizeof(char));
                    memcpy(charTMP, charNIC+(j*32), 32);
                    CP.addNICSN(std::string(charTMP));
                }
                free(charNIC);
            }

            std::string strUSB = std::string(res[i].USBSN);
            int LenUSB = strUSB.size();
            if(LenUSB != 0 && LenUSB % 32 == 0)
            {
                char* charUSB = (char*)malloc((LenUSB+1)*sizeof(char));
                bzero(charUSB, (LenUSB+1)*sizeof(char));
                memcpy(charUSB, strUSB.c_str(), LenUSB*sizeof(char));
                for(int j = 0; j < LenNIC / 32; j++)
                {
                    bzero(charTMP, 33*sizeof(char));
                    memcpy(charTMP, charUSB+(j*32), 32);
                    CP.addUSBSN(std::string(charTMP));
                }
                free(charUSB);
            }

            free(charTMP);
            VCP.push_back(CP);
        }
    }
    return VCP;
}

ClientPolicy SQLConnector::GetClientPolicy(std::string key)
{
    ClientPolicy CP;
    if(this->Status)
    {
        std::string strsql = std::string("SELECT * FROM CLIENTPOLICY WHERE ") + std::string(QUERY_CLIENTPOLICY_KEY) + std::string(" = '") +key +std::string("'");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<CPROW> res;
        query.storein(res);
        if(res.size() == 1)
        {
                CP.setID(res[0].ID);
                CP.setPCSN(std::string(res[0].PCSN));
                CP.setMBSN(std::string(res[0].MBSN));
                CP.setBIOSSN(std::string(res[0].BIOSSN));
                CP.setCPUSN(std::string(res[0].CPUSN));
                CP.setHDDSN(std::string(res[0].HDDSN));
                CP.setHPOLICY(std::string(res[0].HPOLICY));
                CP.setSPOLICY(std::string(res[0].SPOLICY));

                char* charTMP = (char*)malloc(33*sizeof(char));
                bzero(charTMP, 33*sizeof(char));

                std::string strMEM = std::string(res[0].MEMSN);
                int LenMEM = strMEM.size();
                if(LenMEM != 0 && LenMEM % 32 == 0)
                {
                    char* charMEM = (char*)malloc((LenMEM+1)*sizeof(char));
                    bzero(charMEM, (LenMEM+1)*sizeof(char));
                    memcpy(charMEM, strMEM.c_str(), LenMEM*sizeof(char));
                    for(int i = 0; i < LenMEM / 32; i++)
                    {
                        bzero(charTMP, 33*sizeof(char));
                        memcpy(charTMP, charMEM+(i*32), 32);
                        CP.addMEMSN(std::string(charTMP));
                    }
                    free(charMEM);
                }

                std::string strNIC = std::string(res[0].NICSN);
                int LenNIC = strNIC.size();
                if(LenNIC != 0 && LenNIC % 32 == 0)
                {
                    char* charNIC = (char*)malloc((LenNIC+1)*sizeof(char));
                    bzero(charNIC, (LenNIC+1)*sizeof(char));
                    memcpy(charNIC, strNIC.c_str(), LenNIC*sizeof(char));
                    for(int i = 0; i < LenNIC / 32; i++)
                    {
                        bzero(charTMP, 33*sizeof(char));
                        memcpy(charTMP, charNIC+(i*32), 32);
                        CP.addNICSN(std::string(charTMP));
                    }
                    free(charNIC);
                }

                std::string strUSB = std::string(res[0].USBSN);
                int LenUSB = strUSB.size();
                if(LenUSB != 0 && LenUSB % 32 == 0)
                {
                    char* charUSB = (char*)malloc((LenUSB+1)*sizeof(char));
                    bzero(charUSB, (LenUSB+1)*sizeof(char));
                    memcpy(charUSB, strUSB.c_str(), LenUSB*sizeof(char));
                    for(int i = 0; i < LenUSB / 32; i++)
                    {
                        bzero(charTMP, 33*sizeof(char));
                        memcpy(charTMP, charUSB+(i*32), 32);
                        CP.addUSBSN(std::string(charTMP));
                    }
                    free(charUSB);
                }

                free(charTMP);
        }
    }
    return CP;
}

bool SQLConnector::SetClientPolicy(ClientPolicy cp)
{
    if(this->Status)
    {
        std::string strqry = std::string("SELECT * FROM CLIENTPOLICY WHERE MBSN = '") + cp.getMBSN() + std::string("'");
        mysqlpp::Query query = this->Conn.query(strqry);
        std::vector<CPROW> res;
        query.storein(res);
        if(res.size() == 0)
        {
            // Insert
            std::string strins = "INSERT INTO CLIENTPOLICY (PCSN, MBSN, BIOSSN, CPUSN, HDDSN, MEMSN, NICSN, USBSN, HPOLICY, SPOLICY) VALUES (";
            strins+= "'" + cp.getPCSN() + "', ";
            strins+= "'" + cp.getMBSN() + "', ";
            strins+= "'" + cp.getBIOSSN() + "', ";
            strins+= "'" + cp.getCPUSN() + "', ";
            strins+= "'" + cp.getHDDSN() + "', ";
            strins+= "'" + cp.getMEMSN() + "', ";
            strins+= "'" + cp.getNICSN() + "', ";
            strins+= "'" + cp.getUSBSN() + "', ";
            strins+= "'" + cp.getHPOLICY() + "', ";
            strins+= "'" + cp.getSPOLICY() + "')";
            return query.exec(strins);
        }
        else if(res.size() == 1)
        {
            // Update
            cp.setID(res[0].ID);
            std::string strupt = "UPDATE CLIENTPOLICY SET ";
            strupt+= "PCSN='" + cp.getPCSN() + "', ";
            strupt+= "MBSN='" + cp.getMBSN() + "', ";
            strupt+= "BIOSSN='" + cp.getBIOSSN() + "', ";
            strupt+= "CPUSN='" + cp.getCPUSN() + "', ";
            strupt+= "HDDSN='" + cp.getHDDSN() + "', ";
            strupt+= "MEMSN='" + cp.getMEMSN() + "', ";
            strupt+= "NICSN='" + cp.getNICSN() + "', ";
            strupt+= "USBSN='" + cp.getUSBSN() + "', ";
            strupt+= "HPOLICY='" + cp.getHPOLICY() + "', ";
            strupt+= "SPOLICY='" + cp.getSPOLICY() + "' ";
            char id[255] = {0};
            sprintf(id, "%d", cp.getID());
            strupt+= "WHERE ID=" + std::string(id);
            return query.exec(strupt);
        }
        else
            return false;
    }
	else
		return false;
}

bool SQLConnector::DelClientPolicy(std::string key)
{
    if(this->Status)
    {
        std::string strsql = std::string("DELETE FROM CLIENTPOLICY WHERE ") + std::string(QUERY_CLIENTPOLICY_KEY) + std::string(" = '") +key +std::string("'");
        mysqlpp::Query query = this->Conn.query();
        return query.exec(strsql);
    }
    else
        return false;
}

std::vector<ServerPolicy> SQLConnector::GetAllServerPolicy()
{
    std::vector<ServerPolicy> VSP;
    if(this->Status)
    {
        std::string strsql = std::string("SELECT * FROM SERVERPOLICY ");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<SPROW> res;
        query.storein(res);
        for(int i = 0; i < res.size(); i++)
        {
            ServerPolicy SP;
            SP.setID(res[i].ID);
            SP.setSERVICENAME(std::string(res[i].SERVICENAME));
            SP.setHPOLICY(std::string(res[i].HPOLICY));
            SP.setSPOLICY(std::string(res[i].SPOLICY));
            VSP.push_back(SP);
        }
    }
    return VSP;
}

ServerPolicy SQLConnector::GetServerPolicy(std::string key)
{
    ServerPolicy SP;
    if(this->Status)
    {
        std::string strsql = std::string("SELECT * FROM SERVERPOLICY WHERE ") + std::string(QUERY_SERVERPOLICY_KEY) + std::string(" = '") +key +std::string("'");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<SPROW> res;
        query.storein(res);
        if(res.size() == 1)
        {
            SP.setID(res[0].ID);
            SP.setSERVICENAME(std::string(res[0].SERVICENAME));
            SP.setHPOLICY(std::string(res[0].HPOLICY));
            SP.setSPOLICY(std::string(res[0].SPOLICY));
        }
    }
    return SP;
}

bool SQLConnector::SetServerPolicy(ServerPolicy sp)
{
    if(this->Status)
    {
        std::string strqry = std::string("SELECT * FROM SERVERPOLICY WHERE SERVICENAME = '") + sp.getSERVICENAME() + std::string("'");
        mysqlpp::Query query = this->Conn.query(strqry);
        std::vector<SPROW> res;
        query.storein(res);
        if(res.size() == 0)
        {
            //Insert
            std::string strins = "INSERT INTO SERVERPOLICY (SERVICENAME, HPOLICY, SPOLICY) VALUES (";
            strins+= "'" + sp.getSERVICENAME() + "', ";
            strins+= "'" + sp.getHPOLICY() + "', ";
            strins+= "'" + sp.getSPOLICY() + "')";
            return query.exec(strins);
        }
        else if(res.size() == 1)
        {
            //Update
            sp.setID(res[0].ID);
            std::string strupt = "UPDATE SERVERPOLICY SET ";
            strupt+= "SERVICENAME ='" + sp.getSERVICENAME() + "', ";
            strupt+= "HPOLICY ='" + sp.getHPOLICY() + "', ";
            strupt+= "SPOLICY ='" + sp.getSPOLICY() + "' ";
            char id[255] = {0};
            sprintf(id, "%d", sp.getID());
            strupt+= "WHERE ID=" + std::string(id);
            return query.exec(strupt);
        }
        else
            return false;
    }
	else
		return false;
}

bool SQLConnector::DelServerPolicy(std::string key)
{
    if(this->Status)
    {
        std::string strsql = std::string("DELETE FROM SERVERPOLICY WHERE ") + std::string(QUERY_SERVERPOLICY_KEY) + std::string(" = '") +key +std::string("'");
        mysqlpp::Query query = this->Conn.query();
        return query.exec(strsql);
    }
    else
        return false;
}

bool SQLConnector::LoginVLD(std::string USR, std::string PAW)
{
    if(this->Status)
    {
        std::string strsql = std::string("SELECT * FROM `USER` WHERE USERNAME = '") + USR +std::string("' AND PASSWD = MD5('") + PAW + std::string("')");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<UROW> res;
        query.storein(res);
        if(res.size() == 1)
            return true;
        else
            return false;
    }
    else
        return false;
}

std::vector<User> SQLConnector::GetAllUser()
{
	std::vector<User> VUSR;
	if(this->Status)
	{
        std::string strsql = std::string("SELECT * FROM `USER`");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<UROW> res;
        query.storein(res);
		for(int i = 0; i < res.size(); i++)
		{
			User usr;
			usr.setID(res[i].ID);
			usr.setUSERNAME(res[i].USERNAME);
			usr.setPASSWD(res[i].PASSWD);
			VUSR.push_back(usr);
		}
	}
	return VUSR;
}

User SQLConnector::GetUser(std::string name)
{
	User usr;
	if(this->Status)
	{
        std::string strsql = std::string("SELECT * FROM `USER` WHERE ") + std::string(QUERY_USER_KEY) + std::string(" = '") + name +std::string("'");
        mysqlpp::Query query = this->Conn.query(strsql.c_str());
        std::vector<UROW> res;
        query.storein(res);
		if(res.size() == 1)
		{
			usr.setID(res[0].ID);
			usr.setUSERNAME(res[0].USERNAME);
			usr.setPASSWD(res[0].PASSWD);
		}
	}
	return usr;
}

bool SQLConnector::SetUser(User usr)
{
	if(Status)
	{
		std::string strsql = std::string("SELECT * FROM `USER` WHERE ") + std::string(QUERY_USER_KEY) + std::string(" = '") + usr.getUSERNAME() +std::string("'");
		mysqlpp::Query query = this->Conn.query(strsql.c_str());
		std::vector<UROW> res;
		query.storein(res);
		if(res.size() == 0)
		{
			//Insert
			std::string strins = "INSERT INTO `USER` (USERNAME, PASSWD) VALUES (";
			strins += "'" + usr.getUSERNAME() + "', ";
			strins += "'" + usr.getPASSWD() + "')";
			return query.exec(strins);
		}
		else if(res.size() == 1)
		{
			//Update
			usr.setID(res[0].ID);
			std::string strupt = "UPDATE `USER` SET ";
			strupt += "USERNAME ='" + usr.getUSERNAME() + "', ";
			strupt += "PASSWD ='" + usr.getPASSWD() + "' ";
            char id[255] = {0};
            sprintf(id, "%d", usr.getID());
            strupt+= "WHERE ID=" + std::string(id);
            return query.exec(strupt);
		}
		else
			return false;
	}
	else
		return false;
}

bool SQLConnector::DelUser(std::string key)
{
    if(this->Status)
    {
        std::string strsql = std::string("DELETE FROM `USER` WHERE ") + std::string(QUERY_USER_KEY) + std::string(" = '") + key +std::string("'");
        mysqlpp::Query query = this->Conn.query();
        return query.exec(strsql);
    }
    else
        return false;
}

std::map<std::string, std::string> SQLConnector::GetAllSWInfo()
{
	std::map<std::string, std::string> SWMap;
	if(this->Status)
	{
	    std::string strsql = std::string("SELECT * FROM `SOFTWARE`");
	    mysqlpp::Query query = this->Conn.query(strsql.c_str());
	    std::vector<SWROW> res;
	    query.storein(res);
	    for(int i = 0; i < res.size(); i++)
	    {
		SWMap[res[i].NAME] = res[i].HASH;
	    }
	}
	return SWMap;
}

std::string SQLConnector::GetSWInfo(std::string name)
{
    std::string hash;
    hash = this->GetAllSWInfo()[name];
    return hash;
}

bool SQLConnector::DelSWInfo(std::string key)
{
    if(this->Status)
    {
        std::string strsql = std::string("DELETE FROM `SOFTWARE` WHERE ") + std::string(QUERY_SOFTWARE_KEY) + std::string(" = '") + key +std::string("'");
        mysqlpp::Query query = this->Conn.query();
        return query.exec(strsql);
    }
    else
        return false;
}

bool SQLConnector::SetSWInfo(std::string name, std::string hash)
{
    if(this->Status)
    {
	
    }
    else
	return false;
}

