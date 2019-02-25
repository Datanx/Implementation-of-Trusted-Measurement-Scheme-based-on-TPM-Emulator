/* 
 * File:   PolicyClient.cpp
 * Author: jjdengjj
 * 
 * Created on December 1, 2009, 2:54 PM
 */

#include "ClientPolicy.hpp"

ClientPolicy::ClientPolicy() {
    this->ID = -1;
}

ClientPolicy::ClientPolicy(const ClientPolicy& orig) {
    this->BIOSSN = orig.BIOSSN;
    this->CPUSN = orig.CPUSN;
    this->HPOLICY = orig.HPOLICY;
    this->HDDSN = orig.HDDSN;
    this->ID = orig.ID;
    this->MBSN = orig.MBSN;
    this->MEMSN = orig.MEMSN;
    this->NICSN = orig.NICSN;
    this->PCSN = orig.PCSN;
    this->SPOLICY = orig.SPOLICY;
    this->USBSN = orig.USBSN;
}

ClientPolicy::~ClientPolicy() {
}

int ClientPolicy::getID()
{
    return this->ID;
}

void ClientPolicy::setID(int id)
{
    this->ID = id;
}

std::string ClientPolicy::getMBSN()
{
    return this->MBSN;
}

void ClientPolicy::setMBSN(std::string sn)
{
    this->MBSN = sn;
}

std::string ClientPolicy::getBIOSSN()
{
    return this->BIOSSN;
}

void ClientPolicy::setBIOSSN(std::string sn)
{
    this->BIOSSN = sn;
}

std::string ClientPolicy::getCPUSN()
{
    return this->CPUSN;
}

void ClientPolicy::setCPUSN(std::string sn)
{
    this->CPUSN = sn;
}

std::string ClientPolicy::getHDDSN()
{
    return this->HDDSN;
}

void ClientPolicy::setHDDSN(std::string sn)
{
    this->HDDSN = sn;
}

std::string ClientPolicy::getPCSN()
{
    return this->PCSN;
}

void ClientPolicy::setPCSN(std::string sn)
{
    this->PCSN = sn;
}

int ClientPolicy::getMEMSNNum()
{
    return this->MEMSN.size();
}

std::string ClientPolicy::getMEMSN()
{
    std::string sn;
    std::vector<std::string>::iterator it = this->MEMSN.begin();
    while(it != this->MEMSN.end())
    {
        sn += *it;
        it++;
    }
    return sn;
}

std::string ClientPolicy::getMEMSN(int count)
{
    if(count >= this->MEMSN.size())
        return std::string("");
    else
        return this->MEMSN[count];
}

void ClientPolicy::setMEMSN(int count, std::string sn)
{
    if(count >= this->MEMSN.size())
        return;
    else
        this->MEMSN[count] = sn;
}

void ClientPolicy::addMEMSN(std::string sn)
{
    this->MEMSN.push_back(sn);
}

void ClientPolicy::delMEMSN(int count)
{
    if(count >= this->MEMSN.size())
        return;
    else
    {
        std::vector<std::string>::iterator it = this->MEMSN.begin();
        for(int i = 0; i < count; i++)
            it++;
        this->MEMSN.erase(it);
    }
}

bool ClientPolicy::findMEMSN(std::string sn)
{
	std::vector<std::string>::iterator it = this->MEMSN.begin();
	while(it != this->MEMSN.end())
	{
		if(*it == sn)
			return true;
		it++;
	}
	return false;
}

int ClientPolicy::getNICSNNum()
{
    return this->NICSN.size();
}

std::string ClientPolicy::getNICSN()
{
    std::string sn;
    std::vector<std::string>::iterator it = this->NICSN.begin();
    while(it != this->NICSN.end())
    {
        sn += *it;
        it++;
    }
    return sn;
}

std::string ClientPolicy::getNICSN(int count)
{
    if(count >= this->NICSN.size())
        return std::string("");
    else
        return this->NICSN[count];
}

