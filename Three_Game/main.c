#define _CRT_SECURE_NO_WARNINGS 1
#include"chess.h"
int main()
{
	
	int Select = 0;
	int n = 0;
	while (!n)
	{
		menu();//��ӡ�˵�
		scanf("%d", &Select);//���û�����ѡ��
		switch (Select)
		{
		case 1:
			Game();//ѡ��1��ʼ����Ϸ
			break;
		case 2:
			n = 1;//��1����n������ѭ��
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