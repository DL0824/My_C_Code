#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<Windows.h>

/*дһ���������ز����������� 1 �ĸ��� 
���磺 15 0000 1111 4 �� 1 */

//int count_one_bits1(unsigned int value)
//{
//	int count = 0;
//	int i = 0;
//	for (i=0; i < 32; i++)
//	{
//		if (((value >> i) & 1) == 1)
//		{
//			count++;
//		}
//	}
//	return count;
//}
//int count_one_bits(unsigned int value)
//{
//	int n = 0;                             
//	while (value)
//	{
//		n++;
//		value = value & (value - 1);
//	}
//	return n;
//}
//


/*��ȡһ�������������������е�ż��λ������λ�� 
�ֱ�������������С�*/


//void Get(int num)
//{
//	int i = 0;
//	printf("��������Ϊ��");     
//	for (i = 31;i >= 0;i -= 2)  //Ҳ�ǿ���32�Σ�ÿ����������32λ���㣩��ֻ�Ǵ�31λ��ʼ�ƶ�      
//	{          
//		printf("%d ",(num>>i)&1); //��һ�������ƶ�31λ��1��õ���һλ�����ÿ�α仯2λ�����ɵõ�����λ���     
//	}      
//	printf("\n");     
//	printf("ż������Ϊ��");      
//	for(i=30;i>=0;i-=2)  //�����ƶ�30λ������1���ý����ԭ�����ֵĵڶ�λ������28λ...      
//	{          
//		printf("%d ",(num>>i)&1);      
//	}     
//	printf("\n");    
//}
//
//


/*���һ��������ÿһλ*/


//void Print(int n)
//{
//	while (n)
//	{
//		printf("%d\n", n % 10);
//		n /= 10;
//	}
//}
//


/*����int��32λ������m��n�Ķ����Ʊ���У� 
�ж��ٸ�λ(bit)��ͬ�� 
��������: 
1999 2299 
�������:7	
*/


//void Check(int n, int m)
//{
//	int count = 0;
//	for (int i = 0; i < 32; i++)
//	{
//		if ((m & 1) != (n & 1))
//		{
//			count++;
//		}
//		m = m >> 1;
//		n = n >> 1;
//	}
//	printf("%d", count);
//}
//
//int main()
//{
//
//
//	system("pause");
//	return 0;
//}













void fun1(int num)
{
	int i = 0;
	printf("ż������:\n");
	for (i = 31; i >0; i -= 2)
	{
		printf("%d", (num >> i) & 1);
	}
	printf("\n");
	printf("��������:\n");
	for (i = 30; i >=0; i -= 2);
	{
		printf("%d", (num >> i) & 1);
	}
}






int CalcDiff(int m, int n)
{
	int p;
	int count = 0;
	p = m ^ n;
	while (p)
	{
		p = p & (p - 1);
		count++;
	}
	return count;
}



int main()
{

	printf("%d", CalcDiff(7, 4));

	system("pause");
	return 0;
}