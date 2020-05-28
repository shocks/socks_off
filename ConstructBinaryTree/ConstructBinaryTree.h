//问题：通过中序遍历和前序遍历构造出整棵树
//解决方法：前序遍历的第一个为根结点，得到根结点后，通过在中序遍历中查找根结点，根结点左边的为左子树，根节点右边的右子树，然后按照相同的方法，将两个子树逐步分解，进而求出整棵树。
#pragma once
#include<vector>

struct BinaryTreeNode
{
    int m_value;
    BinaryTreeNode* m_left;
    BinaryTreeNode* m_right;
};

BinaryTreeNode* CreateBinaryTreeNode(int value);
void ConnectBinaryTreeNode(BinaryTreeNode* pParent,BinaryTreeNode* p_lChild,BinaryTreeNode* p_rChild);
void PrintBinaryTreeNode(const BinaryTreeNode* pNode);
void PrintTree(const BinaryTreeNode* pRoot);
void DestroyTree(BinaryTreeNode* pRoot);