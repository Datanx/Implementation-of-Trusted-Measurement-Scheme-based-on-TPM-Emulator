/* 
 * File:   PolicyOperator.cpp
 * Author: root
 * 
 * Created on January 9, 2010, 8:15 PM
 */

#include <iostream>

#include "PolicyOperator.h"

char PolicyOperator::m_policy_chktype_array[4] = {'0', '1', '2', '3'};

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1(IN): Policy string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PolicyOperator::PolicyOperator(std::string& policy_str): m_policy_str(policy_str){
    m_policy_pos_mem_pointer = 0;
    m_policy_pos_nic_pointer = 0;
    m_policy_pos_usb_pointer = 0;
}

/*****************************************************
*Function: Copy constructor.
*Parameters:
****Param 1(IN): PolicyOperator object.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PolicyOperator::PolicyOperator(const PolicyOperator& orig):m_policy_str(orig.m_policy_str) {
}

/*****************************************************
*Function: Destructor.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
PolicyOperator::~PolicyOperator() {
}

/*****************************************************
*Function: Set policy to certain type.
*Parameters:
****Param 1(IN): Object type(CPU, MEM, MB, BIOS, etc.).
****Param 2(IN): Check type(This parameter must be one of the 4 values: PLCY_CHKTYPE_IGNORE, PLCY_CHKTYPE_CHECK, PLCY_CHKTYPE_CHECKNOT, PLCY_CHKTYPE_UNDEF).
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PolicyOperator::SetPolicy(int object_type, int policy_chktype)
{
    switch(object_type)
    {
        case PLCY_POS_MEM:
            m_policy_str[object_type + m_policy_pos_mem_pointer++] = PolicyOperator::m_policy_chktype_array[policy_chktype];
            break;
        case PLCY_POS_NIC:
            m_policy_str[object_type + m_policy_pos_nic_pointer++] = PolicyOperator::m_policy_chktype_array[policy_chktype];
            break;
        case PLCY_POS_USB:
            m_policy_str[object_type + m_policy_pos_usb_pointer++] = PolicyOperator::m_policy_chktype_array[policy_chktype];
            break;
        default:
            this->m_policy_str[object_type] = PolicyOperator::m_policy_chktype_array[policy_chktype];
            break;
    }

    return;
}
