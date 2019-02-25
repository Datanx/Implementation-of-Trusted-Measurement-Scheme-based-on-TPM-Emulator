/* 
 * File:   User.hpp
 * Author: jjdengjj
 *
 * Created on December 9, 2009, 4:51 PM
 */

#ifndef _USER_HPP
#define	_USER_HPP

#include <string>

class User {
public:
    User();
    User(const User& orig);
    virtual ~User();

    int getID();
    void setID(int);

    std::string getUSERNAME();
    void setUSERNAME(std::string);

    std::string getPASSWD();
    void setPASSWD(std::string);

    User& operator=(const User& orig);

private:
    int ID;
    std::string USERNAME;
    std::string PASSWD;

};

#endif	/* _USER_HPP */

