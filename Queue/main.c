#include"queue.h"

int main()
{
	Queue qu;
	QueueInit(&qu);

	QueuePush(&qu, 1);
	printf("%d ", QueueBack(&qu));
	QueuePush(&qu, 2);
	printf("%d ", QueueBack(&qu));
	QueuePush(&qu, 3);
	printf("%d ", QueueBack(&qu));
	QueuePush(&qu, 4);
	printf("%d ", QueueBack(&qu));
	QueuePush(&qu, 5);
	printf("%d ", QueueBack(&qu));
	QueuePush(&qu, 6);
	printf("%d\t", QueueBack(&qu));
	printf("%d ", QueueFront(&qu));
	QueuePop(&qu);
	printf("%d ", QueueFront(&qu));
	QueuePop(&qu);
	printf("%d ", QueueFront(&qu));
	QueuePop(&qu);
	printf("%d ", QueueFront(&qu));
	QueuePop(&qu);
	printf("%d ", QueueFront(&qu));
	QueuePush(&qu, 5);
	printf("%d ", QueueBack(&qu));
	


	QueueDestory(&qu);
	system("pause");
	return 0;
}