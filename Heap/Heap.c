#include"Heap.h"

void swap(HPDataType *a, HPDataType *b)
{
	HPDataType tmp;
	tmp = *a;
	*a = *b;
	*b = tmp;
}

void DownAdjust(Heap *hp, int n)
{
	int cur = n;
	while (cur * 2 + 1 < hp->_size) //���һ���ѵĽڵ�û����ڵ㣬��ô������Ҷ�ӣ�����ֱ������
	{
		if (hp->_data[cur] < hp->_data[cur * 2 + 1]) //������У�����ұȵ�����С����ôҪִ�н���
		{
			if (cur * 2 + 2 >= hp->_size ||  //����Һ��Ӳ����ڣ�ֱ�ӽ���
				hp->_data[cur * 2 + 1] >= hp->_data[cur * 2 + 2]) //������ӱ��Һ��Ӵ���ô�Ҹ����ӻ�
			{
				swap(&hp->_data[cur], &hp->_data[cur * 2 + 1]); //��ֵ
				cur = cur * 2 + 1; //��λ
			}
			else //����Һ��Ӵ����ұ����ӻ�����ô���Һ��ӻ�
			{
				swap(&hp->_data[cur], &hp->_data[cur * 2 + 2]);
				cur = cur * 2 + 2;
			}
		}
		else if (cur * 2 + 2 < hp->_size && //����Һ��Ӳ����ڣ��Ͳ���Ƚ��Һ���
			hp->_data[cur] < hp->_data[cur * 2 + 2]) //��������Ӵ󣬵����Һ���С����ô�Ҹ��Һ��ӻ�
		{
			swap(&hp->_data[cur], &hp->_data[cur * 2 + 2]);
			cur = cur * 2 + 2;
		}
		else
		{
			break; //����ұ����Һ��Ӷ�����ôֱ������
		}
	}
}

void HeapInit(Heap *hp, HPDataType *a, int n)//n��ʾ��n���ڵ�
{
	int i;
	hp->_capacity = MAXSIZE > n ? MAXSIZE : n;
	hp->_data = (HPDataType *)malloc(hp->_capacity*sizeof(HPDataType));//��ʼ����
	hp->_size = n;
	memcpy(hp->_data, a, sizeof(HPDataType)*n);//��˳���a�����ݿ���������
	for (i = n / 2 - 1; i >= 0;i--)//�Զ�����������i = n / 2 - 1��ʾ��n���ڵ㣬�������һ������Ҷ�ӽڵ�Ľڵ���±�
	{
		DownAdjust(hp, i);
	}
}
void HeapDestory(Heap *hp)
{
	if (hp->_data)
	{
		free(hp->_data);
		hp->_data = NULL;
		hp->_size = hp->_capacity = 0;
	}
}
void HeapPush(Heap *hp, HPDataType x)
{
	int cur = hp->_size;
	if (cur == hp->_capacity)
	{
		hp->_capacity *= 2;
		hp->_data = (HPDataType *)realloc(hp->_data, hp->_capacity*sizeof(HPDataType));
	}
	hp->_data[cur] = x;
	cur++;
	while (cur)
	{
		if (hp->_data[cur] > hp->_data[(cur - 1) / 2])
		{
			swap(&hp->_data[cur], &hp->_data[(cur - 1) / 2]);
			cur = (cur - 1) / 2;
		}
		else
		{
			break;
		}
	}
}
void HeapPop(Heap *hp)
{
	swap(hp->_data, hp->_data + hp->_size - 1);
	hp->_size--;
	DownAdjust(hp, 0);
}
HPDataType HeapTop(Heap *hp);
int HeapSize(Heap *hp);
int HeapEmpty(Heap *hp);
// ������
void HeapSort(Heap *hp)
{
	int tmp = hp->_size;

	while (hp->_size >= 1)
	{
		HeapPop(hp);
	}

	hp->_size = tmp;
}
void HeapPrint(Heap *hp)
{
	int i;
	int tag = 0;
	int tmp = 1;
	for (i = 0; i < hp->_size; i++)
	{
		printf("%d ", hp->_data[i]);
		if (i == tag)
		{
			putchar('\n');
			tmp *= 2;
			tag += tmp;
		}
	}
}
int HeapSize(Heap *hp)
{
	return hp->_size;
}

int HeapEmpty(Heap *hp)
{
	return hp->_size == 0;
}
void HeapSort(Heap *hp)
{
	int tmp = hp->_size;

	while (hp->_size >= 1)
	{
		HeapPop(hp);
	}

	hp->_size = tmp;
}