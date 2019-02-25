/* 
 * File:   HWCollection.hpp
 * Author: jjdengjj
 *
 * Created on November 27, 2009, 1:47 PM
 */

#ifndef _HWCOLLECTION_HPP
#define	_HWCOLLECTION_HPP

#include <list>
#include "AgentConnector.hpp"

class HWCollection {
    
public:
    HWCollection();
    HWCollection(const char*);
    HWCollection(const HWCollection& orig);
    ~HWCollection();

public:
    bool Update();

    std::string getID();
    void printALL();

    CPU_INFO& getCPU();
    void printCPU();

    HDD_INFO& getHDD();
    void printHDD();

    PC_INFO& getPC();
    void printPC();

    BIOS_INFO& getBIOS();
    void printBIOS();

    MOTHER_BOARD_INFO& getMB();
    void printMB();

    TMainSoftInfo& getKERNEL();
    void printKERNEL();

    TMainSoftInfo& getBASH();
    void printBASH();

    TMainSoftInfo& getGCC();
    void printGCC();

    TMainSoftInfo& getOPENOFFICE();
    void printOPENOFFICE();

    TMainSoftInfo& getFIREFOX();
    void printFIREFOX();

    unsigned int getMEMNum();
    MEM_DEVICE_INFO& getMEM(int);
    void printMEM();
    bool findMEMSN(std::string);

    unsigned int getUSBNum();
    USBINFO_NODE& getUSB(int);
    void printUSB();
    bool findUSBSN(std::string);

    unsigned int getNICNum();
    NIC_INFO& getNIC(int);
    void printNIC();
    bool findNICSN(std::string);

    HWCollection& operator=(const HWCollection& orig);

private:
    std::string ID;
    bool STATUS;

    CPU_INFO CPU;
    HDD_INFO HDD;
    PC_INFO PC;
    BIOS_INFO BIOS;
    MOTHER_BOARD_INFO MB;
    TMainSoftInfo KERNEL,BASH,GCC,OPENOFFICE,FIREFOX;
    
    std::list<PMEM_DEVICE_INFO> MEM;
    std::list<PUSBINFO_NODE> USB;
    std::list<PNIC_INFO> NIC;

private:
    void freeHW();
};

#endif	/* _HWCOLLECTION_HPP */

