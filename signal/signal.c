#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <math.h>


//Global

int         connection;
char         bit;
int         childpid;
int         parentpid;

void get_signal_die();
void get_signal_alarm();
void get_signal_sync();
void get_signal_0();
void get_signal_1();

int tmp;


int main (int argc, char** argv)
{
sigset_t sigset_all;
sigemptyset (&sigset_all);
sigaddset (&sigset_all,  SIGPIPE);
sigaddset (&sigset_all,  SIGALRM);
sigaddset (&sigset_all,  SIGUSR1);
sigaddset (&sigset_all,  SIGUSR2);
sigaddset (&sigset_all,  SIGCHLD);
sigprocmask (SIG_BLOCK,   &sigset_all,  NULL);
	
parentpid = getpid();
childpid = fork();


if (childpid == -1) 
{        
                perror ("fork");
                exit (EXIT_FAILURE);
}
        
		sigset_t sigset_e;
		sigemptyset(&sigset_e);
		
		
        if (childpid == 0) 
        {           
                        int         read_result;
                        int         index;
                        FILE         *file; 
                        
                        connection=0;
                        
                        file = fopen (argv[1], "rb");
                        if (!file)
                        {
                                perror ("Opening file: ");
                                return -1;
                        }
                        sigset_t sigset_pipe, sigset_alarm;
                        
                        sigemptyset (&sigset_pipe);
                        sigemptyset (&sigset_alarm);
                        
                        sigaddset (&sigset_pipe,  SIGPIPE);
                        sigaddset (&sigset_alarm, SIGALRM);
                        
                        sigprocmask (SIG_BLOCK,   &sigset_pipe,  NULL);                
                        sigprocmask (SIG_UNBLOCK, &sigset_alarm, NULL);
                        
                        struct sigaction sigact_pipe;
                        struct sigaction sigact_alarm;
                        
                        
                        memset (&sigact_pipe,  0, sizeof(sigact_pipe));
                        memset (&sigact_alarm, 0, sizeof(sigact_alarm));
                        
                        sigact_alarm . sa_handler = get_signal_alarm;
                        sigact_pipe  . sa_handler = get_signal_sync;
                        
                        sigaction (SIGPIPE, &sigact_pipe,  NULL);
                        sigaction (SIGALRM, &sigact_alarm, NULL);
                        
                        index = 0;
                        
                        read_result = fscanf (file, "%c", &bit);
                        
                        alarm(10);
                        
                        while (read_result>0)
                        {
                                
                                connection=0;
                                alarm(10);

                                              
//									sleep(1);

									sigsuspend(&sigset_e);

                                
                                alarm(10);
                                connection =0;                
                                
                                
                                if (index < 7) 
                                {
                                        int a;
                                        
                                        a = (bit & (1 << index)) >> index;
                       //                  printf ("%d", a);
                                        if (a == 0) 
                                        {
                                                if (kill (parentpid, SIGUSR1)<0) exit(-1);
                                        } 
                                        else 
                                        {
                                                if (kill (parentpid, SIGUSR2)<0) exit(-1);
                                        }

                                        index++;
                                        
                                }
                                else
                                {
                                        int a;
                                        
                                        a = (bit & (1 << index)) >> index;
                     //                   printf ("%d", a);
                                        if (a == 0) 
                                        {
                                                if (kill (parentpid, SIGUSR1)<0) exit(-1);
                                        }         
                                        else 
                                        {
                                                if (kill (parentpid, SIGUSR2)<0) exit(-1);
                                        }

                                        read_result = fscanf (file, "%c", &bit);
                           //             printf ("\n file: %d\n send: ", bit);

                                        index = 0;
                                        connection = 1;
                                }
                        }
                        exit(0);
        } 
        else 
        {                    
                        
                        int         n    = 0;
                        char        data = 0;
                        
                        connection = 0;
                        
                        sigset_t sigset_usr, sigset_child;
                        
                        sigemptyset (&sigset_usr);
                        sigemptyset (&sigset_child);
                        
                        sigaddset (&sigset_usr,  SIGUSR1);                
                        sigaddset (&sigset_usr,  SIGUSR2);                
                        sigaddset (&sigset_child, SIGCHLD);
                        
                        struct sigaction sigact_usr1, sigact_usr2, sigact_child;
                        
                        memset (&sigact_usr1,  0, sizeof(sigact_usr1));
                        memset (&sigact_usr2,  0, sizeof(sigact_usr2));
                        memset (&sigact_child, 0, sizeof(sigact_child));
                        
                        sigact_usr1  . sa_handler  = get_signal_0;
                        sigact_usr2  . sa_handler  = get_signal_1;
                        sigact_child . sa_handler  = get_signal_die;
                        
                        sigaction (SIGUSR1, &sigact_usr1,  NULL);                
                        sigaction (SIGUSR2, &sigact_usr2,  NULL);                
                        sigaction (SIGCHLD, &sigact_child, NULL);
                        
                        sigprocmask (SIG_BLOCK, &sigset_usr,  NULL);
                        sigprocmask (SIG_UNBLOCK, &sigset_child, NULL);
                        
                        //sleep(1);
                        
                        kill(childpid, SIGPIPE);
                        

                        
                        while (1)
                        {
//							sleep(1);
    
                                

									//sleep(1);
                                //sigwait(&sigset_usr,&tmp);
									sigsuspend(&sigset_e);

                            

                                
                                connection = 0;
                                data = data + (bit << n);
                      //          printf ("%d", bit);
                                if (n < 7)
                                {
                                        n++;
                                } 
                                else
                                {
                                        //write(0,&data,sizeof(data));
                         //               printf ("\n out: %d \n get:  ", data);
										printf("%c",data);
                                       // fflush (stdout);
                                        
                                        data = 0;
                                        n = 0;
                                }
                                kill(childpid, SIGPIPE);
                        }
        wait(NULL);
        }

return 0;
}


void get_signal_die()
{
        kill(childpid, SIGKILL);
        exit(0);
}


void get_signal_alarm()
{
        exit(0);
}


void get_signal_sync()
{
        //connection = 1;
}


void get_signal_0()
{
        //connection = 1;
        bit = 0;
}


void get_signal_1()
{
        //connection = 1;
        bit = 1;
}
