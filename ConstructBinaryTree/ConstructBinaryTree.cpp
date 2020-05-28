#include"ConstructBinaryTree.h"
#include<cstdio>
#include<exception>

BinaryTreeNode* CreateBinaryTreeNode(int value){
    BinaryTreeNode* pNew=new BinaryTreeNode();
    pNew->m_left=nullptr;
    pNew->m_right=nullptr;
    return pNew;
}
void ConnectBinaryTreeNode(BinaryTreeNode* pParent,BinaryTreeNode* p_lChild,BinaryTreeNode* p_rChild){

}
void PrintBinaryTreeNode(const BinaryTreeNode* pNode){

}
void PrintTree(const BinaryTreeNode* pRoot){

}
void DestroyTree(BinaryTreeNode* pRoot){

}