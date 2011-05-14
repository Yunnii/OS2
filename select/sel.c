#include <stdlib.h>
#include<stdio.h>
#include <sys/select.h>
#include <unistd.h>


int main(int argc,char **argv)
{
	fd_set set;
	FD_ZERO(&set);
	int max=0,t;
	int arg[argc];
	for(int i=1;i<argc;i++)
	{
		if (arg[i]=atoi(argv[i]);
		FD_SET(arg[i], &set);
		if (arg[i]>max)max=arg[i];
	}
	max++;	
	fd_set blackjack=set;
	char buf[11];
	while(1)
	{	
		if(select(max,&set,NULL,NULL,NULL)==-1)
		{
			printf("Error");	
			exit(0);
		}
		
		for (int i=1;i<argc;i++)
		{
			if(FD_ISSET(arg[i],&set))
				do
				{
					t=read(arg[i],buf,10);
					if(t==0)
						break;
					buf[t]='\0';
					printf("%s\n", buf);
				}
				while(t==10);
		}
		set=blackjack;
		
	}
	return 0;
}
