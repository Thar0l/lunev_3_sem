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


#define WRITER 1
#define READER 0
#define SYNC_FIFO "sync.fifo"
#define BUFF_SIZE 32

char *endname=".fifo";

int  main(int argc, char *argv[])
{

  
char prog_mode;
int fifoid,syncfifoid,fileid;
char *bufer;
char filename[BUFF_SIZE];

/* --------*/
/* GLOBAL  */  
/* --------*/

  if (argc != 3) 
  {
      fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
      exit(EXIT_FAILURE);
  }
  
  if (strcmp(argv[2],"-r")==0) 
  {
      prog_mode=READER;
  } else
    
  if (strcmp(argv[2],"-w")==0) 
  {
      prog_mode=WRITER;
  } else
    
  {
		fprintf(stderr, "Usage: %s <filename> <mode>\n", argv[0]);
		exit(EXIT_FAILURE);
  }
  
/* --------*/
/* READER  */  
/* --------*/

  if (prog_mode==READER)
  {
  int res;
    if (( syncfifoid=open(SYNC_FIFO, O_RDONLY))<0)
    { 
		perror("Opening sync fifo"); 
		exit(EXIT_FAILURE); 
    }
    
    if ((fileid = open(argv[1],O_WRONLY|O_CREAT,0664))<1)
    {
		perror("Opening file");
		exit(EXIT_FAILURE); 
    }
        
    
    while (res=read(syncfifoid,&filename,BUFF_SIZE)<BUFF_SIZE) 
    {
	if (res<1)
	{
	    perror("Reading fifo");
	    exit(EXIT_FAILURE);
	}
    }
    close(syncfifoid);
    strcat(filename,endname);
    if (( fifoid=open(filename, O_RDONLY))<0)
    { 
		perror("Opening fifo"); 
		exit(EXIT_FAILURE); 
    }

    while (read(fifoid, &bufer, 1)>0) write(fileid, &bufer, 1); 
    
    close(fifoid);
    unlink(filename);
    close(fileid);
    exit(EXIT_SUCCESS);
  }
  
/* --------*/
/* WRITER  */  
/* --------*/  
  
  if (prog_mode==WRITER)
  { 
    syncfifoid=mkfifo(SYNC_FIFO, 0664);
    if((syncfifoid == -1)&&(errno!=EEXIST))
    {
		fprintf(stderr, "Can`t create sync fifo\n");
		exit(EXIT_FAILURE);
    }
     
    
    if((syncfifoid=open(SYNC_FIFO, O_WRONLY))<0)
    {
		perror("Opening sync fifo"); 
		exit(EXIT_FAILURE); 
    }
    
    int pid = getpid();
    int n1=sprintf(filename, "%d", pid);
    
    struct sysinfo info;
    sysinfo(&info);
    char *filename0;
   
    int n2=sprintf(filename0, "%ll", info.uptime);
    n1=n1+n2;
   
	
	strcat(filename,filename0);
    for (;n1<BUFF_SIZE;n1++) filename[n1]='z';
    strcat(filename,endname);
       
    fifoid=mkfifo(filename, 0664);
    if((fifoid == -1)&&(errno!=EEXIST))
    {
		fprintf(stderr, "Can`t create fifo\n");
		exit(EXIT_FAILURE);
    }
    close(fifoid); 
    
    write(syncfifoid, &filename,BUFF_SIZE);
    
    if((fifoid=open(filename, O_WRONLY))<0)
    {
		perror("Opening fifo"); 
		exit(EXIT_FAILURE); 
    }
  
    if ((fileid = open(argv[1],O_RDONLY))<1)
    {
		perror("Opening file");
		exit(EXIT_FAILURE); 
    }
    
    while (read(fileid,&bufer,1)>0) write(fifoid, &bufer,1);
    
    close(syncfifoid);
    close(fifoid);
    unlink(filename);
    close(fileid);
    exit(EXIT_SUCCESS);
  }

  

}
