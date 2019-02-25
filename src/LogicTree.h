/* 
 * File:   LogicTree.h
 * Author: root
 * Functions: Establish a logic tree
 * Created on December 14, 2009, 4:39 PM
 */

#ifndef _LOGICTREE_H
#define	_LOGICTREE_H

#include "LogicTreeNode.h"
#include "LogicTreeParser.h"
#include <iostream>

class LogicTree {
public:
    static bool         m_is_proper_priority;
    static LogicTreeNode*      m_root;
    LogicTreeParser*    m_ltparser;
    static LogicTreeNode*      m_tmp_parent;
    static LogicTreeNode*      m_copyed_subtree;
public:
    LogicTree(std::string logic_str);
    LogicTree(const LogicTree& orig);
    virtual ~LogicTree();

    int get_height(LogicTreeNode* subroot);
    void traversal_inorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node));
    void traversal_inorder(LogicTreeNode* subroot, std::string& str, void(*visit)(LogicTreeNode* node, std::string& str));
    void traversal_preorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node));
    void traversal_postorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node));

    static LogicTreeNode* make_a_node(LogicOperator op);
    static LogicTreeNode* make_a_node(std::string var);
    static LogicTreeNode* make_empty_node();

    void CreateLogicTree(); //create logic tree
    bool LogicTreeChkPriority();    //operator priority check
    void LogicTreeAdjust();         //logic tree adjust
    void LogicTreeCopySubtree(LogicTreeNode* subtree);  //clone subtree

    static void logic_tree_duplicate_subtree_helper(LogicTreeNode* subroot, LogicTreeNode* tmpParent, std::string direction);
};

void logic_tree_check_op_priority_helper(LogicTreeNode *node);
void logic_tree_adjust_helper(LogicTreeNode* node);
void logic_tree_adjust_NOT_node(LogicTreeNode* node);
void logic_tree_adjust_AND_node(LogicTreeNode* node);
LogicOperator inverse_operator(LogicOperator op);   //inverse node's operator
void PrintNode(LogicTreeNode* node);
void PrintNodeToString(LogicTreeNode* node, std::string& str);

#endif	/* _LOGICTREE_H */
