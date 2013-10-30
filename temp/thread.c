#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

int count=0;
void *proc(void * var)
{
	printf("%d\n",(int)var);
	pthread_exit(var);
}

int main(int argc, char** argv)
{

	if (argc!=2) 
	{
		exit(EXIT_FAILURE);
	}

int i;
pthread_t id[10000];
void * var;
	for (i=0;i<atoi(argv[1]);i++)
	{
		pthread_create(&id[count],NULL,&proc,(void *)count);
		count++;
	}
	count=0;
	for (i=0;i<atoi(argv[1]);i++)
	{
		pthread_join(id[count],&var);
		count++;
	}
return 0;
}
