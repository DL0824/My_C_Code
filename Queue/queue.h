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

void QueuePush(Queue* pq, QuDataType x);//β��
void QueuePop(Queue* pq);//ͷɾ
QuDataType QueueFront(Queue* pq);//ȡͷԪ��
QuDataType QueueBack(Queue* pq);//ȡβԪ��
int QueueEmpty(Queue* pq);
int QueueSize(Queue* pq);


#endif/*_QUEUE_H_*/