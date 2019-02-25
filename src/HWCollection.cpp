/* 
 * File:   HWCollection.cpp
 * Author: jjdengjj
 * 
 * Created on November 27, 2009, 1:47 PM
 */

#include "HWCollection.hpp"

HWCollection::HWCollection(const char* IP) {
    this->STATUS = false;
    this->ID = std::string(IP);
}

HWCollection::HWCollection() {
}

HWCollection::HWCollection(const HWCollection& orig) {
    this->ID = orig.ID;
    this->BIOS = orig.BIOS;
    this->CPU = orig.CPU;
    this->HDD = orig.HDD;
    this->MB = orig.MB;
    this->MEM = orig.MEM;
    this->NIC = orig.NIC;
    this->PC = orig.PC;
    this->USB = orig.USB;
    this->STATUS = orig.STATUS;
}

HWCollection::~HWCollection() {
    this->freeHW();
}

bool HWCollection::Update() {
    this->freeHW();

    AgentConnector Conn = AgentConnector(this->ID.c_str());
    if (Conn.m_openAgent()) {
        Conn.m_getCPUInfo(&this->CPU);
        usleep(50 * 1000);

        Conn.m_getHDDInfo(&this->HDD);
        usleep(50 * 1000);

        Conn.m_getPCInfo(&this->PC);
        usleep(50 * 1000);

        Conn.m_getBIOSInfo(&this->BIOS);
        usleep(50 * 1000);

        Conn.m_getMBInfo(&this->MB);
        usleep(50 * 1000);

        Conn.m_getMEMInfo(this->MEM);
        usleep(50 * 1000);

        Conn.m_getUSBInfo(this->USB);
        usleep(50 * 1000);

        Conn.m_getNICInfo(this->NIC);
        usleep(50 * 1000);

        Conn.m_getSWInfo(&this->KERNEL, CMD_MN_REQ_KERNELINFO);
        usleep(50 * 1000);

        Conn.m_getSWInfo(&this->BASH, CMD_MN_REQ_BASHINFO);
        usleep(50 * 1000);

        Conn.m_getSWInfo(&this->GCC, CMD_MN_REQ_GCCINFO);
        usleep(50 * 1000);

        Conn.m_getSWInfo(&this->OPENOFFICE, CMD_MN_REQ_OPENOFFICEINFO);
        usleep(50 * 1000);

        Conn.m_getSWInfo(&this->FIREFOX, CMD_MN_REQ_FIREFOXINFO);
        usleep(50 * 1000);

        Conn.m_closeAgent();

        this->STATUS = true;
        return true;
    } else
        return false;
}

std::string HWCollection::getID() {
    return this->ID;
}

void HWCollection::freeHW() {
    if (this->STATUS) {
        std::list<PMEM_DEVICE_INFO>::iterator mit = this->MEM.begin();
        while (mit != this->MEM.end()) {
            free(*mit);
            mit++;
        }
        this->MEM.clear();

        std::list<PUSBINFO_NODE>::iterator uit = this->USB.begin();
        while (uit != this->USB.end()) {
            free(*uit);
            uit++;
        }
        this->USB.clear();

        std::list<PNIC_INFO>::iterator nit = this->NIC.begin();
        while (nit != this->NIC.end()) {
            free(*nit);
            nit++;
        }
        this->NIC.clear();

        this->STATUS = false;
    }
}

CPU_INFO& HWCollection::getCPU() {
    if (STATUS)
        return this->CPU;
}

void HWCollection::printCPU() {
    if (STATUS) {
        print_cpu_info(&this->CPU);
        fprintf(stdout, "\n");
    } else
        fprintf(stdout, "CPU Information Invalid!\n");
}

HDD_INFO& HWCollection::getHDD() {
    if (STATUS)
        return this->HDD;
}

void HWCollection::printHDD() {
    if (STATUS) {
        print_hdd_info(&this->HDD);
        fprintf(stdout, "\n");
    } else
        fprintf(stdout, "HDD Information Invalid!\n");
}

PC_INFO& HWCollection::getPC() {
    if (STATUS)
        return this->PC;
}

void HWCollection::printPC() {
    if (STATUS) {
        print_pc_info(&this->PC);
        fprintf(stdout, "\n");
    } else
        fprintf(stdout, "PC Information Invalid!\n");
}

BIOS_INFO& HWCollection::getBIOS() {
    if (STATUS)
        return this->BIOS;
}

void HWCollection::printBIOS() {
    if (STATUS) {
        print_bios_info(&this->BIOS);
        fprintf(stdout, "\n");
    } else
        fprintf(stdout, "BIOS Information Invalid!\n");
}

MOTHER_BOARD_INFO& HWCollection::getMB() {
    if (STATUS)
        return this->MB;
}

void HWCollection::printMB() {
    if (STATUS) {
        print_mb_info(&this->MB);
        fprintf(stdout, "\n");
    } else
        fprintf(stdout, "Mother Board  Information Invalid!\n");
}

unsigned int HWCollection::getMEMNum() {
    if (STATUS)
        return this->MEM.size();
    else
        return 0;
}

