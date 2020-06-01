#include"ConstructBinaryTree.h"
#include<cstdio>
#include<exception>
#include<iostream>

BinaryTreeNode* CreateBinaryTreeNode(int value){
    BinaryTreeNode* pNew=new BinaryTreeNode();
    pNew->m_left=nullptr;
    pNew->m_right=nullptr;
    return pNew;
}
void ConnectBinaryTreeNode(BinaryTreeNode* pParent,BinaryTreeNode* p_lChild,BinaryTreeNode* p_rChild){
    if(pParent==nullptr)
        return;
    pParent->m_left=p_lChild;
    pParent->m_right=pParent;
}
void PrintBinaryTreeNode(const BinaryTreeNode* pNode){
    if(pNode!=nullptr){
        std::cout<<pNode->m_value<<std::endl;
    }
}
void PrintTree(const BinaryTreeNode* pRoot){
    if(pRoot==nullptr)
        return;
    PrintBinaryTreeNode(pRoot);
    if(pRoot->m_left!=nullptr)
        PrintTree(pRoot->m_left);
    if(pRoot->m_right!=nullptr)
        PrintTree(pRoot->m_right);
}
void DestroyTree(BinaryTreeNode* pRoot){
    BinaryTreeNode* pLeft = pRoot->m_left;
    BinaryTreeNode* pRight = pRoot->m_right;

    delete pRoot;
    pRoot = nullptr;

    DestroyTree(pLeft);
    DestroyTree(pRight);
 /*   if(pRoot==nullptr)
        return;
    if(pRoot->m_left!=nullptr){
        DestroyTree(pRoot->m_left);
    }
    
    if(pRoot->m_right!=nullptr){
        DestroyTree(pRoot->m_right);
    }
    delete pRoot;
    */

}