//#define _CRT_SECURE_NO_WARNINGS 1
//#include<stdio.h>
//#include<stdlib.h>
//��������֮��������ݽ���
//int main()
//{
//	int a[10], b[10], i, t;
//	printf("����������a:\n");
//	for (i = 0;i < 10;i++)
//	{
//		scanf("%d", &a[i]);
//	}
//	printf("����������b:\n");
//	for (i = 0;i < 10;i++)
//	{
//		scanf("%d", &b[i]);
//	}
//	for (i = 0;i < 10;i++)
//	{
//		t = a[i];
//		a[i] = b[i];
//		b[i] = t;
//
//	}
//	printf("������a������Ϊ:\n");
//	for (i = 0;i < 10;i++)
//	{
//		printf("%d\t", a[i]);
//	}
//	printf("\n������b������Ϊ:\n");
//	for (i = 0;i < 10;i++)
//	{
//		printf("%d\t", b[i]);
//	}
//	system("pause");
//	return 0;
//}
//����1/1-1/2+1/3-1/4+1/5 ���� + 1/99 - 1/100 ��ֵ
//int main()
//{
//	int i;
//	double sum = 0.0, t = 1, n;
//	for (i = 1;i <=100;i++)
//	{
//		n= t / i;
//		t = -t;
//		sum = sum + n;
//	}
//	printf("%lf\n", sum);
//	system("pause");
//	return 0;
//}
//��д������һ�� 1�� 100 �����������г��ֶ��ٴ�����9��
//int main()
//{   
//	int i, n = 0;
//	for (i = 0;i < 100;i++)
//	{
//		if (i % 10 == 9)
//			n++;
//		if (i / 10 == 9)
//			n++;
//	}
//	printf("%d", n);
//	system("pause");
//	return 0;
//}
