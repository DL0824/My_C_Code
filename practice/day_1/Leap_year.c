#include<stdio.h>
#include<stdlib.h>

int main() 
{ 
	int year = 0;
	printf("1000~2000年间的闰年：\n");
	for (year = 1000;year <= 2000;year++) 
	{ 
		if (((year % 4 == 0) && (year % 100) != 0) || (year % 400) == 0)   
			printf("%d  ", year);
	} 
	system("pause");
	return 0; 
}
