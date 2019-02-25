/* 
 * File:   ServerPolicy.cpp
 * Author: jjdengjj
 * 
 * Created on December 7, 2009, 4:18 PM
 */

#include "ServerPolicy.hpp"

ServerPolicy::ServerPolicy() {
    this->ID = -1;
}

ServerPolicy::ServerPolicy(const ServerPolicy& orig) {
    this->ID = orig.ID;
    this->SERVICENAME = orig.SERVICENAME;
    this->HPOLICY = orig.HPOLICY;
    this->SPOLICY = orig.SPOLICY;
}

ServerPolicy::~ServerPolicy() {
}

ServerPolicy& ServerPolicy::operator =(const ServerPolicy& orig)
{
    this->ID = orig.ID;
    this->SERVICENAME = orig.SERVICENAME;
    this->HPOLICY = orig.HPOLICY;
    this->SPOLICY = orig.SPOLICY;
}

int ServerPolicy::getID()
{
    return this->ID;
}

void ServerPolicy::setID(int id)
{
    this->ID = id;
}

std::string ServerPolicy::getSERVICENAME()
{
    return this->SERVICENAME;
}

void ServerPolicy::setSERVICENAME(std::string sn)
{
    this->SERVICENAME = sn;
}

std::string ServerPolicy::getHPOLICY()
{
    return this->HPOLICY;
}

bool ServerPolicy::getHPOLICY(int bit)
{
    char HPOLICY[11] = {0};
    memcpy(HPOLICY, this->HPOLICY.c_str(), 11*sizeof(char));
    if(HPOLICY[bit] == '1')
        return true;
    else
        return false;
}

void ServerPolicy::setHPOLICY(std::string hp)
{
    this->HPOLICY = hp;
}

void ServerPolicy::setHPOLICY(int bit)
{
    char* HP = (char*)malloc(12*sizeof(char));
    memcpy(HP, this->HPOLICY.c_str(), 11*sizeof(char));
    *(HP+bit) = '1';
    this->HPOLICY = std::string(HP);
    free(HP);
}

std::string ServerPolicy::getSPOLICY()
{
    return this->SPOLICY;
}

bool ServerPolicy::getSPOLICY(int bit)
{
        return true;
}

void ServerPolicy::setSPOLICY(std::string sp)
{
    this->SPOLICY = sp;
}

void ServerPolicy::setSPOLICY(int bit)
{
    
}
