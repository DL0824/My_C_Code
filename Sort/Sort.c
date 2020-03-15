#include"Sort.h"
#include"Stack.h"

void DealMergeSort(int *arr, int *tmp, int start, int end)
{//�鲢����
	if (start >= end) //����Ҷ��
	{
		return;
	}
	int mid = (start + end) / 2; //����������
	DealMergeSort(arr, tmp, start, mid); //����������
	DealMergeSort(arr, tmp, mid + 1, end); //����������
	//��ʵ����һ���������ĺ������

	int a = start;
	int b = mid + 1;
	int c = a;
	for (; a <= mid && b <= end; c++) //��a��b����һ����Ԫ���Ѿ���ȫ������c��ʱ��ѭ������
	{
		if (arr[a] >= arr[b])
		{
			tmp[c] = arr[b];
			b++;
		}
		else
		{
			tmp[c] = arr[a];
			a++;
		}
	}

	for (; a <= mid; a++, c++) //�����������b����ô��aʣ�µĿ�����c��
	{
		tmp[c] = arr[a];
	}

	for (; b <= end; b++, c++) //�����������a����ô��bʣ�µĿ�����c��
	{
		tmp[c] = arr[b];
	}

	int i;
	for (i = start; i <= end; i++) //a��b��������һ�����飬����ԭ���飬c����ʱ�ռ䣬����c��ֵ������ԭ����
	{
		arr[i] = tmp[i];
	}
}

void MergeSort(int *arr, int n)
{
	int *tmp = (int *)malloc(sizeof(int)*n);
	DealMergeSort(arr, tmp, 0, n - 1);
	free(tmp);
}


void swapArgs(int * a, int * b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void dealQSort(int * arr, int start, int end)
{
	int flag = 1;

	if (start >= end)
	{
		return;
	}

	int i = start, j = end;
	while (i < j)
	{
		if (arr[i] > arr[j])
		{
			swapArgs(arr + i, arr + j);
			flag = !flag;
		}

		flag ? j-- : i++;
	}

	dealQSort(arr, start, i - 1); 
	dealQSort(arr, i + 1, end);
}

void Qsort(int * arr, int n)
{
	dealQSort(arr, 0, n - 1);
}

void dealQsort_w(int *arr, int start, int end)
{
	int i = start;
	int j = end;
	int tmp = arr[start];//ȡ�����һ����Ϊ��׼ֵ
	int flag = 1;
	while (i<j)
	{//����ifֻ�ܽ�һ������Ϊ�����һ��if��ȥ����ôtmpһ������arr[j],if������ֽ�arr[j]����arr[i]��
	//�����¸�if�е�tmp<arr[i]һ��������
		if (flag&&tmp>arr[j])
		{//������β�Ϳ�ʼѰ�ң�����Ȼ�׼ֵС���ͷŵ�����ǰ��
			arr[i] = arr[j];
			flag = !flag;
		}
		if (!flag&&tmp < arr[i])
		{//��ǰ�濪ʼ�ң�����Ȼ�׼ֵ��ͷŵ��������
			arr[j] = arr[i];
			flag = !flag;
		}
		flag ? j-- : i++;
	}
	arr[i] = tmp;
	dealQsort_w(arr, start, i - 1);
	dealQsort_w(arr, i+1, end);
}
void Qsort_w(int *arr, int n)
{
	dealQsort_w(arr, 0, n - 1);
}

int Partition(int *arr, int start, int end)
{
	int mid = (start + end) / 2;
	if (arr[start] > arr[end])
	{
		swapArgs(arr + start, arr + end);
	}
	if (arr[mid] > arr[end])
	{
		swapArgs(arr + mid, arr + end);
	}
	if (arr[start] > arr[mid])
	{
		swapArgs(arr + start, arr + mid);
	}
	if (end - start <= 2)
	{
		return mid;
	}
	swapArgs(arr + mid, arr + end - 1);
	int i = start, j = end;
	while (i < j)
	{
		while (arr[i] < arr[end - 1])
		{
			i++;
		}
		while (arr[j]>arr[end - 1])
		{
			j--;
		}
		swapArgs(arr + i, arr + j);
		j--;
		i++;
	}
	swapArgs(arr + i, arr + end - 1);
	return i;
}
void dealHQsort(int *arr, int start, int end)
{
	if (end - start > INSERT_SORT_LENTH)
	{
		int pivot = Partition(arr, start, end);
		dealHQsort(arr, start, pivot - 1);
		dealHQsort(arr, pivot + 1, end);
	}
	else
	{
		InsertSort(arr + start, end - start + 1);
	}
}
void HQsort(int *arr, int n)
{
	dealHQsort(arr, 0, n - 1);
}

void InsertSort(int *arr, int n)
{
	int i, j;
	int tmp;
	for (i = 1; i < n; i++)
	{
		tmp = arr[i];
		for (j = i; j>0 && arr[j - 1] > tmp; j--)
		{
			arr[j] = arr[j - 1];
		}
		arr[j] = tmp;
	}
}

void QuikSortNonR(int* a, int left, int right)
{
	StackNode st;
	StackInit(&st,100);
	StackPush(&st, left);
	StackPush(&st, right);
	while (!IsStackEmpty(&st))
	{
		int end = StackFront(&st);
		StackPop(&st);
		int begin = StackFront(&st);
		StackPop(&st);
		int div = Partition(a, begin, end);
		if (begin < div - 1)
		{
			StackPush(&st, begin);
			StackPush(&st, div - 1);
		}

		if (div + 1 < end)
		{
			StackPush(&st, div + 1);
			StackPush(&st, end);
		}
	}
}