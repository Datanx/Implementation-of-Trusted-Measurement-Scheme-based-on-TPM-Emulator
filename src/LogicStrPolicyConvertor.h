/* 
 * File:   LogicStrPolicyConvertor.h
 * Author: root
 * Function: Convert logic string to base-4 policy string
 * Created on January 9, 2010, 11:45 AM
 */

#ifndef _LOGICSTRPOLICYCONVERTOR_H
#define	_LOGICSTRPOLICYCONVERTOR_H

#include "TokenTaker.h"
#include "PolicyOperator.h"
#include "ExString.h"
#include <string>
#include <stack>
#include <iostream>


class LogicStrPolicyConvertor {
public:
    LogicStrPolicyConvertor(std::string logic_str);
    LogicStrPolicyConvertor(const LogicStrPolicyConvertor& orig);
    virtual ~LogicStrPolicyConvertor();
    void Convert();
    std::string GetPolicyString() {return this->m_policy_str;}
private:
    int recg_identifier(std::string token);
    //set the object(specified by object_type) policy as policy_chktype in policy string by finding the specific pattern in token
    void ident_set_policy_helper(std::string token, std::string pattern, int object_type);
private:
    std::string m_logic_str;
    std::string m_policy_str;
    std::stack<std::string> m_NOT_stk;
    PolicyOperator* m_plcyOp;
public:
    static std::string m_ident_pattern_array[CHECK_ITEM_NUM];
    static int m_policy_pos_array[CHECK_ITEM_NUM];
};

#endif	/* _LOGICSTRPOLICYCONVERTOR_H */

