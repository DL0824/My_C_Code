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
{//ͷ��
	if (*pphead == NULL)
	{//ͷλ��ֱ�Ӳ���
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
	{//���Ϻ�ɾ һֱɾ�������
		SListEraseAfter(*pphead);
	}
	free(*pphead);//���ͷfree��
	*pphead = NULL;
}

void SListPopFront(SListNode** pphead)
{//ͷɾ
	if (*pphead == NULL)
	{
		return;
	}
	SListNode* tmp = (*pphead)->next;//��ס��һ��λ�õĵ�ַ
	free(*pphead);//ɾ����ǰ��ͷ
	*pphead = tmp;//����һ��λ�õ���ͷ
}

SListNode* SListFind(SListNode* phead, SLTDataType x)
{
	SListNode*tmp;
	for (tmp = phead; tmp; tmp=tmp->next)
	{//��ͷ��ʼ����
		if (tmp->data == x)
			return tmp;
	}
	return NULL;
}

void SListInsertAfter(SListNode* pos, SLTDataType x)
{//��pos֮�����
	SListNode* tmp = BuySListNode(x);//�½ڵ��ַ��tmp
	tmp->next = pos->next;//�½ڵ��ָ�����һ��λ�ø�pos��ָ����һ��λ����ͬ
	pos->next = tmp;//Ȼ��Ͽ�֮ǰposָ�����һ��λ�ã�����ָ���½ڵ��ַ
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
	for (tmp = *pphead; tmp->next;tmp=tmp->next)//����SListEraseAfter��������Ҫ�ж�tmp��һ���Ƿ����
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
	{//��ΪҪʹ�ú�ɾ������Ҫ����ͷ������Ҫɾ����Ŀ������
		SListPopFront(pphead);
	}

	for (tmp = *pphead; tmp && tmp->next;)
	{//ʹ�ú�ɾɾ��ָ��Ŀ��
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
	SListNode *head = *pphead;   //��ָ����ÿ��ѭ����ʼ��ָ��ǰ�����ͷ
	SListNode *tmp = head->next; //��ָ����ÿ��ѭ����ʼ��ָ��Ҫ����ɾͷ��Ľڵ�
	SListNode *oldh = *pphead;   //��ָ����ÿ��ѭ����ʼ��ָ��ԭ����ͷ��㣬����ı�ָ��

	while (tmp) //���tmpΪ�գ�����������������ͷ��next�Ѿ��ǿյ��ˣ���Ϊ�������ĩβ
	{
		oldh->next = tmp->next; //��tmp�ܿգ�ʵ���Ǻ�ɾ������һ����
		tmp->next = head; //��tmp����µ�ͷ��ʵ����ͷ�������һ���� 
		head = tmp; //��ͷ
		tmp = oldh->next; //��tmp����´�ѭ���д�ɾ���Ľڵ�
	}
	*pphead = head;
}

void SListReverse2(SListNode **pphead)
{
	SListNode *pre = *pphead;   //��ִ�в�����ǰһ���ڵ�
	SListNode *cur = pre->next; //��ִ�в����Ľڵ�
	SListNode *next = cur;      //��ִ�в����ĺ�һ���ڵ㣬��ʱָ��cur����ѭ����ʼ��ʱ����ת�����һ���ڵ�

	pre->next = NULL; //��ʱ��ͷ��������ת�����β������������������β�ڵ�
	while (next)
	{
		next = next->next; //����next�����һ���ڵ㣬֮���Բ������������Ϊ����nextΪ�յ�����
		cur->next = pre; //��ԭ��ָ�ź����ָ��ǰ�������Ⱥ�ת��
		pre = cur; //Ϊ���´�ѭ�����������ݣ������������´�ѭ������һ���ڵ㣨����ִ�в����Ľڵ㽫����´�ѭ������һ���ڵ㣩
		cur = next; //ͬ�ϣ����ε���һ���ڵ㽫���Ϊ�´εı�ִ�нڵ㣩
	}

	*pphead = pre; //ѭ��������cur��next���Ѿ�ָ����ˣ�pre�����µ�ͷ
}


SListNode *getIntersectionNode(SListNode *headA, SListNode *headB)
{
	int lenA = 0;//��¼A���ĳ���
	int lenB = 0;//��¼B���ĳ���
	int gap = 0;//���Ȳ�
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
	{//Ĭ��A��B�������Ե�Aû��B����ʱ��Ҫ������������
		tmp = longerlist;
		longerlist = shorterlist;
		shorterlist = tmp;
	}
	for (i = 0; i < gap; i++)
	{//�������������
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
		fast = fast->next->next;//fast���ٶ���slow��2��
		slow = slow->next;
		if (fast == slow)
		{//fast��slow����
			break;
		}
	}

	for (; fast && fast->next; fast = fast->next, head = head->next)
	{
		if (fast == head)
		{//fast��headͬʱ�ߣ�ֱ�������������ĵ���ǽڵ�
			return fast;
		}
	}
	return NULL;
}