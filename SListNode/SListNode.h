#ifndef _SLISTNODE_H_
#define _SLISTNODE_H_

#include<stdio.h>
#include<Windows.h>
#include<math.h>
typedef int SLTDataType;
typedef struct SListNode
{
	SLTDataType data;
	struct SListNode* next;
}SListNode;

void SListInit(SListNode** pphead);
void SListDestory(SListNode** pphead);
SListNode* BuySListNode(SLTDataType x);//创建新的节点
void SListPushFront(SListNode** pphead, SLTDataType x);//前插
void SListPopFront(SListNode** pphead);//前删
SListNode* SListFind(SListNode* phead, SLTDataType x);//查找
void SListInsertAfter(SListNode* pos, SLTDataType x);// 在pos的后面进行插入
void SListEraseAfter(SListNode* pos);//删除pos之后的一个元素
void SListRemove(SListNode** pphead, SLTDataType x);//删除第一个为X的元素
void SListPrint(SListNode* phead);
void SListRemoveAll(SListNode** pphead, SLTDataType x);//删除所有为X的元素
void SListReverse(SListNode** pphead);//反转链表
void SListReverse2(SListNode **pphead);//反转法2

SListNode *getIntersectionNode(SListNode *headA, SListNode *headB);//两单链找第一个相交节点

SListNode *detectCycle(SListNode *head);//给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 NULL
#endif /*_SLISTNODE_H_*/