MEM_DEVICE_INFO& HWCollection::getMEM(int count) {
    if (STATUS && count < this->MEM.size()) {
        PMEM_DEVICE_INFO tmp = NULL;
        std::list<PMEM_DEVICE_INFO>::iterator it = this->MEM.begin();
        for (int i = 0; i < count; i++)
            it++;
        tmp = *it;
        return *tmp;
    }
}

void HWCollection::printMEM() {
    if (STATUS) {
        fprintf(stdout, "Memory Information:\n");
        std::list<PMEM_DEVICE_INFO>::iterator it = this->MEM.begin();
        while (it != this->MEM.end()) {
            print_mm_info(*it);
            it++;
        }
    } else
        fprintf(stdout, "Memory  Information Invalid!\n");
}

bool HWCollection::findMEMSN(std::string sn) {
    if (STATUS) {
        std::list<PMEM_DEVICE_INFO>::iterator it = this->MEM.begin();
        while (it != this->MEM.end()) {
            if (sn == std::string((*it)->md5_hash))
                return true;
            it++;
        }
        return false;
    } else
        return false;
}

unsigned int HWCollection::getUSBNum() {
    if (STATUS)
        return this->USB.size();
    else
        return 0;
}

USBINFO_NODE& HWCollection::getUSB(int count) {
    if (STATUS && count < this->USB.size()) {
        PUSBINFO_NODE tmp = NULL;
        std::list<PUSBINFO_NODE>::iterator it = this->USB.begin();
        for (int i = 0; i < count; i++)
            it++;
        tmp = *it;
        return *tmp;
    }
}

void HWCollection::printUSB() {
    if (STATUS) {
        fprintf(stdout, "USB Information:\n");
        std::list<PUSBINFO_NODE>::iterator it = this->USB.begin();
        while (it != this->USB.end()) {
            print_udi_node(*it);
            it++;
        }
    } else
        fprintf(stdout, "USB  Information Invalid!\n");
}

bool HWCollection::findUSBSN(std::string sn) {
    if (STATUS) {
        std::list<PUSBINFO_NODE>::iterator it = this->USB.begin();
        while (it != this->USB.end()) {
            if (sn == std::string((*it)->md5_hash))
                return true;
            it++;
        }
        return false;
    } else
        return false;
}

unsigned int HWCollection::getNICNum() {
    if (STATUS)
        return this->NIC.size();
    else
        return 0;
}

NIC_INFO& HWCollection::getNIC(int count) {
    if (STATUS && count < this->NIC.size()) {
        PNIC_INFO tmp = NULL;
        std::list<PNIC_INFO>::iterator it = this->NIC.begin();
        for (int i = 0; i < count; i++)
            it++;
        tmp = *it;
        return *tmp;
    }
}

void HWCollection::printNIC() {
    if (STATUS) {
        fprintf(stdout, "NIC Information:\n");
        std::list<PNIC_INFO>::iterator it = this->NIC.begin();
        while (it != this->NIC.end()) {
            print_nic_info(*it);
            it++;
        }
    } else
        fprintf(stdout, "NIC  Information Invalid!\n");
}

bool HWCollection::findNICSN(std::string sn) {
    if (STATUS) {
        std::list<PNIC_INFO>::iterator it = this->NIC.begin();
        while (it != this->NIC.end()) {
            if (sn == std::string((*it)->md5_hash))
                return true;
            it++;
        }
        return false;
    } else
        return false;
}

void HWCollection::printALL() {
    fprintf(stdout, "*************************************************************************\n");
    fprintf(stdout, "*The IPv4=\"%s\" Hardware Information List here\n", this->ID.c_str());
    fprintf(stdout, "*************************************************************************\n\n");
    this->printPC();
    this->printCPU();
    this->printBIOS();
    this->printMB();
    this->printMEM();
    this->printHDD();
    this->printNIC();
    this->printUSB();
}

HWCollection& HWCollection::operator =(const HWCollection& orig) {
    this->ID = orig.ID;
    this->BIOS = orig.BIOS;
    this->CPU = orig.CPU;
    this->HDD = orig.HDD;
    this->MB = orig.MB;
    this->MEM = orig.MEM;
    this->NIC = orig.NIC;
    this->PC = orig.PC;
    this->USB = orig.USB;
    this->STATUS = orig.STATUS;
}

TMainSoftInfo& HWCollection::getKERNEL() {
 if (STATUS)
        return this->KERNEL;
}

void HWCollection::printKERNEL() {

}

TMainSoftInfo& HWCollection::getBASH() {
if (STATUS)
        return this->BASH;
}

void HWCollection::printBASH() {

}

TMainSoftInfo& HWCollection::getGCC() {
if (STATUS)
        return this->GCC;
}

void HWCollection::printGCC() {

}

TMainSoftInfo& HWCollection::getOPENOFFICE() {
if (STATUS)
        return this->OPENOFFICE;
}

void HWCollection::printOPENOFFICE() {

}

TMainSoftInfo& HWCollection::getFIREFOX() {
if (STATUS)
        return this->FIREFOX;
}

void HWCollection::printFIREFOX() {

}
