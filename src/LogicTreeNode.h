/* 
 * File:   LogicTreeNode.h
 * Author: root
 * Function: Definition of the logic tree node
 * Created on December 14, 2009, 4:41 PM
 */

#ifndef _LOGICTREENODE_H
#define	_LOGICTREENODE_H

#include <string>

typedef enum _TreeNodeType{TNT_EMPTY=0, TNT_SYM, TNT_VAR} TreeNodeType;
typedef enum _LogicOperator{LO_NULL=0, LO_AND, LO_OR, LO_NOT, LO_LBRACE, LO_RBRACE} LogicOperator;

class LogicTreeNode {
public:
    TreeNodeType        m_NodeType;     //indicates node type, operator, value or empty
    LogicOperator       m_Operator;     //logic operator(activated when node type is operator)
    std::string         m_NodeValue;    //stores the value(activated when node type is value)
    LogicTreeNode       *m_LChild;
    LogicTreeNode       *m_RChild;
    LogicTreeNode       *m_Parent;
public:
    LogicTreeNode(TreeNodeType tnt=TNT_VAR);
    LogicTreeNode(const LogicTreeNode& orig);
    LogicTreeNode(const LogicTreeNode* orig);
    virtual ~LogicTreeNode();
private:

};

#endif	/* _LOGICTREENODE_H */

