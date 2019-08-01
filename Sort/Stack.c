#include"Stack.h"
void StackInit(StackNode* psl, size_t capacity)
{
	psl->array = (StackData*)calloc(capacity, sizeof(StackData));
	psl->capacity = capacity;
	psl->size = 0;
}
void StackDestory(StackNode* psl)
{
	if (psl->array)
	{
		free(psl->array);
		psl->array = NULL;
		psl->size = 0;
		psl->capacity = 0;
	}
}
void CheckCapacity(StackNode* psl)
{
	if (psl->size >= psl->capacity)
	{
		psl->capacity *= 2;
		psl->array = (StackData *)realloc(psl->array, psl->capacity * sizeof(StackData));
	}
}
void StackPush(StackNode* psl, StackData x)
{
	CheckCapacity(psl);//检查是否需要扩容
	psl->array[psl->size] = x;
	psl->size++;
}
void StackPop(StackNode* psl)
{
	if (psl->size == 0)
	{
		return;
	}
	psl->size--;//让实际容量自减1
}
StackData StackFront(StackNode*psl)
{
	if (psl->size == 0)
	{
		return (StackData)0;
	}
	return psl->array[psl->size - 1];
}
size_t IsStackEmpty(StackNode*psl)
{
	return psl->size == 0;
}