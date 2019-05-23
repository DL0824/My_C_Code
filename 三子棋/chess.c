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
		int x = rand() % row;//����rand���������������������ȡģ�õ���Ҫ���������Χ
		int y = rand() % col;
		if (board[x][y] == ' ')//�жϸ�λ���Ƿ�Ϊ��
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
		if (x >= 1 && x <= row && y >= 1 && y <= col)//�ж��û�����������Ƿ�����������������֮��
		{
			if (board[x - 1][y - 1] == ' ')//�û�������±��1��ʼ���������0��ʼ�����Լ�1
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
	for (i=0;i < row;i++)//�ж�ÿһ���ߵ������Ƿ���ͬ
	{
		if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ')
			return board[i][0];
	}
	for (j=0;j < col;j++)//�ж�ÿһ���ߵ������Ƿ���ͬ
	{
		if (board[0][j] == board[1][j] && board[1][j] == board[2][j] && board[0][j] != ' ')
			return board[0][j];
	}
	if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[1][1] != ' ')//�ж���Խ����ߵ������Ƿ���ͬ
		return board[1][1];
	if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[1][1] != ' ')//�ж��ҶԽ����ߵ������Ƿ���ͬ
		return board[1][1];
	for (i=0;i < row;i++)
	{
		for (j = 0;j < col;j++)
		{
			if (board[i][j] == ' ')
				return 'N';//����'N'˵�������ϻ��пո������
		}
	}
	return 'D';//����'D'��ʾƽ��
}





void Show(char board[][COL], int row, int col)
{
	system("cls");//ÿ����ʾ����֮ǰ�����
	int i = 0;
	int j = 0;
	printf("    1   2   3\n");
	printf("---------------\n");
	for (i=0;i < row;i++)
	{
		printf("%d |", i + 1);//i+1��ʾ���˿���������Ϊ1
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
	char board[ROW][COL];//������������
	memset(board, ' ', sizeof(board));//�����ʼ�����̣�������Ϊ�գ�����ͷ�ļ�Ϊ#include<string.h>
	char result = 'N';
	srand((unsigned long)time(NULL));//�����������
	while (1)
	{
		Computer(board, ROW, COL);
		Show(board, ROW, COL);
		result=Judge(board, ROW, COL);
		if (result != 'N')//����'X'��������'O'���ո񷵻�'N'��ƽ�ַ���'D'
			break;
		Player(board, ROW, COL);
		Show(board, ROW, COL);
		result = Judge(board, ROW, COL);
		if (result != 'N')//ÿ�ε��Ի����������֮����Ҫ�ж���Ӯ
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
		printf("ƽ��Ү�������\n");
		break;
	default:
		break;
	}
}