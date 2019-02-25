/* 
 * File:   CMSG.hpp
 * Author: jjdengjj
 *
 * Created on November 30, 2009, 4:47 PM
 */

#ifndef _CMSG_HPP
#define	_CMSG_HPP

#include <string>

class CMSG {
public:
    CMSG();
    CMSG(const CMSG& orig);
    virtual ~CMSG();
    CMSG& operator=(const CMSG&);
public:
    std::string id;
    std::string msg;

};

#endif	/* _CMSG_HPP */

