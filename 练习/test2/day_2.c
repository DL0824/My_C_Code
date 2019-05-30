#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<stdlib.h>
int main()
{/
	int a, b, t;
	scanf("%d%d", &a, &b);
	t = a;
	a = b;
	b = t;
	printf("%d\n%d", a, b);
	system("pause");
	return 0;
}
//int main()
//{
//	int a = 10;
//	int b = 20;
//	a = a ^ b;
//	b = a ^ b;
//	a = a ^ b;
//	/*a = a + b;
//	b = a - b;
//	a = a - b;*/
//	printf("%d\n%d", a, b);
//	system("pause");
//	return 0;
//
//}
//int main()
//{
//	int arr[10], max = arr[0], i;
//	for ( i = 0;i < 10;i++)
//		scanf("%d", &arr[i]);
//	for (i = 0;i < 10;i++)
//	{
//		if (max < arr[i])
//		{
//			max = arr[i];
//		}
//	}
//	printf("最大数为:%d\n", max);
//	system("pause");
//	return 0;
//}
//int main()
//{
//	int a, b, t;
//	scanf("%d%d", &a, &b);
//	while (a%b != 0)
//	{
//		t = a % b;
//		a = b;
//		b = t;
//	}
//	printf("最大公约数为:%d\n", b);
//	system("pause");
//	return 0;
//}