#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

int piparr[2];
int gi=0;

void function(int i, siginfo_t * info,void * vinfo)
{
	gi=i;
	int x=getpid();
	if(write(piparr[1],x,sizeof(int))==-1)
	{
		puts("Error i");
	}
	x=getpgid(x);
	if(write(piparr[1],x,sizeof(int))==-1)
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
	int pid=0,er,gpid=0;
	
	while(gi!=20)
	{	
		er=read(piparr[0],&pid,sizeof(int));
		if(er==-1 && errno!=EINTR)
			puts("read error");
		else if(er>0)
		{	
			printf ("PID %d\n", pid);
			read(piparr[0],&gpid,sizeof(int));
			printf ("Group pid %d\n", gpid);
		}
	}	
	close(piparr[1]);
	close(piparr[0]);

	return 0;
}