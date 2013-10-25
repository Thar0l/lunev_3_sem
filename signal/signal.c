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



void getChildSignalDie()
{
		printf("\n");
		kill(childpid, SIGKILL);
		exit(0);
}

void getAlarmSignal()
{
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
		sigset_t set,set0;
		
		sigemptyset(&set);
		sigaddset(&set,SIGPIPE);
		sigprocmask(SIG_BLOCK,&set,NULL);
		
		sigemptyset(&set0);
		sigaddset(&set0,SIGALRM);
		sigprocmask(SIG_UNBLOCK,&set0,NULL);
		
		struct sigaction act,alrm;
		
		
		memset(&act,0,sizeof(act));
		act.sa_handler=getParentSignal;
		sigaction(SIGPIPE,&act,NULL);
		
		memset(&alrm,0,sizeof(alrm));
		alrm.sa_handler=getAlarmSignal;
		sigaction(SIGALRM,&alrm,NULL);
		
		n=0;
		readresult=fscanf(f,"%c",&data);
		alarm(10);
		while (readresult>0)
		{
			
			connect=0;
			
			sigprocmask(SIG_UNBLOCK,&set,NULL);		
			alarm(10);
			while (!connect) {}
			alarm(10);
					
			sigprocmask(SIG_BLOCK,&set,NULL);
			if (n<7) 
			{
				int a;
				a=(data&(1<<n))>>n;
				if (a==0) 
				{
					if (kill(parentpid, SIGUSR1)<0) exit(-1);
				} else 
				{
					if (kill(parentpid, SIGUSR2)<0) exit(-1);
				}
//				printf("Send: %d\n",a);
				fflush(stdout);
				n++;
			} else
			{
				int a;
				a=(data&(1<<n))>>n;
				if (a==0) 
				{
					if (kill(parentpid, SIGUSR1)<0) exit(-1);
				}	 else 
				{
					if (kill(parentpid, SIGUSR2)<0) exit(-1);
				}
//				printf("Send: %d\n",a);
				fflush(stdout);
				readresult=fscanf(f,"%c",&data);
				n=0;
				connect=1;
			}
			/*
			if (data=='0') kill(parentpid, SIGUSR1); else
			if (data=='1') kill(parentpid, SIGUSR2); else
			{
				exit(0);			
			}
			* */
		}

		exit(0);
} else 
	
/* Code executed by parent */	
{                    
		connect=0;
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
		int n=0;
		char outdata=0;
		while (1)
		{
			sigprocmask(SIG_UNBLOCK,&set0,NULL);
			sigprocmask(SIG_UNBLOCK,&set1,NULL);	
			while (!connect) {};
			connect=0;
			sigprocmask(SIG_BLOCK,&set0,NULL);
			sigprocmask(SIG_BLOCK,&set1,NULL);
			
			if (n<7)
			{
//				printf("Get: %d\n",data);
				fflush(stdout);
				outdata=outdata+(data<<n);
				n++;
			} else
			{
//				printf("Get_: %d\n",data);
				fflush(stdout);
				fflush(stdout);
				outdata=outdata+(data<<n);
				n=0;
				printf("%c",outdata);
				fflush(stdout);
				outdata=0;
				
				//connect=1;
			}
			/*
			printf("%d",data);
			fflush(stdout);
			*/
			kill(childpid, SIGPIPE);
	
		}
		
wait(NULL);
	
}

return 0;
}
