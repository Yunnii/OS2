#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int piparr[2];

void function(int i, siginfo_t * info,void * vinfo)
{
	if(write(piparr[1],&i,sizeof(int))==-1)
	{
		puts("Error i");
	}

	if(write(piparr[1],info,sizeof(siginfo_t))==-1)
	{
		puts("Error info");
	}	
}

int main()
{
	struct sigaction sag;
	if (pipe(piparr) == -1) 
	{
               printf("Error in creating pipe");
               exit(0);
        }
	sag.sa_sigaction = function;
	sag.sa_flags=SA_SIGINFO; 

	for (int num=1;num<=31;num++)
		if(sigaction(num,&sag,0)==-1)
			printf("error: %d\n",num);
	int gi=0,er;
	siginfo_t info;
	while(gi!=20)
	{	
		er=read(piparr[0],&gi,sizeof(int));
		if(er==-1 && errno!=EINTR)
			puts("read error");
		else if(er>0)
		{	
			printf ("I am signal number %d\n", gi);
			read(piparr[0],&info,sizeof(info));
			printf ("process id %d\n", (int)(info.si_pid));
		}
	}	
	close(piparr[1]);
	close(piparr[0]);

	return 0;
}
