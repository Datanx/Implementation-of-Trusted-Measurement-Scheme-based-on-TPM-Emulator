/* 
 * File:   NCSFConnector.hpp
 * Author: jjdengjj
 *
 * Created on November 29, 2009, 9:20 PM
 */

#ifndef _NCSFCONNECTOR_HPP
#define	_NCSFCONNECTOR_HPP

#ifndef _NCSFRAMEWORK_SEVER_IP
#define _NCSFRAMEWORK_SEVER_IP "192.168.12.254"
#endif

#include <unistd.h>
#include <string>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "NTCFPACKAGECODE.hpp"
#include "NTCFPackage.hpp"
#include "MainServer.hpp"

class NTCFConnector {
public:
    NTCFConnector();
    NTCFConnector(const NTCFConnector& orig);
    virtual ~NTCFConnector();

    static std::string Validate(std::string);
    static bool getServerIP(char**);

};

#endif	/* _NCSFCONNECTOR_HPP */

