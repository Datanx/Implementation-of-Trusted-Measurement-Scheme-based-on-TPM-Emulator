/* 
 * File:   PolicyOperator.h
 * Author: root
 *
 * Created on January 9, 2010, 8:15 PM
 */

#ifndef _POLICYOPERATOR_H
#define	_POLICYOPERATOR_H

#include <string>

//Item type
#define PLCY_TYPE_PC    1
#define PLCY_TYPE_MB    2
#define PLCY_TYPE_BIOS  3
#define PLCY_TYPE_CPU   4
#define PLCY_TYPE_HDD   5
#define PLCY_TYPE_MEM   6
#define PLCY_TYPE_NIC   7
#define PLCY_TYPE_USB   8
#define PLCY_TYPE_KERNEL    9
#define PLCY_TYPE_GCC       10
#define PLCY_TYPE_BASH      11
#define PLCY_TYPE_OPENOFFICE    12
#define PLCY_TYPE_FIREFOX   13

//Items' positions in policy string
#define PLCY_POS_PC     0
#define PLCY_POS_MB     1
#define PLCY_POS_BIOS   2
#define PLCY_POS_CPU    3
#define PLCY_POS_HDD    4
#define PLCY_POS_HRESRV  5
#define PLCY_POS_HRESRVLEN  10
#define PLCY_POS_MEM    16
#define PLCY_POS_MELLEN 10
#define PLCY_POS_NIC    27
#define PLCY_POS_NICLEN 10
#define PLCY_POS_USB    38
#define PLCY_POS_USBLEN 20
#define PLCY_POS_KERNEL 59
#define PLCY_POS_GCC    60
#define PLCY_POS_BASH   61
#define PLCY_POS_OPENOFFICE 62
#define PLCY_POS_FIREFOX    63
#define PLCY_POS_SRESRV     64
#define PLCY_POS_SRESRVLEN     10

#define CHECK_ITEM_NUM  13

//Policy check type
#define PLCY_CHKTYPE_IGNORE     0
#define PLCY_CHKTYPE_CHECK      1
#define PLCY_CHKTYPE_CHECKNOT   2
#define PLCY_CHKTYPE_UNDEF      3

class PolicyOperator {
public:
    PolicyOperator(std::string& policy_str);
    PolicyOperator(const PolicyOperator& orig);
    virtual ~PolicyOperator();
public:
    void SetPolicy(int object_type, int policy_chktype);
private:
    int m_policy_pos_mem_pointer;
    int m_policy_pos_nic_pointer;
    int m_policy_pos_usb_pointer;
public:
    static char m_policy_chktype_array[4];
    std::string& m_policy_str;
};

#endif	/* _POLICYOPERATOR_H */

