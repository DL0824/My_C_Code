#define _CRT_SECURE_NO_WARNINGS 1
#include"chess.h"
int main()
{
	
	int Select = 0;
	int n = 0;
	while (!n)
	{
		menu();//打印菜单
		scanf("%d", &Select);//让用户输入选择
		switch (Select)
		{
		case 1:
			Game();//选择1开始玩游戏
			break;
		case 2:
			n = 1;//将1赋给n，跳出循环
			printf("See you next time!\n");
			break;
		default:
			printf("Please Enter Again!\n");
			break;
		}
	}
	system("pause");
	return 0;
}