#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



int threads[2];
int turn;
int sch = 0;

void  critical_function(int thread_id)
{
	++sch;
	printf("Critical func hello =) %d\n",thread_id);
}
void mfence_c()
{
//#define mbarrier() asm volatile ("":::"memory")
	asm volatile("mfence":::"memory");
}
void *dekker(int thread_id)
{
    threads[thread_id] = 1; 
	mfence_c();
    while (threads[1-thread_id])
    {
        // Сюда мы попадаем, когда еще кто-то есть. 
        if (turn != thread_id)
        {
            // Очередь не наша, подождем...
            threads[thread_id] = 0;
            while (turn != thread_id) {}
            threads[thread_id] = 1;
        }
    }
    critical_function();
    // Мы попользовались ресурсом, спасибо. Дадим и другим.
	
    turn = 1-thread_id;
	mfence_c(thread_id);
    threads[thread_id] = 0;
}

int main()
 {
	pthread_t thread1;
	pthread_t thread2;
	int thread_args[] = {0, 1};
	 
	int status=pthread_create(&thread1,NULL,&dekker,0);
	if (status)
	{
		printf("Can't create 1st thread x_X \n");
		return 1;
	}
	
	status=pthread_create(&thread2,NULL,&dekker,1);
	if (status)
	{
		printf("Can't create 2st thread \\*o*\\\n");
		return 1;
	}

	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);
	

	return 0;
}
