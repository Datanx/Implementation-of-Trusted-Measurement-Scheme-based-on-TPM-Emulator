/* 
 * File:   User.cpp
 * Author: jjdengjj
 * 
 * Created on December 9, 2009, 4:51 PM
 */

#include "User.hpp"

User::User() {
	this->ID = -1;
}

User::User(const User& orig) {
    this->ID = orig.ID;
    this->USERNAME = orig.USERNAME;
    this->PASSWD = orig.PASSWD;
}

User::~User() {
}

int User::getID()
{
    return this->ID;
}

void User::setID(int id)
{
    this->ID = id;
}

std::string User::getUSERNAME()
{
    return this->USERNAME;
}

void User::setUSERNAME(std::string usr)
{
    this->USERNAME = usr;
}

std::string User::getPASSWD()
{
    return this->PASSWD;
}

void User::setPASSWD(std::string pwd)
{
    this->PASSWD = pwd;
}

User& User::operator =(const User& orig)
{
    this->ID = orig.ID;
    this->USERNAME = orig.USERNAME;
    this->PASSWD = orig.PASSWD;
}
