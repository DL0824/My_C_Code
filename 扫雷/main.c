#define _CRT_SECURE_NO_WARNINGS 1
#include"mine.h"
int main()
{
	int select = 0;
	int n = 0;
	while (!n)
	{
		Menu();
		scanf("%d", &select);
		switch (select)
		{
		case 1:
			Game();
			break;
		case 2:
			printf("GoodBye!\n");
			n = 1;
			break;
		default:
			break;
		}
	}
	system("pause");
	return 0;
}