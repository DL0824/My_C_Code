#define _CRT_SECURE_NO_WARNINGS 1
#include"chess.h"

void menu()
{
	printf("***************************\n");
	printf("**1.Play           2.exit**\n");
	printf("***************************\n");
	printf("Please Select:>");
}




void Computer(char board[][COL], int row, int col)
{
	while (1)
	{
		int x = rand() % row;//调用rand函数生成随机数，并对其取模得到想要的随机数范围
		int y = rand() % col;
		if (board[x][y] == ' ')//判断该位置是否为空
		{
			board[x][y] = 'O';
			break;
		}
	}
}




void Player(char board[][COL], int row, int col)
{
	int x, y;
	while (1)
	{
		printf("Please Enter Your Pos(x,y):>");
		scanf("%d%d", &x, &y);
		if (x >= 1 && x <= row && y >= 1 && y <= col)//判断用户输入的坐标是否满足条件，在棋盘之内
		{
			if (board[x - 1][y - 1] == ' ')//用户输入的下标从1开始，而数组从0开始，所以减1
			{
				board[x - 1][y - 1] = 'X';
				break;
			}
			else
			{
				printf("Enter Pos Is Not Fine!Try Again!\n");
			}
		}
		else
		{
			printf("Enter Error, Try Again!\n");
		}
	}
}









char Judge(char board[][COL], int row, int col)
{
	int i = 0;
	int j = 0;
	for (i=0;i < row;i++)//判断每一行走的棋子是否相同
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
			return board[i][0];
	}
	for (j=0;j < col;j++)//判断每一列走的棋子是否相同
	{
		if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != ' ')
			return board[0][j];
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')//判断左对角线走的棋子是否相同
		return board[1][1];
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')//判断右对角线走的棋子是否相同
		return board[1][1];
	for (i=0;i < row;i++)
	{
		for (j = 0;j < col;j++)
		{
			if (board[i][j] == ' ')
				return 'N';//返回'N'说明棋盘上还有空格继续走
		}
	}
	return 'D';//返回'D'表示平局
}





void Show(char board[][COL], int row, int col)
{
	system("cls");//每次显示棋盘之前先清空
	int i = 0;
	int j = 0;
	printf("    1   2   3\n");
	printf("---------------\n");
	for (i=0;i < row;i++)
	{
		printf("%d |", i + 1);//i+1表示给人看到的坐标为1
		for (j = 0;j < col;j++)
		{
			printf(" %c |",board[i][j]);
		}
		printf("\n---------------\n");
	}
	printf("\n");
}




void Game()
{
	char board[ROW][COL];//定义数组棋盘
	memset(board, ' ', sizeof(board));//整体初始化棋盘，都设置为空，函数头文件为#include<string.h>
	char result = 'N';
	srand((unsigned long)time(NULL));//布置随机种子
	while (1)
	{
		Computer(board, ROW, COL);
		Show(board, ROW, COL);
		result=Judge(board, ROW, COL);
		if (result != 'N')//人走'X'，电脑走'O'，空格返回'N'，平局返回'D'
			break;
		Player(board, ROW, COL);
		Show(board, ROW, COL);
		result = Judge(board, ROW, COL);
		if (result != 'N')//每次电脑或者玩家走完之后都需要判断输赢
			break;
	}
	switch (result)
	{
	case 'X':
		printf("Good Job You Win!\n");
		break;
	case 'O':
		printf("Sorry You Lose!\n");
		break;
	case 'D':
		printf("平局耶！真棒！\n");
		break;
	default:
		break;
	}
}