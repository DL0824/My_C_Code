#include"queue.h"

void QueueInit(Queue* pq)
{
	pq->front = pq->rear = pq->data;
	pq->size = 0;
}
void QueueDestory(Queue* pq)
{
	pq->front = pq->rear = pq->data;
	pq->size = 0;
}
void QueuePush(Queue* pq, QuDataType x)
{
	if (pq->size  == QueueMax)
	{
		return;
	}
	*pq->rear = x;
	pq->rear++;
	pq->size++;
	if (pq->rear - pq->data == QueueMax)
	{
		pq->rear = pq->data;
	}
}
void QueuePop(Queue* pq)
{
	if (pq->size ==0)
	{
		return;
	}
	pq->front++;
	pq->size--;
	if (pq->front - pq->data == QueueMax)
	{
		pq->front = pq->data;
	}
}
QuDataType QueueFront(Queue* pq)
{
	if (pq->size == 0)
	{
		return (QuDataType)0;
	}
	return *pq->front;
}
QuDataType QueueBack(Queue* pq)
{
	if (pq->size == 0)
	{
		return (QuDataType)0;
	}
	if (pq->rear == pq->data)
	{
		return pq->data[QueueMax - 1];
	}
	return *(pq->rear - 1);
}