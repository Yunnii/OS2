#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define A 100000000
#define mfence_c() asm volatile ("":::"memory")

volatile int threads[2];
volatile int turn;
volatile int sch = 0;

void  critical_function(int thread_id)
{
    mfence_c();
	++sch;
	//printf("Critical func hello =) %d\n",thread_id);
}

void* dekker(void* id)
{
	int thread_id=*((int*)id);
    
    mfence_c();
    while (sch < A)
    {
        threads[thread_id] = 1;
        while (threads[1-thread_id])
        {
            if (turn != thread_id)
            {
                threads[thread_id] = 0;
                while (turn != thread_id) {}
                threads[thread_id] = 1;
            }
        }
        
        mfence_c();
        if ( sch < A ) 
            critical_function(thread_id);
	    
        turn = 1-thread_id;
        threads[thread_id] = 0;
    }
}

int main()
 {
	pthread_t thread1;
	pthread_t thread2;
	int id[] = {0,1};
	 
	int status=pthread_create(&thread1, NULL, dekker,(void*)&id[0]);
	if (status)
	{
		printf("Can't create 1st thread x_X \n");
		return 1;
	}
	
	status=pthread_create(&thread2, NULL,dekker,(void*)&id[1]);
	if (status)
	{
		printf("Can't create 2st thread \\*o*\\\n");
		return 1;
	}

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	
	printf("%i\n", sch);

	return 0;
}