/*
 * File:   SQLConnector.hpp
 * Author: jjdengjj
 *
 * Created on December 1, 2009, 2:51 PM
 */

#ifndef _SQLCONNECTOR_HPP
#define	_SQLCONNECTOR_HPP

#define QUERY_CLIENTPOLICY_KEY "MBSN"
#define QUERY_SERVERPOLICY_KEY "SERVICENAME"
#define QUERY_USER_KEY "USERNAME"
#define QUERY_SOFTWARE_KEY "NAME"

#include <mysql++.h>
#include <ssqls.h>
#include <vector>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <iterator>
#include "ClientPolicy.hpp"
#include "ServerPolicy.hpp"
#include "User.hpp"

class SQLConnector {
public:
    SQLConnector();
    virtual ~SQLConnector();

    std::vector<ClientPolicy> GetAllClientPolicy();
    ClientPolicy GetClientPolicy(std::string);
    bool SetClientPolicy(ClientPolicy);
    bool DelClientPolicy(std::string);

    std::vector<ServerPolicy> GetAllServerPolicy();
    ServerPolicy GetServerPolicy(std::string);
    bool SetServerPolicy(ServerPolicy);
    bool DelServerPolicy(std::string);

    bool LoginVLD(std::string, std::string);
    std::vector<User> GetAllUser();
    User GetUser(std::string);
    bool SetUser(User);
    bool DelUser(std::string);

    std::map<std::string, std::string> GetAllSWInfo();
    std::string GetSWInfo(std::string);
    bool SetSWInfo(std::string, std::string);
    bool DelSWInfo(std::string);

private:
    bool Status;
    char DB[255];
    char SERVER[255];
    char USER[255];
    char PASS[255];
    unsigned int PORT;
    mysqlpp::Connection Conn;
    void Init();

};

#endif	/* _SQLCONNECTOR_HPP */

