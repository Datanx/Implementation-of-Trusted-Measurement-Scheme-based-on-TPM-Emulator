/* 
 * File:   LogicTree.cpp
 * Author: root
 * 
 * Created on December 14, 2009, 4:39 PM
 */

#include "LogicTree.h"


bool LogicTree::m_is_proper_priority = true;
LogicTreeNode* LogicTree::m_copyed_subtree = NULL;
LogicTreeNode* LogicTree::m_tmp_parent = NULL;
LogicTreeNode* LogicTree::m_root = NULL;

/*****************************************************
*Function: Constructor. Parse logic string into logic tree.
*Parameters:
****Param 1: Logic string.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
LogicTree::LogicTree(std::string logic_str) {
    m_ltparser = new LogicTreeParser(logic_str);
}

LogicTree::LogicTree(const LogicTree& orig) {
}

LogicTree::~LogicTree() {
}

/*****************************************************
*Function: Create logic tree.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::CreateLogicTree()
{
    if(m_ltparser == NULL)
        return;

    m_ltparser->LogicStringParser();
    m_root = m_ltparser->GetLogicTree();
}

/*****************************************************
*Function: Adjust logic tree to satisfy the operator priority principle of Low-Top&High-Bottom.
*Parameters: No parameter.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::LogicTreeAdjust()
{
    if(m_root == NULL)
        return;

    this->traversal_inorder(m_root, logic_tree_adjust_helper);
}

/*****************************************************
*Function: Check the logic tree to assure whether the operators meet the principle of Low-Top&High-Bottom.
*Parameters: No parameter.
*Return: Check result.
*Remark: Nothing.
*****************************************************/
bool LogicTree::LogicTreeChkPriority()
{
    this->traversal_preorder(m_root, logic_tree_check_op_priority_helper);

    return m_is_proper_priority;
}

/*****************************************************
*Function: Clone a subtree based on a given one.
*Parameters: 
****Param 1: Given subtree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::LogicTreeCopySubtree(LogicTreeNode* subtree)
{
    this->logic_tree_duplicate_subtree_helper(subtree, m_tmp_parent, "ROOT");
}

int LogicTree::get_height(LogicTreeNode* subroot)
{
    if(subroot == NULL)
        return 1;

    return std::max(this->get_height(subroot->m_LChild), this->get_height(subroot->m_RChild)) + 1;
}

/*****************************************************
*Function: Inorder tree traversal.
*Parameters: 
****Param 1: The tree root.
****Param 2: A pointer to function that handles the tree nodes.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::traversal_inorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node))
{
    if(subroot != NULL)
    {
        traversal_inorder(subroot->m_LChild, visit);
        visit(subroot);
        traversal_inorder(subroot->m_RChild, visit);
    }
}

/*****************************************************
*Function: Inorder tree traversal.
*Parameters: 
****Param 1: The tree root.
****Param 2: Extra string value.
****Param 3: A pointer to function that handles the tree nodes.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::traversal_inorder(LogicTreeNode* subroot, std::string& str, void(*visit)(LogicTreeNode* node, std::string& str))
{
    if(subroot != NULL)
    {
        traversal_inorder(subroot->m_LChild, str, visit);
        visit(subroot, str);
        traversal_inorder(subroot->m_RChild, str, visit);
    }
}

/*****************************************************
*Function: Preorder tree traversal.
*Parameters: 
****Param 1: The tree root.
****Param 2: A pointer to function that handles the tree nodes.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::traversal_preorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node))
{
    if(subroot != NULL)
    {
        visit(subroot);
        traversal_preorder(subroot->m_LChild, visit);
        traversal_preorder(subroot->m_RChild, visit);
    }
}

/*****************************************************
*Function: Postorder tree traversal.
*Parameters: 
****Param 1: The tree root.
****Param 2: A pointer to function that handles the tree nodes.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::traversal_postorder(LogicTreeNode* subroot, void(*visit)(LogicTreeNode* node))
{
    if(subroot != NULL)
    {
        traversal_postorder(subroot->m_LChild, visit);
        traversal_postorder(subroot->m_RChild, visit);
        visit(subroot);
    }
}

/*****************************************************
*Function: Make a operator tree node.
*Parameters: 
****Param 1: Operator.
*Return: New tree node.
*Remark: Nothing.
*****************************************************/
LogicTreeNode* LogicTree::make_a_node(LogicOperator op)
{
    LogicTreeNode* op_node = new LogicTreeNode(TNT_SYM);
    op_node->m_Operator = op;

    return op_node;
}

