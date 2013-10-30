#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{

	if (argc!=2) 
	{
		printf("No child`s count! \n");
		exit(EXIT_FAILURE);
	}
int i;
for (i=0;i<atoi(argv[1]);i++)
{
int status;
int cpid = fork();
	if (cpid == -1) 
	{	
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (cpid == 0) 
	{            /* Code executed by child */
		printf("Child PID is %ld\n", (long) getpid());
		exit(0);
	} else 
	{                    /* Code executed by parent */
	 do 	{
                   int w = waitpid(cpid, &status, WUNTRACED | WCONTINUED);
		}
	  while (!WIFEXITED(status) && !WIFSIGNALED(status));
	
	}
}
return 0;
}
