#include<stdio.h>

main()
{
	unsigned int i = 1;
	char *c = (char*)&i;
	if(*c)
		printf("The machine is Little endian\n");
	else
		printf("The machine is Big endian\n");
}
