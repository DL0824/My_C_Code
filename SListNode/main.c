#include"SListNode.h"

int main()
{
	SListNode *phead;

	SListInit(&phead);

	SListPushFront(&phead, 1);
	SListPushFront(&phead, 2);
	SListPushFront(&phead, 3);
	SListPushFront(&phead, 4);
	SListPushFront(&phead, 5);
	SListPushFront(&phead, 6);
	SListPushFront(&phead, 7);
	SListPushFront(&phead, 8);
	SListPushFront(&phead, 9);
	SListPushFront(&phead, 9);
	SListPushFront(&phead, 9);
	SListPopFront(&phead);
	/*SListInsertAfter(SListFind(phead, 6), 9);
	SListEraseAfter(SListFind(phead, 4));
	SListRemove(&phead, 7);*/
	SListRemoveAll(&phead, 9);
	//SListReverse(&phead);
	//SListReverse2(&phead);
	SListPrint(phead);
	SListDestory(&phead);
	system("pause");
	return 0;
}

int yuesefu()
{
	SListNode *phead;
	SListNode *plast;
	SListNode *cur;
	int m = 50, n = 3;
	int i;
	SListInit(&phead);

	SListPushFront(&phead, m);
	plast = phead;
	for (i = m - 1; i >= 1; i--)
	{
		SListPushFront(&phead, i);
	}
	plast->next = phead;

	cur = plast;
	for (; m > 1; m--)
	{
		for (i = 1; i < n; i++)
		{
			cur = cur->next;
			printf("%d号报%d\n", cur->data, i);
		}
		printf("%d号出列\n", cur->next->data);
		SListEraseAfter(cur);
	}

	printf("%d号胜利\n", cur->data);

	free(cur);
	system("pause");
	return 0;
}
