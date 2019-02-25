/* 
 * File:   LogicTreeAnalyzer.cpp
 * Author: root
 * 
 * Created on December 20, 2009, 4:58 PM
 */

#include "LogicTreeAnalyzer.h"

LogicTreeNode* LogicTreeAnalyzer::m_min_logic_tree[1024] = {NULL};
std::string LogicTreeAnalyzer::m_logic_str_array[1024]={""};
int LogicTreeAnalyzer::m_min_logic_tree_ptr = 0;

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1: Logic string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicTreeAnalyzer::LogicTreeAnalyzer(LogicTree* lgt) {
    m_lgt = lgt;
}

LogicTreeAnalyzer::LogicTreeAnalyzer(const LogicTreeAnalyzer& orig) {
}

LogicTreeAnalyzer::~LogicTreeAnalyzer() {
}

/*****************************************************
*Function: Extract each minimum logic trees from the big logic tree.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTreeAnalyzer::ExtractMinLogicTree()
{
    if(LogicTree::m_root == NULL)
        return;

    if(LogicTree::m_root->m_Operator == LO_AND || LogicTree::m_root->m_Operator == LO_NOT || LogicTree::m_root->m_NodeType == TNT_VAR)
        LogicTreeAnalyzer::m_min_logic_tree[LogicTreeAnalyzer::m_min_logic_tree_ptr++] = LogicTree::m_root;
    else
        m_lgt->traversal_preorder(LogicTree::m_root, extract_min_logic_tree_helper);

    //convert each sub logic tree into logic string, and save it into m_logic_str_array
    for(int i=0; i<m_min_logic_tree_ptr; i++)
    {
        LogicTreeNode* subroot = NULL;
        std::string str("");
        subroot = LogicTreeAnalyzer::m_min_logic_tree[i];
        m_lgt->traversal_inorder(subroot, str, PrintNodeToString);
        LogicTreeAnalyzer::m_logic_str_array[i] = this->formalize_logic_string(str);
    }
}

/*****************************************************
*Function: Clear the class members used for storing minimum logic trees.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTreeAnalyzer::Truncate()
{
    for(int i=0; i<1024; i++)
    {
        LogicTreeAnalyzer::m_logic_str_array[i] = "";
        LogicTreeAnalyzer::m_min_logic_tree[i] = NULL;
    }
    LogicTreeAnalyzer::m_min_logic_tree_ptr = 0;
}

/*****************************************************
*Function: Formalize the logic string(Eliminate meanless token in logic string).
*Parameters: 
****Param 1: Logic string.
*Return: Formalized logic string.
*Remark: Nothing.
*****************************************************/
std::string LogicTreeAnalyzer::formalize_logic_string(std::string& str)
{
    std::string token("");
    std::string tmp("");
    std::string result("");
    std::stack<std::string> stk;
    TokenTaker tt(str);

    while(tt.GetNextToken(token) != GETTOKEN_SOURCESCANFIN)
    {
        if(token == "AND")
        {
            if(!stk.empty())
            {
                tmp = stk.top();
                stk.pop();
                result.append(tmp + " " + token + " ");
            }
            else
                result.append(token + " ");
        }
        else if(token == "NOT")
        {
            tmp=stk.top();
            stk.pop();
            result.append(token + " " + tmp + " ");
            //eat next "NULL"
            tt.GetNextToken(token);
        }
        else
        {
            stk.push(token);
        }
    }//while
    if(!stk.empty())    //single variable expression
    {
        result.append(stk.top() + " ");
        stk.pop();
    }

    tt.Truncate();

    return result;
}

/*****************************************************
*Function: Extract minimum logic trees(Help Function, called by LogicTreeAnalyzer::ExtractMinLogicTree).
*Parameters: 
****Param 1: The root of logic tree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void extract_min_logic_tree_helper(LogicTreeNode* node)
{
    if(node != NULL)
    {
        if(node->m_NodeType == TNT_SYM && node->m_Operator == LO_OR)
        {
            if(node->m_LChild->m_Operator != LO_OR)
            {
                LogicTreeAnalyzer::m_min_logic_tree[LogicTreeAnalyzer::m_min_logic_tree_ptr++] = node->m_LChild;
            }
            if(node->m_RChild->m_Operator != LO_OR)
            {
                LogicTreeAnalyzer::m_min_logic_tree[LogicTreeAnalyzer::m_min_logic_tree_ptr++] = node->m_RChild;
            }
        }//if
    }//if
}
