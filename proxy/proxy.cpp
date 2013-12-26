#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <iostream>

#define BUFF_SIZE_LIMIT 128000
#define SIZE 1024
using namespace std;

struct Child
{
        unsigned char *buffer;
        int size;
        int count;
        int end;
        int childfd[2];
        int parentfd[2];
        
        void setsize(int n)
        {
                this->count = 0;
                this->end = 0;
                this->size = 300*n;
                if (this->size > BUFF_SIZE_LIMIT)
                        this->size = BUFF_SIZE_LIMIT;
                this->buffer = new unsigned char[this->size];
        }
        
        void execute(void)
        {
                char *buffer = new char[size];
                int rd_res, wr_res;
                while (1)
                {
                        rd_res= read(this->childfd[0], buffer, SIZE);
                        if (rd_res == 0)
                        {
                                close(this->childfd[0]);
                                close(this->childfd[1]);

                                break;
                        }

                        while (rd_res > 0)
                        {
                                wr_res = write(this->childfd[1], buffer, rd_res);

                                for (int i= 0; i < (rd_res-wr_res); i++)
                                {
                                        buffer[i]= buffer[i+wr_res];
                                }
                                rd_res = rd_res - wr_res;
                        }
                }
                return;
        }
        
};

int main(int argc, char *argv[])
{
        Child *child;
        int child_count;
        int pid;
        int index;
        int result;
        fd_set st_read, st_write;
        fd_set st_read_old, st_write_old;
        int max_fd = 0;
        int fd[2];
        

        
        /** Load input params*/
        
        sscanf(argv[2], "%u", &child_count);
        
        
        if ((argc != 3)||(child_count == 0))
        {
                cerr<<"Usage: "<<argv[0]<<" <InputFileName> <ChildsCount> "<<endl;
                fflush(stderr);
                exit(EXIT_FAILURE);
        }
        
        /** Format startig data*/
        
        child = new Child [child_count];
        
        for (int i = 0; i < child_count; i++)
        {
                child[i].setsize(child_count - i + 1);
                if (i != 0)
                {
                        pipe(fd);
                        child[i].childfd[0] = fd[0];
                        child[i-1].parentfd[1] = fd[1];
                        if (fd[1] > max_fd)
                        {
                                max_fd= fd[1];
                        }
                        if (fd[0] > max_fd)
                        {
                                max_fd= fd[0];
                        }
                        fcntl(child[i-1].parentfd[1], F_SETFL, O_NONBLOCK);
                }
                pipe(fd);
                child[i].childfd[1]=fd[1];
                child[i].parentfd[0]=fd[0];
                if (fd[0] > max_fd)
                {
                        max_fd= fd[0];
                }
                if (fd[1] > max_fd)
                {
                        max_fd= fd[1];
                }
                fcntl(child[i].parentfd[0], F_SETFL, O_NONBLOCK);
        
        
                /** Fork*/
                
                pid = fork();        
        
                if (pid == 0)

                /* CHILD */

                {
                        for (int j= 0; j < i; j++)
                        {
                                close(child[j].parentfd[0]);
                                close(child[j].parentfd[1]);
                        }
                        close(child[i].parentfd[0]);
                        
                        if (i==0)
                        {
                                child[i].childfd[0] = open(argv[1], O_RDONLY);
                        }
                        child[i].execute();
                        
                        exit(EXIT_SUCCESS);
                }
                
                close(child[i].childfd[0]);
                close(child[i].childfd[1]);
        }

        child[child_count-1].parentfd[1] = 1;
        max_fd++;
        
        FD_ZERO(&st_read);
        FD_ZERO(&st_write);
        FD_ZERO(&st_read_old);
        FD_ZERO(&st_write_old);
        
        for (int i= 0; i < child_count; i++)
        {
                FD_SET(child[i].parentfd[0], &st_read);
        }
        while (1)  
        {
			st_write_old = st_write;
            st_read_old = st_read;
            
			select(max_fd, &st_read_old, &st_write_old, NULL, NULL);
			
				
				for (index = 0; index <= child_count -1; index++)
				{
					
					/** read */
					if (FD_ISSET(child[index].parentfd[0], &st_read_old))
					{
							result = read(child[index].parentfd[0], child[index].buffer+child[index].count, child[index].size-child[index].count);
							
							if (result == 0)
							{
									child[index].end = 1;
									FD_CLR(child[index].parentfd[0], &st_read);
									if (child[index].count == 0)
									{                 
											close(child[index].parentfd[0]);
											if (index != child_count-1)
											{
													close(child[index].parentfd [1]);
											}
											else
											{
													 break;
											}
									}
							}
							else
							{
									if ((child[index].count += result) == child[index].size)
									{
											FD_CLR(child[index].parentfd[0], &st_read);
									}
							}
							if (child[index].count != 0)
							{
									FD_SET(child[index].parentfd[1], &st_write);
							}
					} 

                
					/** write */

                
					if (FD_ISSET(child[index].parentfd[1], &st_write_old))
					{
							result = write(child[index].parentfd[1], child[index].buffer, child[index].count);
							
							if (result < child[index].count)
							{
									for (int j= 0; j < (child[index].count-result); j++)
									{
											child[index].buffer[j]= child[index].buffer[j+result];
									}
									child[index].count-= result;
									
							}
							else
							{
									child[index].count = 0;
									FD_CLR(child[index].parentfd[1], &st_write);
									
									if (child[index].end == 1)
									{                 
											close(child[index].parentfd[0]);
											if (index != child_count-1)
											{
													close(child[index].parentfd[1]);
											}
											else
											{
													break;
											}
									}
									else
									{
											FD_SET(child[index].parentfd[0], &st_read);
									}
							}
					} 
				}
			

            if (child[child_count - 1].end == 1) break;
        }
        
        for (int i= 0; i < child_count; i++)
        {
                wait(NULL);
        }
        
        return 0;
}
