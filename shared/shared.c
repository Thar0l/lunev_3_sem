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

int semGet(key_t key, int offset)
{
	key_t full_key = key + (key_t)(offset);
	return semget(full_key, 1, IPC_CREAT | 0664);
}

void semSet(int semid, int val)
{
	semctl(semid, 0, SETVAL, val);
}

int shmGet(key_t key, int offset)
{
	int shmid;
	key_t full_key = key + (key_t)(offset);
	if (shmid = shmget(full_key, sizeof(char) * BUFF_SIZE, 0664 | IPC_CREAT | IPC_EXCL) < 0)
    {
		if (errno == EEXIST) 
		{
			if (shmid = shmget(full_key, sizeof(char) * BUFF_SIZE, 0) < 0)
			{
				perror("shmget: ");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			perror("shmget: ");
			exit(EXIT_FAILURE);
		}
	}
	return shmid;
}

char * shmAt(int shmid)
{
	return (char *) shmat(shmid, NULL, 0);
}

int isNull(int semid)
{
	return (semctl(semid, 0, GETVAL) == 0);
}

void ping(int semid)
{
	if (semctl(semid, 0, GETVAL) <= 0) exit(0);
}



int main(int argc, char *argv[])
{

  
char prog_mode;

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
  
int fileid, shmid;
char bufer;
int sem_mutex, sem_full, sem_empty;
int sem_reader,sem_writer,sem_exist_r,sem_exist_w;

key_t key = ftok (argv[0], 0);

sem_mutex	=	semGet (key, 1);
sem_full	=	semGet (key, 2);
sem_empty	=	semGet (key, 3);

sem_reader	=	semGet (key, 4);
sem_writer	=	semGet (key, 5);
sem_exist_w	=	semGet (key, 6);
sem_exist_r	=	semGet (key, 7);
    
shmid	=	shmGet(key, 0);


  if (prog_mode == READER)
  {
        
         char * mem = shmAt(shmid);
         int i;
         
         semctl(sem_exist_r,0,SETVAL,0);
         V(sem_exist_r);
         
		 while(1)
         {
                
                V(sem_reader);
                
                for (i=0;i<10;i++)
                {
                        if (isNull(sem_exist_w)) sleep(1); 
                        else
							break;
                }
                
                ping(sem_exist_w);
                P(sem_writer);
                
                ping(sem_exist_w);
                P(sem_full);
                
                ping(sem_exist_w);
                P(sem_mutex);
                
                bufer=mem[0];
                
                V(sem_mutex);
                V(sem_empty);
                
                printf("%c",bufer);
                fflush(stdout);
                
                ping(sem_exist_w);
                
          }        
        return 0;
  }
  

  
  if (prog_mode == WRITER)
  {
        
        int i;
        int fileid;
        char str[BUFF_SIZE];
        int buff_size;
        
        for (i=0;i<10;i++)
        {
			if (isNull(sem_exist_r)) sleep(1); 
			else
				break;
		}
		
        ping(sem_exist_r);
        
        
        fileid = open("in.txt",O_RDONLY,0664);
        char * mem = shmAt(shmid);
        
        semSet(sem_mutex,1);
        semSet(sem_empty,BUFF_SIZE);
        semSet(sem_full,0);
		semSet(sem_exist_w,0);

        V(sem_exist_w);
        
        while (buff_size=read(fileid,&str,BUFF_SIZE)>0)
        {
			for (i=0; i<buff_size; i++)
			{
					V(sem_writer);
					
					bufer=str[i];
					
					ping(sem_exist_r);
					P(sem_empty);
					
					ping(sem_exist_r);
					P(sem_mutex);
					
					mem[0]=bufer;
					
					V(sem_mutex);
					
					V(sem_full);
					
					ping(sem_exist_r);
					P(sem_reader);
			}
        }
        return 0;
  }

  

}
