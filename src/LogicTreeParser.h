/* 
 * File:   LogicTreeParser.h
 * Author: root
 * Function: Parse a logic string into logic tree
 * Created on December 14, 2009, 7:32 PM
 */

#ifndef _LOGICTREEPARSER_H
#define	_LOGICTREEPARSER_H

#include "LogicTreeNode.h"
#include <string.h>
#include <stack>

//get token return values
#define     GETTOKEN_EMPTYSOURCE    1
#define     GETTOKEN_TOKENFOUND     2
#define     GETTOKEN_SOURCESCANFIN  3

//operator priority definitions
#define     CAL_PRIORITY_0          0
#define     CAL_PRIORITY_1          1
#define     CAL_PRIORITY_2          2
#define     CAL_PRIORITY_3          3

class LogicTreeParser {
private:
    LogicTreeNode   *m_subroot;
    std::string     m_logic_str;
    int             m_logic_str_pointer;

    std::stack<std::string> m_opStack;      //operator stack
    std::stack<LogicTreeNode*> m_nodeStack; //tree node stack
public:
    static std::string m_opArray[6];        //operator array
    LogicTreeParser(std::string logicstr);
    LogicTreeParser(const LogicTreeParser& orig);
    virtual ~LogicTreeParser();
    void LogicStringParser();     //parse logical string into logical binary tree
    LogicTreeNode* GetLogicTree();     //return logic tree root
public:
    static int logic_string_ret_op_priority(std::string op);       //return op priority by op string
    static LogicOperator logic_string_ret_op_sym(std::string op);  //return op symbol by op string
private:
    void logic_string_trim();
    int logic_string_get_next_token(std::string &token);    //get next token
    LogicTreeNode* logic_string_make_subtree();             //make subtree procedure
    LogicTreeNode *logic_string_make_empty_node();          //make empty tree node(for NOT)
    LogicTreeNode* logic_string_make_single_node(std::string token);    //make var tree node
    //make a node with two child, one is opObj, the other is empty tree node, root is op
    //this method is implemented for unary operator
    LogicTreeNode *logic_string_make_node(std::string op, LogicTreeNode *opObj);
    //make a node with two child, one is opObj1, the other is opObj2, root is op
    //this method is implemented for binary operator
    LogicTreeNode *logic_string_make_node(std::string op, LogicTreeNode *opObj1, LogicTreeNode *opObj2);
    
    
};

#endif	/* _LOGICTREEPARSER_H */

