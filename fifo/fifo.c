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
#define NAME_LENGTH 16
#define SYNC_FIFO "sync.fifo"
#define BUFF_SIZE 1024

char endname[] = ".fifo";



int  main(int argc, char *argv[])
{

  
char prog_mode;
int fifoid, syncfifoid,fileid;
int result;
char buffer[BUFF_SIZE];
char filename[NAME_LENGTH];

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
	  
	if ((mkfifo(SYNC_FIFO, 0644 ) < 0 ) && ( errno != EEXIST ) )
	{
					
		perror("Creating sync fifo: ");
		exit(EXIT_FAILURE); 
	}
	
    if (( syncfifoid=open(SYNC_FIFO, O_RDONLY)) < 0)
    { 
		perror("Opening sync fifo: "); 
		exit(EXIT_FAILURE); 
    }
    
    if ((fileid = open(argv[1],O_WRONLY|O_CREAT,0664)) < 1)
    {
		perror("Opening file: ");
		exit(EXIT_FAILURE); 
    }
    result = 0;
    while ( result < NAME_LENGTH) 
	{
		result = read ( syncfifoid, &filename, NAME_LENGTH);
		if ( result < 0 )
		{
			perror("Reading from sync fifo: ");
			exit(EXIT_FAILURE); 
		}
	} 
	close (syncfifoid);
	
	if ((fifoid = open( filename , O_WRONLY|O_NDELAY ) )> 0)
	{
			fprintf(stderr, "Opening fifo: alredy opened to write.\n");
			exit(EXIT_FAILURE); 
	}
	
	
	
	if ((fifoid = open( filename , O_RDONLY|O_NDELAY  ) )< 0)
	{
			perror("Opening fifo: ");
			exit(EXIT_FAILURE); 
	}
	
	sleep (3);
	
	result = 1;
	while (result  != 0)
	{
		result = read (fifoid, &buffer, BUFF_SIZE);
		if (result > 0) write (fileid, &buffer, result);
	}
	
	close(fileid);
	close(fifoid);
	unlink(filename);
	exit(EXIT_SUCCESS);
	}
  
/* --------*/
/* WRITER  */  
/* --------*/  
  
  if (prog_mode==WRITER)
  { 
	if ((mkfifo(SYNC_FIFO, 0644 ) < 0 ) && ( errno != EEXIST ) )
	{
					
		perror("Creating sync fifo: ");
		exit(EXIT_FAILURE); 
	}
	
    if (( syncfifoid=open(SYNC_FIFO, O_WRONLY)) < 0)
    { 
		perror("Opening sync fifo: "); 
		exit(EXIT_FAILURE); 
    }
    
    int pid = getpid();
    
    int n = sprintf(filename, "%d", pid);
    
    while (n < NAME_LENGTH-5) 
    {
		filename[n]='a';
		n++;
    }
	strcat(filename,endname);
    
    if ( ( mkfifo ( filename, 0644 ) < 0 ) && ( errno != EEXIST ) )
	{
					
		perror("Creating fifo: ");
		exit(EXIT_FAILURE); 
	}
	
	
	sleep(1);
	
	if ((fileid = open(argv[1],O_RDONLY,0664)) < 1)
    {
		perror("Opening file: ");
		exit(EXIT_FAILURE); 
    }
    
    if (write(syncfifoid, &filename, NAME_LENGTH) < NAME_LENGTH)
    {
		perror("Writing to sync fifo: ");
		exit(EXIT_FAILURE); 
	}
	
	close(syncfifoid);
	
	sleep (1);	
	
	if ((fifoid = open( filename, O_WRONLY  )) < 0)
	{
			perror("Opening fifo: ");
			exit(EXIT_FAILURE); 
	}
	
	
	
	
	int flags = fcntl(fifoid, F_GETFL, 0);
	flags &= ~O_NONBLOCK;
	fcntl(fifoid, F_SETFL, flags);
	
	result = 1;
	
	while (result)
	{
		result = read (fileid, &buffer, BUFF_SIZE);
		result = write(fifoid, &buffer, result);
	}
	
	close(fileid);
	while (( write ( fifoid, '\0', 1)) && ( errno != EBADF ) )
	close(fifoid);
	unlink(filename);
	exit(EXIT_SUCCESS);
	
  }

  
	exit(EXIT_SUCCESS);
}
