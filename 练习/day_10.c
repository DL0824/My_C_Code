#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<Windows.h>

/*写一个函数返回参数二进制中 1 的个数 
比如： 15 0000 1111 4 个 1 */

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


/*获取一个数二进制序列中所有的偶数位和奇数位， 
分别输出二进制序列。*/


//void Get(int num)
//{
//	int i = 0;
//	printf("奇数序列为：");     
//	for (i = 31;i >= 0;i -= 2)  //也是控制32次（每个整型数按32位来算），只是从31位开始移动      
//	{          
//		printf("%d ",(num>>i)&1); //第一次向右移动31位和1与得到第一位情况，每次变化2位，即可得到奇数位情况     
//	}      
//	printf("\n");     
//	printf("偶数序列为：");      
//	for(i=30;i>=0;i-=2)  //向右移动30位，再与1所得结果是原来数字的第二位，再移28位...      
//	{          
//		printf("%d ",(num>>i)&1);      
//	}     
//	printf("\n");    
//}
//
//


/*输出一个整数的每一位*/


//void Print(int n)
//{
//	while (n)
//	{
//		printf("%d\n", n % 10);
//		n /= 10;
//	}
//}
//


/*两个int（32位）整数m和n的二进制表达中， 
有多少个位(bit)不同？ 
输入例子: 
1999 2299 
输出例子:7	
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
	printf("偶数序列:\n");
	for (i = 31; i >0; i -= 2)
	{
		printf("%d", (num >> i) & 1);
	}
	printf("\n");
	printf("奇数序列:\n");
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