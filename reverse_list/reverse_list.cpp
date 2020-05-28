#include"reverse_list.h"
//实现反向输出链表
void PrintListReversingly_Iteratively(ListNode* pHead){
    //使用栈实现，每当遍历一个节点，便向栈中存储一个节点，遍历完整个链表后依次弹出栈中的数据
    std::stack<ListNode*> nodes;
    ListNode* pNode=pHead;
    while(pNode!=nullptr){
        nodes.push(pNode);
        pNode=pNode->m_pNext;
    }
    while(!nodes.empty()){
        pNode=nodes.top();
        std::cout<<"pNode="<<pNode->m_nValue<<std::endl;
        nodes.pop();
    }
}
void PrintListReversingly_Recursively(ListNode* pHead){
    //如果当前节点的下一个节点不为空，则递归下一个节点，到最后一个节点后反向输出数据
    if(pHead!=nullptr){
        if(pHead->m_pNext!=nullptr)
        {
            PrintListReversingly_Recursively(pHead->m_pNext);
        }
        std::cout<<"data="<<pHead->m_nValue<<std::endl;
    }
}


ListNode* CreateListNode(int value){
    ListNode* pNode=new ListNode();
    pNode->m_nValue=value;
    return pNode;
}
void ConnectListNodes(ListNode* pCurrent,ListNode* pNext){
    if(nullptr==pCurrent){
        printf("error to connect list!\n");
        exit(1);
    }
    pCurrent->m_pNext=pNext;
}
void PrintListNode(ListNode* pNode){
    if(nullptr==pNode){
        printf("error to print node!!\n");
        exit(1);
    }
    printf("the key in nodes is %d\n",pNode->m_nValue);
}
void PrintList(ListNode* pHead){
    printf("print list start.\n");
    ListNode* pNode=pHead;
    while (pNode!=nullptr)
    {
        printf("%d\t",pNode->m_nValue);
        pNode=pNode->m_pNext;
    }
    printf("print list end.\n");
}
void DestroyList(ListNode* pHead){
    ListNode* pNode=pHead;
    while (nullptr!=pNode)
    {
        pHead=pHead->m_pNext;
        delete pNode;
        pNode=pHead;
    }
}
/*void DestroyList(ListNode* pHead){
    if(nullptr!=pHead->m_pNext){
      DestroyList(pHead->m_pNext);  
    }
    else
    {
        delete pHead;
    }
    
}*/
void AddToTail(ListNode** pHead,int value){
    ListNode* pNew=new ListNode();
    pNew->m_nValue=value;
    pNew->m_pNext=nullptr;
    if(*pHead==nullptr){
        *pHead=pNew;
    }
    else{
        ListNode* pNode=*pHead;
        while (pNode->m_pNext!=nullptr)
        {
            pNode=pNode->m_pNext;
        }
        pNode->m_pNext=pNew;
    }
}
void RemoveNode(ListNode** pHead,int value){
    if(pHead==nullptr || *pHead==nullptr)
        return;
    ListNode* pToDeleted=nullptr;
    if((*pHead)->m_nValue==value){
        pToDeleted=*pHead;
        *pHead=(*pHead)->m_pNext;
    }
    else{
        ListNode* pNode=*pHead;
        while(pNode->m_pNext!=nullptr && pNode->m_pNext->m_nValue!=value){
            pNode=pNode->m_pNext;
        }
        if(pNode->m_pNext!=nullptr && pNode->m_pNext->m_nValue==value)
        {
            pToDeleted=pNode->m_pNext;
            pNode->m_pNext=pNode->m_pNext->m_pNext;
        }
    }
    if(pToDeleted!=nullptr){
        delete pToDeleted;
        pToDeleted=nullptr;
    }
}