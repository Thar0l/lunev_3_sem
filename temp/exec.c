#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	if (argc==1) exit(0);
	execvp(argv[1],&argv[1]);
	printf("Can`t run process!\n");
	return 0;
}