/*****************************************************
*Function: Make a variable tree node.
*Parameters: 
****Param 1: Variable.
*Return: New tree node.
*Remark: Nothing.
*****************************************************/
LogicTreeNode* LogicTree::make_a_node(std::string var)
{
    LogicTreeNode* var_node = new LogicTreeNode(TNT_VAR);
    var_node->m_NodeValue = var;

    return var_node;
}

/*****************************************************
*Function: Make a empty tree node.
*Parameters: No parameter.
*Return: New tree node.
*Remark: Nothing.
*****************************************************/
LogicTreeNode* LogicTree::make_empty_node()
{
    LogicTreeNode* empty_node = new LogicTreeNode(TNT_EMPTY);
    empty_node->m_Operator = LO_NULL;

    return empty_node;
}

/*****************************************************
*Function: Clone a subtree(Help function, called by LogicTree::LogicTreeCopySubtree).
*Parameters: 
****Param 1: The subtree to be cloned.
****Param 2: Temp variable in which the parent node is stored temporarily.
****Param 3: The direction to indicate which part of the tree is being duplicated.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void LogicTree::logic_tree_duplicate_subtree_helper(
        LogicTreeNode* subroot,
        LogicTreeNode* tmpParent,
        std::string direction)
{
    if(subroot != NULL)
    {
        if(direction == "ROOT")
        {
            m_copyed_subtree = new LogicTreeNode(subroot);
            tmpParent = m_copyed_subtree;
        }
        else if(direction == "LEFT")
        {
            LogicTreeNode* lnode = new LogicTreeNode(subroot);
            tmpParent->m_LChild = lnode;
            lnode->m_Parent = tmpParent;
            tmpParent = tmpParent->m_LChild;
        }
        else    //RIGHT
        {
            LogicTreeNode* rnode = new LogicTreeNode(subroot);
            tmpParent->m_RChild = rnode;
            rnode->m_Parent = tmpParent;
            tmpParent = tmpParent->m_RChild;
        }
        logic_tree_duplicate_subtree_helper(subroot->m_LChild, tmpParent, "LEFT");
        logic_tree_duplicate_subtree_helper(subroot->m_RChild, tmpParent, "RIGHT");
    }
}

/*****************************************************
*Function: Print a tree node(External function).
*Parameters: 
****Param 1: Tree node.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PrintNode(LogicTreeNode* node)
{
    if(node->m_NodeType == TNT_SYM || node->m_NodeType == TNT_EMPTY)
        std::cout<<LogicTreeParser::m_opArray[(int)node->m_Operator]<<" ";
    else
        std::cout<<node->m_NodeValue<<" ";
}

/*****************************************************
*Function: Write a tree node into a string(External function).
*Parameters: 
****Param 1: Tree node.
****Param 2(OUT): The string to be written into.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void PrintNodeToString(LogicTreeNode* node, std::string& str)
{
    if(node->m_NodeType == TNT_SYM || node->m_NodeType == TNT_EMPTY)
    {
        //std::cout<<LogicTreeParser::m_opArray[(int)node->m_Operator]<<" ";
        str.append(LogicTreeParser::m_opArray[(int)node->m_Operator]);
        str.append(" ");
    }
    else
    {
        //std::cout<<node->m_NodeValue<<" ";
        str.append(node->m_NodeValue);
        str.append(" ");
    }
}

/*****************************************************
*Function: Check the priority of operator in the tree(Help function, called by LogicTree::LogicTreeChkPriority).
*Parameters: 
****Param 1: The root of tree/subtree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void logic_tree_check_op_priority_helper(LogicTreeNode *node)
{
    int parent_op_pri, lchild_op_pri, rchild_op_pri;
    LogicTreeNode* parent = NULL;
    LogicTreeNode* lchild = NULL;
    LogicTreeNode* rchild = NULL;

    if(node->m_NodeType == TNT_SYM)
    {
        parent = node;
        parent_op_pri = LogicTreeParser::logic_string_ret_op_priority(LogicTreeParser::m_opArray[parent->m_Operator]);
        
        if(node->m_LChild->m_NodeType == TNT_SYM)
        {
            lchild = parent->m_LChild;
            lchild_op_pri = LogicTreeParser::logic_string_ret_op_priority(LogicTreeParser::m_opArray[lchild->m_Operator]);
            if(parent_op_pri > lchild_op_pri)
                LogicTree::m_is_proper_priority = false;
        }
        if(node->m_RChild->m_NodeType == TNT_SYM)
        {
            rchild = parent->m_RChild;
            rchild_op_pri = LogicTreeParser::logic_string_ret_op_priority(LogicTreeParser::m_opArray[rchild->m_Operator]);
            if(parent_op_pri > rchild_op_pri)
                LogicTree::m_is_proper_priority = false;
        }
    }
}

/*****************************************************
*Function: Adjust the tree(Help function, called by LogicTree::LogicTreeAdjust).
*Parameters: 
****Param 1: The root of tree/subtree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void logic_tree_adjust_helper(LogicTreeNode* node)
{
    if(node->m_NodeType == TNT_SYM)
    {
        switch(node->m_Operator)
        {
            case LO_NOT:
                logic_tree_adjust_NOT_node(node);
                break;
            case LO_AND:
                logic_tree_adjust_AND_node(node);
                break;
            default:
                break;
        }
    }
}

/*****************************************************
*Function: Reversse the operator(External function).
*Parameters: 
****Param 1: Operator to be reversed.
*Return: Reversed operator.
*Remark: Nothing.
*****************************************************/
LogicOperator inverse_operator(LogicOperator op)
{
    switch(op)
    {
        case LO_AND:
            return LO_OR;
        case LO_OR:
            return LO_AND;
    }

    return LO_NULL;
}

