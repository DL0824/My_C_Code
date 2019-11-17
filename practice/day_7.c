#define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>
#include<Windows.h>
#include<assert.h>


int MyLen(char* str)
{
	assert(*str != NULL);//断言，这是一个宏！会帮助你找到错误只在Debug环境下有用
	int n = 0;
	while (*str != '\0')
	{
		str++;
		n++;
	}
	return n;
}

int fab1(int n)
{
	if (n <= 2)
		return 1;
	else
		return fab1(n - 1) + fab1(n - 2);
}
int fab2(int n)
{
	int a = 1, b = 1, c = 1;
	while (n > 2)
	{
		c = a + b;
		a = b;
		b = c;
	}
	return c;
}



int np(int n, int k)
{
	if (k == 0)
		return 1;
	return n * np(n, k - 1);
}






/*
一个数字%上10的几次方就是取出这个数的后几位，
一个数字如果/上10的几次方就是取出这个数的前几位
*/
int DigitSum(int n)

{
	if (n < 10)
		return n;
	else
		return n % 10 + DigitSum(n / 10);
}

void reverse_string1(char * string)
{

}



void reverse_string(char * string)
{
	if (*string != '\0')
	{   //判断string中ASCII值是否为0,在C语言中以'\0'作为结束字符		
		string++;  //一直向后进行统计元素 		
		reverse_string(string);
		printf("%c", *(string - 1));   // string-1指字符串中从最后一个元素输出 	
	}
}



int my_strlen1(char* str)
{
	int n = 0;
	while (*str++)
	{
		n++;
	}
	return n;
}
int my_strlen2(char*str)
{
	static int n = 0;
	if (*str != '\0')
	{
		str++;
		my_strlen2(str);
	}
	return n++;
}




int fac1(int n)
{
	int i = 0;
	for (i = 1;n > 1;n--)
		i *= n;
	return i;
}
int fac2(int n)
{
	if (n > 0)
		return n * fac2(n - 1);
	else
		return 1;
}



void Print(int n)
{
	if(n>9)
		Print(n / 10);
	printf("%d\n", n % 10);
}

int main()
{
	char* string = "abcd";
	reverse_string(string);
	system("pause");
	return 0;
}