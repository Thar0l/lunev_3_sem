#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <errno.h>
#include <sys/sysinfo.h>       
#include <sys/ipc.h>
#include <sys/shm.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>



#define WRITER 1
#define READER 0
#define BUFF_SIZE 1


void P(int semid)
{
	struct sembuf mybuf;
	mybuf.sem_op = -1;
	mybuf.sem_flg = SEM_UNDO;
	mybuf.sem_num = 0;
	semop(semid, &mybuf, 1);
}

void V(int semid)
{
	struct sembuf mybuf;
	mybuf.sem_op = 1;
	mybuf.sem_flg = SEM_UNDO;
	mybuf.sem_num = 0;
	semop(semid, &mybuf, 1);
}



int  main(int argc, char *argv[])
{

  
char prog_mode;
int fileid,memid;
char bufer;
int mutex, full, empty;
int reader,writer,rexist,wexist;

mutex=semget(ftok(argv[0],0), 1, IPC_CREAT|0664);
full=semget(ftok(argv[0],0)+1, 1, IPC_CREAT|0664);
empty=semget(ftok(argv[0],0)+2, 1, IPC_CREAT|0664);

reader=semget(ftok(argv[0],0)+4, 1, IPC_CREAT|0664);
writer=semget(ftok(argv[0],0)+5, 1, IPC_CREAT|0664);

wexist=semget(ftok(argv[0],0)+6, 1, IPC_CREAT|0664);
rexist=semget(ftok(argv[0],0)+7, 1, IPC_CREAT|0664);






  if (argc > 2) 
  {
      fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
      exit(EXIT_FAILURE);
  }
  
  if (argc==1) 
  {
      prog_mode=READER;
  } else
    
  {
      prog_mode=WRITER;
  } 
  
    if (memid=shmget(ftok(argv[0],0)+3,sizeof(char)*BUFF_SIZE,0664|IPC_CREAT|IPC_EXCL)<0)
    {
		if (errno==EEXIST) memid=shmget(ftok(argv[0],0)+3,sizeof(char)*BUFF_SIZE,0);
	}

/* --------*/
/* READER  */  
/* --------*/

  if (prog_mode==READER)
  {
	  
	  char *mem = (char *)shmat(memid, NULL, 0);
	  semctl(wexist,0,SETVAL,0);
	  V(rexist);
	while(1)
	{
		
		V(reader);
		int i;
		for (i=0;i<10;i++)
		{
			if (semctl(wexist,0,GETVAL)<=0) sleep(1); else
			break;
		}
		
		if (semctl(wexist,0,GETVAL)<=0) exit(0);
		P(writer);
		if (semctl(wexist,0,GETVAL)<=0) exit(0);
		P(full);
		if (semctl(wexist,0,GETVAL)<=0) exit(0);
		P(mutex);
		bufer=mem[0];
		V(mutex);
		V(empty); 
		printf("%c",bufer);
		fflush(stdout);
		if (semctl(wexist,0,GETVAL)<=0) exit(0);
		
	}	
	return 0;
  }
  
/* --------*/
/* WRITER  */  
/* --------*/  
  
  if (prog_mode==WRITER)
  { 
	  
	  	int i;
		for (i=0;i<10;i++)
		{
			if (semctl(rexist,0,GETVAL)<=0) sleep(1); else
			break;
		}
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
	int fileid;
	fileid = open("in.txt",O_RDONLY,0664);
	
	semctl(mutex,0,SETVAL,1);
	semctl(empty,0,SETVAL,BUFF_SIZE);
	semctl(full,0,SETVAL,0);
	//char str[10]="0123456789";

	char *mem = (char *)shmat(memid, NULL, 0);
	semctl(wexist,0,SETVAL,0);
	struct sembuf mybuf;
	char str[BUFF_SIZE];
	V(wexist);
	int buff_size;
	while (buff_size=read(fileid,&str,BUFF_SIZE)>0)
	{
	for (i=0; i<buff_size; i++)
	{
		V(writer);
		bufer=str[i];
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
		P(empty);
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
		P(mutex);
		mem[0]=bufer;
		V(mutex);
		V(full); 
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
		P(reader);
	}
	}
	return 0;
  }

  

}

