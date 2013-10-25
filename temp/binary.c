#include <stdio.h>
#include <stdlib.h>


int main()
{
	printf("Enter mode: \n");
	printf("1 - char to bin\n");
	printf("2 - bin to char\n");
	printf("0 - exit:\n");
	printf("> ");
	int mode;
	scanf("%d",&mode);
	getchar();
	if (mode==1)
	{
		char c;
		scanf("%c",&c);
		int i;
		for (i=0;i<8;i++) 
		{
			char ai=0;
			ai=(c&(1<<i))>>i;
			printf("%d",ai);
		}
		printf("\n");
		return 0;
	}
	if (mode==2)
	{
		char c=0;
		int i;
		for (i=0;i<8;i++)
		{
			int ai;
			scanf("%d",&ai);
			c=c+(ai<<i);
		}
		printf("%c\n",c);
	}
	return 0;
}

