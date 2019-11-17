#define _CRT_SECURE_NO_WARNINGS 1
#include"mine.h"
void Menu()//菜单
{
	printf("**************************\n");
	printf("****1.Play      2.Exit****\n");
	printf("**************************\n");
	printf("Please Select:>");
}


static int GetIndex(int start, int end)//之所以用static函数是因为在一个地方随机埋下雷之后那么这个地方就不可以再次埋雷了
{//将1传给start，COL-2即10传给end
	return rand() % (end - start + 1) + start;//虽然我们定义的数组是12*12，但是我们想让用户看到一个10*10的扫雷界面，所以返回的随机数为1~10
}


void SetMine(char mine_board[][COL], int row, int col)
{
	srand((unsigned long)time(NULL));//埋雷需要随机埋雷，所以需要随机种子
	int mine_num = MINES;
	while (mine_num)
	{
		int i_index = GetIndex(1, col - 2);//雷的坐标
		int j_index = GetIndex(1, col - 2);
		if (mine_board[i_index][j_index] == '0')
		{
			mine_board[i_index][j_index] = '1';//用1表示雷
			mine_num--;//每埋下一个雷就减少一个
		}
	}
}



int GetMineNum(char mine_board[][COL], int i, int j)//计算以输入坐标为中心的九宫格内的雷的个数
{
	return mine_board[i - 1][j - 1] + mine_board[i - 1][j] + \
		mine_board[i - 1][j + 1] + mine_board[i][j - 1] + \
		mine_board[i][j + 1] + mine_board[i + 1][j - 1] + \
		mine_board[i + 1][j] + mine_board[i + 1][j + 1] - 8 * '0';
}




void ShowBoard(char show_board[][COL], int row, int col)
{
	system("cls");
	int i = 1;
	int j = 1;
	printf("    ");
	for (i=1; i <= col - 2; i++) 
	{
		printf("%d   ", i);
	}
	printf("\n");
	for (i = 1; i <= col - 1; i++) 
	{
		printf("----");
	}
	printf("\n");
	for (i = 1; i <= row - 2; i++) 
	{
		printf("%2d|", i);
		for (j = 1; j <= col - 2; j++) 
		{
			printf(" %c |", show_board[i][j]);
		}
		printf("\n");
		int k = 1;
		for (k=1; k <= col - 1; k++) 
		{
			printf("----");
		}
		printf("\n");
	}
}


void PlayGame(char show_board[][COL], char mine_board[][COL], int row, int col)//扫雷
{
	int i = 0;
	int j = 0;
	int total = (ROW - 2)*(COL - 2);//总共100个格子
	while (1)
	{
		ShowBoard(show_board, row, col);
		printf("Pleae Enter Pos<x,y>:");
		scanf("%d%d", &i, &j);
		if (i >= 1 && i <= row - 2 && j >= 1 && j <= col - 2)
		{
			if (mine_board[i][j] == '0')//翻出来没有雷
			{
				int num = GetMineNum(mine_board, i, j);//计算周围雷的个数
				show_board[i][j] = num+'0';//将雷的个数显示出来
				total--;
			}
			else
			{
				ShowBoard(mine_board, row, col);
				printf("Gmae Over!You Lose:(\n");
				break;
			}
		}
		else
		{
			printf("Enter Error,Try Again!\n");
			continue;
		}
		if (total == MINES)
		{
			printf("You Win:)\n");
			break;
		}
	}
}
void Game()
{
	char show_board[ROW][COL];
	char mine_board[ROW][COL];
	memset(show_board, '*', sizeof(show_board));//初始化两个数组，一个是用户能看到的扫雷数组一个是埋雷的
	memset(mine_board, '0', sizeof(mine_board));
	SetMine(mine_board, ROW, COL);//埋雷
	PlayGame(show_board,mine_board, ROW, COL);//扫雷
}