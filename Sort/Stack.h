#ifndef _STACK_H_
#define _STACK_H_

#include<stdio.h>
#include<stdlib.h>


typedef int * StackData;

typedef struct
{
	StackData * array; // ָ��̬���ٵ�����
	size_t size; // ��Ч���ݸ���
	size_t capacity; // �����ռ�Ĵ�С
}StackNode;

void StackInit(StackNode* psl, size_t capacity);
void StackDestory(StackNode* psl);
void CheckCapacity(StackNode* psl);
void StackPush(StackNode* psl, StackData x);
void StackPop(StackNode* psl);
StackData StackFront(StackNode*psl);
size_t IsStackEmpty(StackNode*psl);

#endif/*_STACK_H_*/