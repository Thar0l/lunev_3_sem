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



void P (int semid)
{
	struct sembuf mybuf;
	mybuf . sem_op 	= -1;
	mybuf . sem_flg = SEM_UNDO;
	mybuf . sem_num = 0;
	semop (semid, &mybuf, 1);
}

void V (int semid)
{
	struct sembuf mybuf;
	mybuf . sem_op	 = 1;
	mybuf . sem_flg	 = SEM_UNDO;
	mybuf . sem_num  = 0;
	semop (semid, &mybuf, 1);
}

int sem_get (char file[], int offset)
{
	int semid = semget (ftok (file, 0) + offset, 1, IPC_CREAT | 0664);
	if (semid < 0)
	{
		perror ("semget: ");
		exit (EXIT_FAILURE);
	}
	return semid;
}

int mem_get (char file[], int offset)
{
	int shmid;
	if (shmid = shmget(ftok(file, 0) + offset, sizeof(char) * BUFF_SIZE, 0664 | IPC_CREAT | IPC_EXCL) < 0)
    {
		if (errno == EEXIST)
		{ 
			shmid = shmget (ftok (file, 0) + 3, sizeof(char) * BUFF_SIZE, 0);
		}
		else
		{
			perror ("shmget (exist): ");
			exit (EXIT_FAILURE);
		}
		
	}
	else
	{
		perror ("shmget (create): ");
		exit (EXIT_FAILURE);
	}
	return shmid;
}

int  main(int argc, char *argv[])
{

if (argc > 2) 
{
	fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
	exit(EXIT_FAILURE);
}
  
char prog_mode;
int fileid,memid;
char bufer;
int sem_mutex, sem_full, sem_empty;
int reader,writer,rexist,wexist;

sem_mutex 	=	sem_get (argv[0], 1);
sem_full	=	sem_get (argv[0], 2);
sem_empty	=	sem_get (argv[0], 3);
reader=sem_get(argv[0],4);
writer=sem_get(argv[0],5);
wexist=sem_get(argv[0],6);
rexist=sem_get(argv[0],7);








  
  if (argc==1) 
  {
      prog_mode=READER;
  } else
    
  {
      prog_mode=WRITER;
  } 
  
  
  
  memid=mem_get(argv[0],8);


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
		P(sem_full);
		if (semctl(wexist,0,GETVAL)<=0) exit(0);
		P(sem_mutex);
		bufer=mem[0];
		V(sem_mutex);
		V(sem_empty); 
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
	
	semctl(sem_mutex,0,SETVAL,1);
	semctl(sem_empty,0,SETVAL,BUFF_SIZE);
	semctl(sem_full,0,SETVAL,0);
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
		P(sem_empty);
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
		P(sem_mutex);
		mem[0]=bufer;
		V(sem_mutex);
		V(sem_full); 
		if (semctl(rexist,0,GETVAL)<=0) exit(0);
		P(reader);
	}
	}
	return 0;
  }

  

}

