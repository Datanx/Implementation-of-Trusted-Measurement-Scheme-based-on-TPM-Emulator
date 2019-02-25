/* 
 * File:   ServerPolicy.hpp
 * Author: jjdengjj
 *
 * Created on December 7, 2009, 4:18 PM
 */

#ifndef _SERVERPOLICY_HPP
#define	_SERVERPOLICY_HPP
#include <string.h>
#include <stdlib.h>
#include <string>
#include "POLICY.hpp"

class ServerPolicy {
public:
    ServerPolicy();
    ServerPolicy(const ServerPolicy& orig);
    virtual ~ServerPolicy();

    int getID();
    void setID(int);

    std::string getSERVICENAME();
    void setSERVICENAME(std::string);

    std::string getHPOLICY();
    bool getHPOLICY(int);
    void setHPOLICY(std::string);
    void setHPOLICY(int);

    std::string getSPOLICY();
    bool getSPOLICY(int);
    void setSPOLICY(std::string);
    void setSPOLICY(int);

    ServerPolicy& operator=(const ServerPolicy& orig);

private:
    int ID;
    std::string SERVICENAME;
    std::string HPOLICY;
    std::string SPOLICY;
};

#endif	/* _SERVERPOLICY_HPP */

