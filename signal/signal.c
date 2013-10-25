#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>

int connect;
char data;
int childpid;
int parentpid;

char pow2 (char n)
{
	if (n==0) return 1;
	return 2*pow2(n-1);
}
void getChildSignalDie()
{
		kill(childpid, SIGKILL);
		exit(0);
}

void getParentSignal()
{
	connect=1;
}

void getChildSignal0()
{
	connect=1;
	data=0;
}

void getChildSignal1()
{
	connect=1;
	data=1;
}

int main(int argc, char** argv)
{
parentpid = getpid();
childpid = fork();


if (childpid == -1) 
{	
		perror("fork");
		exit(EXIT_FAILURE);
}
	
 /* Code executed by child */	
if (childpid == 0) 
{           
		int readresult;
		int o;
		int n;
		FILE *f = fopen("1.txt", "rb");
		if (!f)
		{
			perror("Opening file: ");
			return -1;
		}
		sigset_t set;
		
		sigemptyset(&set);
		sigaddset(&set,SIGPIPE);
		sigprocmask(SIG_BLOCK,&set,NULL);
		
		struct sigaction act;
		
		memset(&act,0,sizeof(act));
		act.sa_handler=getParentSignal;
		sigaction(SIGPIPE,&act,NULL);
		n=0;
		readresult=fscanf(f,"%c",&data);
		while (readresult>0)
		{
			
			connect=0;
			
			sigprocmask(SIG_UNBLOCK,&set,NULL);
			
			while (!connect) {}
			sigprocmask(SIG_BLOCK,&set,NULL);
			if (n==8) readresult=fscanf(f,"%c",&data);
			else
			{
			o=data%2;
			data=(data-o)/2; 
			printf("%c",o);
			n++;
			}
			if (data=='0') kill(parentpid, SIGUSR1);
			else kill(parentpid, SIGUSR2);
		}

		exit(0);
} else 
	
/* Code executed by parent */	
{                    
		connect=0;
		int n=0;;
		char o=0;
		sigset_t set0,set1,setdie;
		
		sigemptyset(&set0);
		sigaddset(&set0,SIGUSR1);
		
		sigemptyset(&set1);
		sigaddset(&set1,SIGUSR2);
		
		sigemptyset(&setdie);
		sigaddset(&setdie,SIGCHLD);
		
		struct sigaction act0,act1,actdie;
		
		memset(&act0,0,sizeof(act0));
		act0.sa_handler=getChildSignal0;
		sigaction(SIGUSR1,&act0,NULL);
		
		memset(&act1,0,sizeof(act1));
		act1.sa_handler=getChildSignal1;
		sigaction(SIGUSR2,&act1,NULL);
		
		memset(&actdie,0,sizeof(actdie));
		actdie.sa_handler=getChildSignalDie;
		sigaction(SIGCHLD,&actdie,NULL);
		
		sigprocmask(SIG_UNBLOCK,&set0,NULL);
		sigprocmask(SIG_UNBLOCK,&set1,NULL);
		sigprocmask(SIG_UNBLOCK,&setdie,NULL);
		
		sleep(1);
		
		kill(childpid, SIGPIPE);
		
		while (1)
		{
			while (!connect) {};
			sigprocmask(SIG_BLOCK,&set0,NULL);
			sigprocmask(SIG_BLOCK,&set1,NULL);
			
			if (n<7) 
			{
				o=o+data;
				o=o*2;
				n++;
			}else
			{	
				printf("%c",o);
				o=0;
				n=0;
			}
			
			//printf("%c",data);
			kill(childpid, SIGPIPE);
			sigprocmask(SIG_UNBLOCK,&set0,NULL);
			sigprocmask(SIG_UNBLOCK,&set1,NULL);		
		}
		
wait(NULL);
	
}

return 0;
}
