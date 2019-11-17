#include<stdio.h>
#include<windows.h>
//void revolve(char*left,char*right)
//{
//	while (left < right)
//	{
//		char t;
//		t = *left;
//		*left = *right;
//		*right = t;
//		left++;
//		right--;
//	}
//}
//void leftrevolve(char str[],int k)
//{
//	char* left = str;
//	char* right = str + strlen(str) - 1;
//	revolve(left, right);
//	char* start1 = str;
//	char* end1 = str + k - 1;
//	char* start2 = str + k;
//	char* end2 = str + strlen(str) - 1;
//	revolve(start1, end1);
//	revolve(start2, end2);
//}
//int Check(char str1[], char str2[])
//{
//	leftrevolve(str1, 2);
//	if (strcmp(str1, str2) == 0)
//		return 1;
//	return 0;
//}
//void left_move1(char* str, int k)
//{
//	while (k != 0)
//	{
//		char* cur = str;
//		int tmp = *cur;
//		for (int i = 0; i < strlen(str); i++)
//		{
//			str[i] = str[i + 1];
//		}
//		str[strlen(str)] = str[strlen(str) - 1];
//		str[strlen(str) - 1] = tmp;
//		k--;
//	}
//}
//void left_move2(char* str, int k)
//{
//	char* cur = str;
//	int tmp = *cur;
//	while (k != 0)
//	{
//		while (*(cur++) != '\0')
//		{
//			*cur = *(++cur);
//		}
//		*cur = tmp;
//		k--;
//	}
//}
//int main()
//{
//	char str1[] = "ABCDE";
//	char str2[] = "DEABC";
//	left_move2(str2, 2);
//	printf("%s", str1);
//	if (Check(str1, str2)==1)
//		printf("yes");
//	else
//		printf("no");
//	system("pause");
//	return 0;
//}
