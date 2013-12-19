#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>

#define BUFF_SIZE 1024

#define SEM_READER 0
#define SEM_WRITER 1
#define SEM_EMPTY  2
#define SEM_FULL   3
#define SEM_SYNC   4

union semun{
	int val;
	unsigned short *array;
};



void addtobuff(struct sembuf *buff, unsigned short semnum, short op, short flag)
{
	buff->sem_num = semnum;
	buff->sem_op  = op;
	buff->sem_flg = flag;
}


int main(int argc, char* argv[])
{
		/********/
		/*GLOBAL*/
		/********/
	
	int semid;
	int shmid;
	
	struct sembuf buff[8];
	void *shm_mem;
	
	union semun semunion;
	
	unsigned short sems[5]= {0, 0, 0, 0, 0};
	
	int i= 3;
	int count = 1;
	int fileid;
	
	semid = semget(ftok(argv[0], 1), 5, IPC_CREAT | 0664);
	shmid = shmget(ftok(argv[0], 2), BUFF_SIZE+4, IPC_CREAT | 0664);
	
	if (argc == 1)
	{
		/********/
		/*READER*/
		/********/


		addtobuff(&buff[0], SEM_READER, 0, IPC_NOWAIT);
		addtobuff(&buff[1], SEM_SYNC,   0, IPC_NOWAIT);
		addtobuff(&buff[2], SEM_READER, +1, SEM_UNDO);
		semop(semid, buff, 3);
		if (errno == EAGAIN)
		{
			fprintf(stderr, "\n Reader already exists\n");
			exit(EXIT_FAILURE);
		}
		
		addtobuff(&buff[0], SEM_WRITER, -1, 0);
		addtobuff(&buff[1], SEM_WRITER, +1, 0);
		addtobuff(&buff[2], SEM_SYNC,   +1, SEM_UNDO);
		addtobuff(&buff[3], SEM_FULL,   +1, SEM_UNDO);
		semop(semid, buff, 4);


		addtobuff(&buff[0], SEM_WRITER, -1, IPC_NOWAIT);
		addtobuff(&buff[1], SEM_WRITER, +1, 0);
		addtobuff(&buff[2], SEM_SYNC,   -2, 0);
		addtobuff(&buff[3], SEM_SYNC,   +2, 0);
		semop(semid, buff, 4);
		if (errno == EAGAIN)
		{
			fprintf(stderr, "\n Sync error\n");
			exit(EXIT_FAILURE);
		}

		shm_mem= shmat(shmid, NULL, 0);
		
		while (1)
		{
			addtobuff(&buff[0], SEM_WRITER, -1, IPC_NOWAIT);
			addtobuff(&buff[1], SEM_WRITER, +1, 0);
			addtobuff(&buff[2], SEM_EMPTY,   0, 0);
			addtobuff(&buff[3], SEM_EMPTY,  +1, 0);
			semop(semid, buff, 4);
			
			if (semctl(semid, SEM_SYNC, GETVAL) == 1)
			{
				fprintf(stderr, "\n No writer\n");
				semunion.array = sems;
				semctl(semid, SEM_READER, SETALL, semunion);
				exit(EXIT_FAILURE);
			}

			count = *((int*)shm_mem);
			if (count == 0) break;
			count = write(1, shm_mem+4, count);


			addtobuff(&buff[0], SEM_FULL, -1, 0);
			semop(semid, buff, 1);
		}
		
		addtobuff(&buff[0], SEM_FULL, -1, 0);
		semop(semid, buff, 1);
		
		addtobuff(&buff[0], SEM_SYNC, 0, 0);
		semop(semid, buff, 1);
	}

	else if (argc == 2)
	{
		/********/
		/*WRITER*/
		/********/
		
		addtobuff(&buff[0], SEM_WRITER,  0, IPC_NOWAIT);
		addtobuff(&buff[1], SEM_SYNC,    0, IPC_NOWAIT);
		addtobuff(&buff[2], SEM_WRITER, +1, SEM_UNDO);
		semop(semid, buff, 3);
		
		if (errno == EAGAIN)
		{
			fprintf(stderr, "\n Writer already exists.\n");
			exit(EXIT_FAILURE);
		}
		
		addtobuff(&buff[0], SEM_READER, -1, 0);
		addtobuff(&buff[1], SEM_READER, +1, 0);
		addtobuff(&buff[2], SEM_SYNC,   +1, SEM_UNDO);
		addtobuff(&buff[3], SEM_EMPTY,  +1, SEM_UNDO);
		semop(semid, buff, 4);

		addtobuff(&buff[0], SEM_READER, -1, IPC_NOWAIT);
		addtobuff(&buff[1], SEM_READER, +1, 0);
		addtobuff(&buff[2], SEM_SYNC,   -2, 0);
		addtobuff(&buff[3], SEM_SYNC,   +2, 0);
		semop(semid, buff, 4);
		
		if (errno == EAGAIN)
		{
			fprintf(stderr, "\n Sync error\n");
			exit(EXIT_FAILURE);
		}

		shm_mem= shmat(shmid, NULL, 0);
		fileid= open(argv[1], O_RDONLY);
		
		while (count)
		{

			count= read(fileid, shm_mem+4, BUFF_SIZE);
			*((int*)shm_mem)= count;

			addtobuff(&buff[0], SEM_EMPTY,  -1, 0);
			semop(semid, buff, 1);

			addtobuff(&buff[0], SEM_READER, -1, IPC_NOWAIT);
			addtobuff(&buff[1], SEM_READER, +1, 0);
			addtobuff(&buff[2], SEM_FULL,    0, 0);
			addtobuff(&buff[3], SEM_FULL,   +1, 0);
			semop(semid, buff, 4);
			
			if (semctl(semid, 4, GETVAL) == 1)
			{
				fprintf(stderr, "\n No reader\n");
				semunion.array= sems;
				semctl(semid, 0, SETALL, semunion);
				exit(EXIT_FAILURE);
			}
		}

		semunion.array= sems;
		semctl(semid, 0, SETALL, semunion);
	}

	shmdt(shm_mem);

	return 0;
}

