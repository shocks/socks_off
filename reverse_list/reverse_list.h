//倒序输出链表
//解决办法 1：遍历一边链表，将数据存入栈中，然后遍历栈依次弹出数据
        //2：递归，遍历链表，到达最后最后一个节点后输出数据，依次返回上一个节点输出数据

#include<iostream>
#include<stack>

struct ListNode{
    int m_nValue;
    ListNode* m_pNext;
};
void PrintListReversingly_Iteratively(ListNode* pHead);
void PrintListReversingly_Recursively(ListNode* pHead);
ListNode* CreateListNode(int value);
void ConnectListNodes(ListNode* pCurrent,ListNode* pNext);
void PrintListNode(ListNode* pNode);
void PrintList(ListNode* pHead);
void DestroyList(ListNode* pHead);
void AddToTail(ListNode** pHead,int value);
void RemoveNode(ListNode** pHead,int value);
