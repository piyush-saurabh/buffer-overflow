#include<stdio.h>

int sum(int a, int b)
{
	int c;
	c = a+b;
	return c;
}

main()
{
	int a = 20, b = 30;
	int result;
	result = sum(a,b);
	printf("The result is : %d\n", result);
}