/*****************************************************
*Function: Adjust the subtree with the root of NOT operator(External function).
*Parameters: 
****Param 1: The root of the subtree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void logic_tree_adjust_NOT_node(LogicTreeNode* node)
{
    if(node->m_LChild->m_NodeType == TNT_SYM)
    {
        LogicTreeNode* lchild = node->m_LChild;
        LogicTreeNode* node_parent = NULL;
        bool node_is_left_of_its_parent = true;

        if(lchild->m_Operator == LO_AND || lchild->m_Operator == LO_OR)
        {
            //if node's parent is not NULL, save it
            if(node->m_Parent != NULL)
            {
                node_parent = node->m_Parent;
                if(node_parent->m_LChild != node)
                    node_is_left_of_its_parent = false;
            }
            //inverse left child operator
            lchild->m_Operator = inverse_operator(lchild->m_Operator);

            //create a new NOT node
            LogicTreeNode* not_node = LogicTree::make_a_node(LO_NOT);
            not_node->m_RChild = LogicTree::make_empty_node();
            not_node->m_LChild = lchild->m_RChild;
            lchild->m_RChild->m_Parent = not_node;

            //make the left child of lchild be left child of node
            node->m_LChild = lchild->m_LChild;
            lchild->m_LChild->m_Parent = node;

            //build new tree relationship
            //make the lchild be the parent of node & not_node
            lchild->m_LChild = node;
            lchild->m_RChild = not_node;
            node->m_Parent = lchild;
            not_node->m_Parent = lchild;

            //if node's parent exists,
            //make the lchild be its child according to node_is_left_of_its_parent
            if(node_parent != NULL)
            {
                if(node_is_left_of_its_parent)
                    node_parent->m_LChild = lchild;
                else
                    node_parent->m_RChild = lchild;

                lchild->m_Parent = node_parent;
            }
            else
            {
                LogicTree::m_root = lchild;
                LogicTree::m_root->m_Parent = NULL;
            }
        }
        else if(lchild->m_Operator == LO_NOT)
        {
            //if node's parent is not NULL, save it
            if(node->m_Parent != NULL)
            {
                node_parent = node->m_Parent;
                if(node_parent->m_LChild != node)   //node is node_parent's right child
                {
                    node_parent->m_RChild = lchild->m_LChild;
                    lchild->m_LChild->m_Parent = node_parent;
                    
                    delete lchild->m_RChild;
                    delete lchild;
                    delete node->m_RChild;
                    delete node;

                    LogicTree::m_root = node_parent;
                    LogicTree::m_root->m_Parent = NULL;
                }
                else    //node is node_parent's left child
                {
                    node_parent->m_LChild = lchild->m_LChild;
                    lchild->m_LChild->m_Parent = node_parent;
                    
                    delete lchild->m_RChild;
                    delete lchild;
                    delete node->m_RChild;
                    delete node;

                    LogicTree::m_root = node_parent;
                    LogicTree::m_root->m_Parent = NULL;
                }
            }
            else        //node's parent is null
            {
                LogicTree::m_root = lchild->m_LChild;
                LogicTree::m_root->m_Parent = NULL;
                
                delete lchild->m_RChild;
                delete lchild;
                delete node->m_RChild;
                delete node;
            }
        }
    }
}

/*****************************************************
*Function: Adjust the subtree with the root of AND operator(External function).
*Parameters: 
****Param 1: The root of the subtree.
*Return: NULL.
*Remark: Nothing.
*****************************************************/
void logic_tree_adjust_AND_node(LogicTreeNode* node)
{
    LogicTreeNode* lchild = NULL;
    LogicTreeNode* rchild = NULL;

    lchild = node->m_LChild;
    rchild = node->m_RChild;

    if(lchild->m_Operator == LO_OR)
    {
        LogicTreeNode* node_parent = NULL;
        bool node_is_left_of_its_parent = true;
        
        if(node->m_Parent != NULL)
        {
            node_parent = node->m_Parent;
            if(node_parent->m_LChild != node)
                node_is_left_of_its_parent = false;
        }
        //both of the children are OR operator
        //Step 1: break the left child tree
        LogicTreeNode* tmp_lchild = node->m_LChild;
        node->m_LChild = NULL;

        //Step 2: duplicate right partation of node(including node)
        LogicTree::m_tmp_parent = NULL;
        LogicTree::m_copyed_subtree = NULL;
        LogicTree::logic_tree_duplicate_subtree_helper(node, LogicTree::m_tmp_parent, "ROOT");
        LogicTreeNode* tmp_rPart = LogicTree::m_copyed_subtree;

        //Step 3: forming new tree
        node->m_LChild = tmp_lchild->m_LChild;
        tmp_lchild->m_LChild->m_Parent = node;
        tmp_rPart->m_LChild = tmp_lchild->m_RChild;
        tmp_lchild->m_RChild->m_Parent = tmp_rPart;
        
        tmp_lchild->m_LChild = node;
        tmp_lchild->m_RChild = tmp_rPart;
        node->m_Parent = tmp_lchild;
        tmp_rPart->m_Parent = tmp_lchild;

        if(node_parent != NULL)
        {
            if(node_is_left_of_its_parent)
                node_parent->m_LChild = tmp_lchild;
            else
                node_parent->m_RChild = tmp_lchild;

            tmp_lchild->m_Parent = node_parent;
        }
        else
        {
            LogicTree::m_root = tmp_lchild;
            LogicTree::m_root->m_Parent = NULL;
        }
    }
    else if(rchild->m_Operator == LO_OR && lchild->m_Operator != LO_OR)
    {
        //swap left and right child of node
        LogicTreeNode* tmpNode = NULL;

        tmpNode = node->m_LChild;
        node->m_LChild = node->m_RChild;
        node->m_RChild = tmpNode;
    }
}
