#include"Sort.h"
#include"Stack.h"

void DealMergeSort(int *arr, int *tmp, int start, int end)
{//归并排序
	if (start >= end) //到了叶子
	{
		return;
	}
	int mid = (start + end) / 2; //分左右子树
	DealMergeSort(arr, tmp, start, mid); //遍历左子树
	DealMergeSort(arr, tmp, mid + 1, end); //遍历右子树
	//其实这是一个二叉树的后序遍历

	int a = start;
	int b = mid + 1;
	int c = a;
	for (; a <= mid && b <= end; c++) //当a和b其中一个的元素已经完全拷贝到c中时，循环跳出
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

	for (; a <= mid; a++, c++) //如果跳出的是b，那么把a剩下的拷贝到c里
	{
		tmp[c] = arr[a];
	}

	for (; b <= end; b++, c++) //如果跳出的是a，那么把b剩下的拷贝到c里
	{
		tmp[c] = arr[b];
	}

	int i;
	for (i = start; i <= end; i++) //a和b本质上是一个数组，就是原数组，c是临时空间，最后把c的值拷贝回原数组
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
	int tmp = arr[start];//取数组第一个数为基准值
	int flag = 1;
	while (i<j)
	{//两个if只能进一个，因为如果第一个if进去，那么tmp一定大于arr[j],if语句里又将arr[j]赋给arr[i]。
	//所以下个if中的tmp<arr[i]一定不成立
		if (flag&&tmp>arr[j])
		{//从数组尾巴开始寻找，如果比基准值小，就放到数组前面
			arr[i] = arr[j];
			flag = !flag;
		}
		if (!flag&&tmp < arr[i])
		{//从前面开始找，如果比基准值大就放到数组后面
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