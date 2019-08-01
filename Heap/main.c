#include"Heap.h"

#define ARRSIZE(a) (sizeof(a) / sizeof(a[0]))

int main()
{
	int a[] = { 1, 6, 3, 7, 9, 8 };

	Heap hp;

	HeapInit(&hp, a, ARRSIZE(a));
	HeapPrint(&hp);
	putchar('\n');
	HeapPop(&hp);
	//HeapPush(&hp, 5);
	HeapPrint(&hp);
	putchar('\n');
	//HeapPush(&hp, 10);
	//HeapPrint(&hp);


	system("pause");
	return 0;
}