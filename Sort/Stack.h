#ifndef _STACK_H_
#define _STACK_H_

#include<stdio.h>
#include<stdlib.h>


typedef int * StackData;

typedef struct
{
	StackData * array; // 指向动态开辟的数组
	size_t size; // 有效数据个数
	size_t capacity; // 容量空间的大小
}StackNode;

void StackInit(StackNode* psl, size_t capacity);
void StackDestory(StackNode* psl);
void CheckCapacity(StackNode* psl);
void StackPush(StackNode* psl, StackData x);
void StackPop(StackNode* psl);
StackData StackFront(StackNode*psl);
size_t IsStackEmpty(StackNode*psl);

#endif/*_STACK_H_*/