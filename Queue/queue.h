#ifndef _QUEUE_H_
#define _QUEUE_H_

#include<stdio.h>
#include<Windows.h>

#define QueueMax 5
typedef int QuDataType;

typedef struct
{
	QuDataType data[QueueMax];
	size_t size;
	QuDataType*front;
	QuDataType*rear;
}Queue;

void QueueInit(Queue* pq);
void QueueDestory(Queue* pq);

void QueuePush(Queue* pq, QuDataType x);//尾插
void QueuePop(Queue* pq);//头删
QuDataType QueueFront(Queue* pq);//取头元素
QuDataType QueueBack(Queue* pq);//取尾元素
int QueueEmpty(Queue* pq);
int QueueSize(Queue* pq);


#endif/*_QUEUE_H_*/