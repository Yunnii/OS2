#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

int x=1;
int gi=0;
int pid=0;

void function(int i, siginfo_t * info,void * vinfo)
{
	gi=i;
	pid=(int)info->si_pid;	
	if(i==20 || i==5)
		x=0;
}

int main()
{
	struct sigaction sag;
	sag.sa_sigaction = function;
	sag.sa_flags=SA_SIGINFO; 

	for (int num=1;num<=31;num++)
		if(sigaction(num,&sag,0)==-1)
			printf("error: %d\n",num);
	while(x==1)
	{	
		if((gi!=0)&&(gi<=31))		
		{
			printf ("I am signal number %d\nprocess id %d\n", gi,pid);
			gi=0;
		}
	}

	return 0;
}
