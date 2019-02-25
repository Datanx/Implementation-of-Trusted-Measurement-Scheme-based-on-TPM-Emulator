/* 
 * File:   CMSG.cpp
 * Author: jjdengjj
 * 
 * Created on November 30, 2009, 4:47 PM
 */

#include "CMSG.hpp"

CMSG::CMSG() {
}

CMSG::CMSG(const CMSG& orig) {
    this->id = orig.id;
    this->msg = orig.msg;
}

CMSG::~CMSG() {
}

CMSG& CMSG::operator =(const CMSG& orig)
{
    this->id = orig.id;
    this->msg = orig.msg;
}
