/* 
 * File:   LogicStrPolicyConvertor.cpp
 * Author: root
 * 
 * Created on January 9, 2010, 11:45 AM
 */

#include "LogicStrPolicyConvertor.h"
#include "LogicTreeNode.h"

std::string LogicStrPolicyConvertor::m_ident_pattern_array[] = {
    "PC","MB","BIOS","CPU","HDD","MEM","NIC","USB","KERNEL","GCC","BASH","OPENOFFICE","FIREFOX"
};
int LogicStrPolicyConvertor::m_policy_pos_array[] = {
    PLCY_POS_PC,
    PLCY_POS_MB,
    PLCY_POS_BIOS,
    PLCY_POS_CPU,
    PLCY_POS_HDD,
    PLCY_POS_MEM,
    PLCY_POS_NIC,
    PLCY_POS_USB,
    PLCY_POS_KERNEL,
    PLCY_POS_GCC,
    PLCY_POS_BASH,
    PLCY_POS_OPENOFFICE,
    PLCY_POS_FIREFOX
};

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1: Logic string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicStrPolicyConvertor::LogicStrPolicyConvertor(std::string logic_str) {
    m_logic_str = logic_str;
    m_policy_str = "000000000000000|0000000000|0000000000|00000000000000000000$0000000000";
    m_plcyOp = new PolicyOperator(this->m_policy_str);
}

LogicStrPolicyConvertor::LogicStrPolicyConvertor(const LogicStrPolicyConvertor& orig) {
}

LogicStrPolicyConvertor::~LogicStrPolicyConvertor() {
    //delete m_plcyOp;
}

/*****************************************************
*Function: Convert logic string into policy string.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicStrPolicyConvertor::Convert()
{
    if(m_logic_str.empty())
        return;

    std::string token("");
    TokenTaker tt(m_logic_str);
    while(tt.GetNextToken(token) != GETTOKEN_SOURCESCANFIN)
    {
        if(token == "NOT")
        {
            m_NOT_stk.push(token);
        }
        else if(token == "AND")
        {
            ;   //ignore
        }
        else    //identifier
        {
            this->recg_identifier(token);
        }
    }
}

/*****************************************************
*Function: Recoganize the identifier(token) in logic string and set the corresponding policy value in policy string.
*Parameters:
****Param 1: A token from logic string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
int LogicStrPolicyConvertor::recg_identifier(std::string token)
{
    for(int i=0; i<CHECK_ITEM_NUM; i++)
        ident_set_policy_helper(token,
            LogicStrPolicyConvertor::m_ident_pattern_array[i],
            LogicStrPolicyConvertor::m_policy_pos_array[i]);
}

/*****************************************************
*Function: Find the pattern from token and set the check policy at the corresponding position in policy string(Help Function, called by LogicStrPolicyConvertor::recg_identifier).
*Parameters:
****Param 1: A token from logic string.
****Param 2: Pattern to be matched.
****Param 3: The position corresponding to the object in policy string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicStrPolicyConvertor::ident_set_policy_helper(std::string token, std::string pattern, int object_type)
{
    ExString::esToUpper(token);
    if(token.find(pattern) != std::string::npos)
    {
        if(!m_NOT_stk.empty())
        {
            m_plcyOp->SetPolicy(object_type, PLCY_CHKTYPE_CHECKNOT);
            m_NOT_stk.pop();
        }
        else
        {
            m_plcyOp->SetPolicy(object_type, PLCY_CHKTYPE_CHECK);
        }
    }
}
