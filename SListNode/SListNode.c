#include"SListNode.h"
SListNode* BuySListNode(SLTDataType x)
{
	SListNode*new_node = (SListNode*)malloc(sizeof(SListNode));
	new_node->data = x;
	new_node->next = NULL;
	return new_node;
}

void SListInit(SListNode** pphead)
{
	*pphead = NULL;
}

void SListPushFront(SListNode** pphead, SLTDataType x)
{//头插
	if (*pphead == NULL)
	{//头位空直接插入
		*pphead = BuySListNode(x);
	}
	else
	{
		SListNode* tmp = BuySListNode(x);
		tmp->next = *pphead;
		*pphead = tmp;
	}
}

void SListDestory(SListNode** pphead)
{
	if (*pphead == NULL)
	{
		return;
	}

	while ((*pphead)->next)
	{//不断后删 一直删除到最后
		SListEraseAfter(*pphead);
	}
	free(*pphead);//最后将头free掉
	*pphead = NULL;
}

void SListPopFront(SListNode** pphead)
{//头删
	if (*pphead == NULL)
	{
		return;
	}
	SListNode* tmp = (*pphead)->next;//记住下一个位置的地址
	free(*pphead);//删除当前的头
	*pphead = tmp;//把下一个位置当成头
}

SListNode* SListFind(SListNode* phead, SLTDataType x)
{
	SListNode*tmp;
	for (tmp = phead; tmp; tmp=tmp->next)
	{//从头开始遍历
		if (tmp->data == x)
			return tmp;
	}
	return NULL;
}

void SListInsertAfter(SListNode* pos, SLTDataType x)
{//在pos之后插入
	SListNode* tmp = BuySListNode(x);//新节点地址给tmp
	tmp->next = pos->next;//新节点的指向的下一个位置跟pos的指向下一个位置相同
	pos->next = tmp;//然后断开之前pos指向的下一个位置，让它指向新节点地址
}

void SListEraseAfter(SListNode* pos)
{
	SListNode*tmp = pos->next;
	if (tmp == NULL)
	{
		return;
	}
	pos->next = tmp->next;
	free(tmp);
}

void SListRemove(SListNode** pphead, SLTDataType x)
{
	SListNode*tmp;
	if (*pphead == NULL)
	{
		return;
	}
	if ((*pphead)->data == x)
	{
		SListPopFront(pphead);
	}
	for (tmp = *pphead; tmp->next;tmp=tmp->next)//用了SListEraseAfter所以我们要判断tmp下一个是否存在
	{
		if (tmp->next->data == x)
		{
			SListEraseAfter(tmp);
			return;
		}
	}
}





void SListPrint(SListNode* phead)
{
	SListNode*tmp;
	for (tmp = phead; tmp; tmp=tmp->next)
	{
		printf("%d-->", tmp->data);
	}
	printf("NULL\n");
}

void SListRemoveAll(SListNode** pphead, SLTDataType x)
{
	SListNode* tmp;
	while (*pphead && (*pphead)->data == x)
	{//因为要使用后删，所以要考虑头部有想要删除的目标的情况
		SListPopFront(pphead);
	}

	for (tmp = *pphead; tmp && tmp->next;)
	{//使用后删删除指定目标
		if (tmp->next->data == x)
		{
			SListEraseAfter(tmp);
		}
		else
		{
			tmp = tmp->next;
		}
	}
}

void SListReverse(SListNode **pphead)
{
	SListNode *head = *pphead;   //此指针在每次循环中始终指向当前链表的头
	SListNode *tmp = head->next; //此指针在每次循环中始终指向要被后删头插的节点
	SListNode *oldh = *pphead;   //此指针在每次循环中始终指向原本的头结点，不会改变指向

	while (tmp) //如果tmp为空，则代表逆序结束，旧头的next已经是空的了，成为新链表的末尾
	{
		oldh->next = tmp->next; //将tmp架空，实际是后删操作的一部分
		tmp->next = head; //让tmp变成新的头，实际是头插操作的一部分 
		head = tmp; //换头
		tmp = oldh->next; //让tmp变成下次循环中待删除的节点
	}
	*pphead = head;
}

void SListReverse2(SListNode **pphead)
{
	SListNode *pre = *pphead;   //被执行操作的前一个节点
	SListNode *cur = pre->next; //被执行操作的节点
	SListNode *next = cur;      //被执行操作的后一个节点，暂时指在cur，在循环开始的时候跳转到其后一个节点

	pre->next = NULL; //此时的头，将会是转换后的尾，这里是在设置链表尾节点
	while (next)
	{
		next = next->next; //先让next变成下一个节点，之所以不放在最后，是因为会有next为空的限制
		cur->next = pre; //让原本指着后面的指到前面来（先后转）
		pre = cur; //为了下次循环而传递数据，这里是设置下次循环的上一个节点（本次执行操作的节点将会成下次循环的上一个节点）
		cur = next; //同上（本次的下一个节点将会成为下次的被执行节点）
	}

	*pphead = pre; //循环跳出后cur和next都已经指向空了，pre才是新的头
}


SListNode *getIntersectionNode(SListNode *headA, SListNode *headB)
{
	int lenA = 0;//记录A链的长度
	int lenB = 0;//记录B链的长度
	int gap = 0;//长度差
	int i;
	SListNode*tmp;
	SListNode*longerlist;
	SListNode*shorterlist; 
	for (longerlist = headA; longerlist; longerlist = longerlist->next)
	{
		lenA++;
	}
	for (shorterlist = headB; shorterlist; shorterlist = shorterlist->next)
	{
		lenB++;
	}
	gap = abs(lenA - lenB);
	longerlist = headA;
	shorterlist = headB;
	if (lenA < lenB)
	{//默认A比B长，所以当A没有B长的时候要交换两条单链
		tmp = longerlist;
		longerlist = shorterlist;
		shorterlist = tmp;
	}
	for (i = 0; i < gap; i++)
	{//长链与短链对齐
		longerlist = longerlist->next;
	}
	for (; longerlist&&shorterlist; longerlist = longerlist->next, shorterlist = shorterlist->next)
	{
		if (longerlist == shorterlist)
		{
			return longerlist;
		}
	}
	return NULL;
}

SListNode *detectCycle(SListNode *head)
{
	SListNode * fast = head;
	SListNode * slow = head;

	while (fast && slow && fast->next)
	{
		fast = fast->next->next;//fast的速度是slow的2倍
		slow = slow->next;
		if (fast == slow)
		{//fast与slow相遇
			break;
		}
	}

	for (; fast && fast->next; fast = fast->next, head = head->next)
	{
		if (fast == head)
		{//fast与head同时走，直到相遇，相遇的点就是节点
			return fast;
		}
	}
	return NULL;
}