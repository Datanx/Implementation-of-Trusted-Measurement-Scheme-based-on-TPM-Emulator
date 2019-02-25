/* 
 * File:   LogicTreeNode.cpp
 * Author: root
 * 
 * Created on December 14, 2009, 4:41 PM
 */

#include "LogicTreeNode.h"

/*****************************************************
*Function: Constructor.
*Parameters:
****Param 1: Tree node type.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicTreeNode::LogicTreeNode(TreeNodeType tnt) {
    switch(tnt)
    {
	case TNT_EMPTY:
	    this->m_NodeType = TNT_EMPTY;
	    this->m_Operator = LO_NULL;
	    break;
	case TNT_SYM:
	    this->m_NodeType = TNT_SYM;
	    this->m_Operator = LO_AND;
	    break;
	case TNT_VAR:
	    this->m_NodeType = TNT_VAR;
	    this->m_Operator = LO_NULL;
	    break;
	default:
	    break;
    }
    
    this->m_NodeValue = "";
    this->m_LChild = this->m_RChild = this->m_Parent = NULL;
}

/*****************************************************
*Function: Copy constructor 1.
*Parameters:
****Param 1: LogicTreeNode object.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicTreeNode::LogicTreeNode(const LogicTreeNode& orig) {
    this->m_LChild = orig.m_LChild;
    this->m_RChild = orig.m_RChild;
    this->m_Parent = orig.m_Parent;
    this->m_NodeType = orig.m_NodeType;
    this->m_NodeValue = orig.m_NodeValue;
    this->m_Operator = orig.m_Operator;
}

/*****************************************************
*Function: Copy constructor 2.
*Parameters:
****Param 1: LogicTreeNode object.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicTreeNode::LogicTreeNode(const LogicTreeNode* orig)
{
    this->m_LChild = NULL;
    this->m_RChild = NULL;
    this->m_Parent = NULL;
    this->m_NodeType = orig->m_NodeType;
    this->m_NodeValue = orig->m_NodeValue;
    this->m_Operator = orig->m_Operator;
}

LogicTreeNode::~LogicTreeNode() {
}

