/* 
 * File:   PolicyClient.hpp
 * Author: jjdengjj
 *
 * Created on December 1, 2009, 2:54 PM
 */

#ifndef _POLICYCLIENT_HPP

#include <string.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include "POLICY.hpp"

class ClientPolicy {
public:
    ClientPolicy();
    ClientPolicy(const ClientPolicy& orig);
    virtual ~ClientPolicy();
    
    int getID();
    void setID(int);

    std::string getMBSN();
    void setMBSN(std::string);

    std::string getBIOSSN();
    void setBIOSSN(std::string);

    std::string getCPUSN();
    void setCPUSN(std::string);

    std::string getHDDSN();
    void setHDDSN(std::string);

    std::string getPCSN();
    void setPCSN(std::string);

    int getMEMSNNum();
    std::string getMEMSN();
    std::string getMEMSN(int count);
    void setMEMSN(int count, std::string);
    void addMEMSN(std::string);
    void delMEMSN(int count);
	bool findMEMSN(std::string);

    int getNICSNNum();
    std::string getNICSN();
    std::string getNICSN(int count);
    void setNICSN(int count, std::string);
    void addNICSN(std::string);
    void delNICSN(int count);
	bool findNICSN(std::string);

    int getUSBSNNum();
    std::string getUSBSN();
    std::string getUSBSN(int count);
    void setUSBSN(int count, std::string);
    void addUSBSN(std::string);
    void delUSBSN(int count);
	bool findUSBSN(std::string);

    std::string getHPOLICY();
    bool getHPOLICY(int);
    void setHPOLICY(std::string);
    void setHPOLICY(int);


    std::string getSPOLICY();
    bool getSPOLICY(int);
    void setSPOLICY(std::string);
    void setSPOLICY(int);


    ClientPolicy& operator=(const ClientPolicy&);

private:
    int ID;
    std::string MBSN;
    std::string BIOSSN;
    std::string CPUSN;
    std::string HDDSN;
    std::string PCSN;
    std::vector<std::string> MEMSN;
    std::vector<std::string> NICSN;
    std::vector<std::string> USBSN;
    std::string HPOLICY;
    std::string SPOLICY;
    
};

#endif	/* _POLICYCLIENT_HPP */
