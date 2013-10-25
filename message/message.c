#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct MSG 
{
  long mtype;      
  long num;    
};

int main (int argc, char** argv)
{

	if (argc != 2) 
	{
		printf ("No child`s count! \n");
		exit (EXIT_FAILURE);	  
	}
	  
	long i;
	struct MSG msg;
	ssize_t size
	int msgid; 
	long count; 

	count = atol(argv[1]);
	msgid = msgget(ftok(argv[0], 0), 0664|IPC_CREAT);  
	size = sizeof (struct MSG) - sizeof (long);

	for (i=1; i<=count; i++)
	{
		int pid = fork();
		
		if (pid == 0)
		{
			msgrcv (msgid, &msg, size, i, 0);
			
			printf ("%ld \n", i);
			fflush (stdout);
			
			msg . mtype = i + 1;
			
			msgsnd (msgid, &msg, size, 0); 
			exit(0);
		}      
	}

	msg . mtype = 1;
	msg . num = 0;
	
	msgsnd (msgid, &msg, size, 0);
	msgrcv (msgid, &msg, size, count+1, 0);
		  
	exit(0);
}
