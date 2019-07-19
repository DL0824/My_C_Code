#include"seqlist.h"

void SeqListInit(SeqList* psl, size_t capacity)
{
	psl->array = (SLDataType*)calloc(capacity, sizeof(SLDataType));
	psl->capacity = capacity;
	psl->size = 0;
} 

void SeqListDestory(SeqList* psl)
{
	if (psl->array)
	{
		free(psl->array);
		psl->array = NULL;
		psl->size = 0;
		psl->capacity = 0;
	}
}

void CheckCapacity(SeqList* psl)
{
	if (psl->size >= psl->capacity)
	{
		psl->capacity *= 2;
		psl->array = (SLDataType *)realloc(psl->array, psl->capacity * sizeof(SLDataType));
	}
}

void SeqListPushBack(SeqList* psl, SLDataType x)
{
	CheckCapacity(psl);//检查是否需要扩容
	psl->array[psl->size] = x;
	psl->size++;
}
void SeqListPopBack(SeqList* psl)
{
	psl->size--;//让实际容量自减1
}
void SeqListPushFront(SeqList* psl, SLDataType x)
{
	int i;
	CheckCapacity(psl);//检查是否需要扩容
	for (i = psl->size - 1; i >= 0; i--)
	{//把每个内容向后移位一个，从最后一个开始
		psl->array[i + 1] = psl->array[i];
	}
	psl->array[0] = x;
	psl->size++;
}
void SeqListPopFront(SeqList* psl)
{
	int i;
	for (i = 0; i < psl->size-1; i++)
	{
		psl->array[i] = psl->array[i + 1];
	}
	psl->size--;
}
int SeqListFind(SeqList* psl, SLDataType x)
{
	int i;
	for (i = 0; i < psl->size; i++)
	{
		if (x == psl->array[i])
			return i;
	}
	return -1;
}
void SeqListInsert(SeqList* psl, size_t pos, SLDataType x)
{
	int i;
	CheckCapacity(psl);//检查是否需要扩容
	for (i = psl->size - 1; i >= pos; i--)
	{//把每个内容向后移位一个，从最后一个开始
		psl->array[i + 1] = psl->array[i];
	}
	psl->array[pos] = x;
	psl->size++;
}
void SeqListErase(SeqList* psl, size_t pos)
{
	int i;
	for (i = pos; i < psl->size; i++)
	{
		psl->array[i] = psl->array[i + 1];
	}
	psl->size--;
}
void SeqListRemove(SeqList* psl, SLDataType x)
{
	SeqListErase(psl, SeqListFind(psl, x));
}
void SeqListModify(SeqList* psl, size_t pos, SLDataType x)
{
	psl->array[pos] = x;
}
void SeqListPrint(SeqList* psl)
{
	int i;
	for (i = 0; i < psl->size; i++)
	{
		printf("%d ", psl->array[i]);
	}
	putchar('\n');
}


void SeqListBubbleSort(SeqList* psl)//冒泡排序
{
	int i, j;
	int tmp;
	for (i = 0; i < psl->size - 1; i++)
	{
		for (j = 0; j < psl->size - 1 - i; j++)
		{
			if (psl->array[i]>psl->array[i + 1])
			{
				tmp = psl->array[i];
				psl->array[i] = psl->array[i + 1];
				psl->array[i + 1] = tmp;
			}
		}
	}
}

int SeqListBinaryFind(SeqList* psl, SLDataType x)//二分查找
{
	int start = 0;
	int end = psl->size - 1;
	int mid = (end - start) / 2;
	while (start <= end)
	{
		if (psl->array[mid] < x)
		{
			start = mid + 1;
			start++;
			end--;
		}
		if (psl->array[mid]>x)
		{
			end = mid - 1;
			start++;
			end--;
		}
		if (psl->array[mid] = x)
		{
			return mid;
		}
	}
	return -1;
}

// 本题要求：时间复杂度：O(N) 空间复杂度 O(1)
void SeqListRemoveAll(SeqList* psl, SLDataType x)
{
	int i;
	int gap=0;
	for (i = 0; i < psl->size; i++)
	{
		if (psl->array[i] == x)
		{
			gap++;
		}
		else
		{
			psl->array[i - gap] = psl->array[i];
		}
	}
	psl->size -= gap;
}