void ClientPolicy::setNICSN(int count, std::string sn)
{
    if(count >= this->NICSN.size())
        return;
    else
        this->NICSN[count] = sn;
}

void ClientPolicy::addNICSN(std::string sn)
{
    this->NICSN.push_back(sn);
}

void ClientPolicy::delNICSN(int count)
{
    if(count >= this->NICSN.size())
        return;
    else
    {
        std::vector<std::string>::iterator it = this->NICSN.begin();
        for(int i = 0; i < count; i++)
            it++;
        this->NICSN.erase(it);
    }
}

bool ClientPolicy::findNICSN(std::string sn)
{
	std::vector<std::string>::iterator it = this->NICSN.begin();
	while(it != this->NICSN.end())
	{
		if(*it == sn)
			return true;
		it++;
	}
	return false;
}

int ClientPolicy::getUSBSNNum()
{
    return this->USBSN.size();
}

std::string ClientPolicy::getUSBSN()
{
    std::string sn;
    std::vector<std::string>::iterator it = this->USBSN.begin();
    while(it != this->USBSN.end())
    {
        sn += *it;
        it++;
    }
    return sn;
}

std::string ClientPolicy::getUSBSN(int count)
{
    if(count >= this->USBSN.size())
        return std::string("");
    else
        return this->USBSN[count];
}

void ClientPolicy::setUSBSN(int count, std::string sn)
{
    if(count >= this->USBSN.size())
        return;
    else
        this->USBSN[count] = sn;
}

void ClientPolicy::addUSBSN(std::string sn)
{
    this->USBSN.push_back(sn);
}

void ClientPolicy::delUSBSN(int count)
{
    if(count >= this->USBSN.size())
        return;
    else
    {
        std::vector<std::string>::iterator it = this->USBSN.begin();
        for(int i = 0; i < count; i++)
            it++;
        this->USBSN.erase(it);
    }
}

bool ClientPolicy::findUSBSN(std::string sn)
{
	std::vector<std::string>::iterator it = this->USBSN.begin();
	while(it != this->USBSN.end())
	{
		if(*it == sn)
			return true;
		it++;
	}
	return false;
}

std::string ClientPolicy::getHPOLICY()
{
    return this->HPOLICY;
}

bool ClientPolicy::getHPOLICY(int bit)
{
    char HP[11] = {0};
    memcpy(HP, this->HPOLICY.c_str(), 11*sizeof(char));
    if(HP[bit] == '1')
        return true;
    else
        return false;
}

void ClientPolicy::setHPOLICY(std::string sn)
{
    this->HPOLICY = sn;
}

void ClientPolicy::setHPOLICY(int bit)
{
    char* HP = (char*)malloc(12*sizeof(char));
    memcpy(HP, this->HPOLICY.c_str(), 11*sizeof(char));
    *(HP+bit) = '1';
    this->HPOLICY = std::string(HP);
    free(HP);
}

std::string ClientPolicy::getSPOLICY()
{
    return this->SPOLICY;
}

void ClientPolicy::setSPOLICY(std::string sn)
{
    this->SPOLICY = sn;
}

bool ClientPolicy::getSPOLICY(int bit)
{
    return true;
}

void ClientPolicy::setSPOLICY(int bit)
{
    
}

ClientPolicy& ClientPolicy::operator =(const ClientPolicy& orig)
{
    this->BIOSSN = orig.BIOSSN;
    this->CPUSN = orig.CPUSN;
    this->HPOLICY = orig.HPOLICY;
    this->HDDSN = orig.HDDSN;
    this->ID = orig.ID;
    this->MBSN = orig.MBSN;
    this->MEMSN = orig.MEMSN;
    this->NICSN = orig.NICSN;
    this->PCSN = orig.PCSN;
    this->SPOLICY = orig.SPOLICY;
    this->USBSN = orig.USBSN;
}
