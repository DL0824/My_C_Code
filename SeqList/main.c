#include"seqlist.h"

int main()
{
	SeqList test;
	SeqListInit(&test,100);
	SeqListPushBack(&test, 10);
	SeqListPushBack(&test, 8);
	SeqListPushBack(&test, 7);
	SeqListPushBack(&test, 4);
	SeqListPushBack(&test, 2);
	SeqListPushBack(&test, 2);
	SeqListPushBack(&test, 2);
	SeqListPushBack(&test, 3);
	SeqListPushBack(&test, 2);
	SeqListRemoveAll(&test, 2);
	//SeqListPopFront(&test);

	SeqListPrint(&test);
	SeqListDestory(&test);
	system("pause");
	return 0;
}