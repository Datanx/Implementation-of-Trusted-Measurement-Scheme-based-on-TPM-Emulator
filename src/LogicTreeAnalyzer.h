/* 
 * File:   LogicTreeAnalyzer.h
 * Author: root
 * Function: Extract subtrees from "... OR ... OR ...", and save every subtree to m_min_logic_tree array.
 * m_min_logic_tree_ptr holds the number of subtrees
 * Created on December 20, 2009, 4:58 PM
 */

#ifndef _LOGICTREEANALYZER_H
#define	_LOGICTREEANALYZER_H

#include "LogicTree.h"
#include "TokenTaker.h"
#include <stack>

class LogicTreeAnalyzer {
public:
    LogicTreeAnalyzer(LogicTree* lgt);
    LogicTreeAnalyzer(const LogicTreeAnalyzer& orig);
    virtual ~LogicTreeAnalyzer();

    void ExtractMinLogicTree();
    void Truncate();    //clear all members of the class
private:
    //formalize logic string(especially for NOT operator)
    std::string formalize_logic_string(std::string& str);
public:
    LogicTree*  m_lgt;
    static LogicTreeNode *m_min_logic_tree[1024];   //store subtrees
    static std::string m_logic_str_array[1024];     //store logic string corresponding to relavant subtree
    static int m_min_logic_tree_ptr;
};

void extract_min_logic_tree_helper(LogicTreeNode* node);

#endif	/* _LOGICTREEANALYZER_H */

