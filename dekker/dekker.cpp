bool threads[2];
int turn;

void *dekker(int thread_id)
{
    threads[thread_id] = true; 
    while (threads[1-thread_id])
    {
        // ���� �� ��������, ����� ��� ���-�� ����. 
        if (turn != thread_id)
        {
            // ������� �� ����, ��������...
            threads[thread_id] = false;
            while (turn != thread_id) {}
            threads[thread_id] = true;
        }
    }
    critical_function();
    // �� �������������� ��������, �������. ����� � ������.
	
    turn = 1-thread_id;
    threads[thread_id] = false;
}

int main()
 {
	pthread_t thread1;
	pthread_t thread2;
	int thread_args[] = {0, 1};
	
	 NULL, do_it, (void*)&thread_args[i])) {
	 
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
	}

	return 0;
}
