#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int coorx[] = {-1,-1,-1,0,1,1,1,0};
const int coory[] = {1,0,-1,-1,-1,0,1,1};

class life
 {

	public:
		int field[10][10];
		int num_state;
		bool flag;

		life() 
		{
			num_state = 0;
			memset(field,0,sizeof(field));
			field[4][4] = 1;
			field[4][5] = 1;
			field[3][5] = 1;
			field[3][3] = 1;
			field[5][5] = 1;			
			flag = true;
		}

		life* next_state() const{
			life* next_life = new life();
			memset(next_life->field,0,sizeof(next_life->field));
			int n ,x,y;						
			for(int i = 0; i< 10; ++i)
			{
				for(int j = 0; j< 10; ++j)
				{
					n = 0;	
					for(int k = 0; k < 8; ++k)
					{
						x = (i + coorx[k]+10) % 10;
						y = (j + coory[k]+10) % 10;
						n += field[x][y];	
					}
					if(n == 3 || (n == 2 && field[i][j]))
						next_life->field[i][j] = 1;
				}
			}
			next_life->num_state  = num_state + 1;
			return next_life;
		}

		void print(int fd) const {
			char str[120];
			sprintf(str,"State: %d\r\n",num_state);
			write(fd,str,strlen(str));
			for(int i=0; i<10; ++i){
				for(int j = 0; j < 10; ++j)
				{
					str[j] = "01"[field[i][j]];
				}
				str[10]=13;
				str[11]=10;
				write(fd,str,12);
			}


		}

		bool fr(){return flag;}

		void lock(){flag = false;}

		void release(){flag = true;}
};


struct node 
{
	life* value;
	volatile node* next;
};

volatile node* cur_life;



int client_handler(void *arg)
{
	int c_sock = (int) arg;	

	char buf[100];

	while(1)
	{
		read(c_sock, buf, 99);

		if(buf[0] == 'q') 
		{
			break;
		}
		volatile node* cl_life;

		cl_life = cur_life;

		cl_life->value->lock();

		cl_life->value->print(c_sock);

		cl_life->value->release();

	}	
	close(c_sock);

	return 0;
}

int process(void* arg)
{

	volatile node *next_life , *prev_life , *tmp;

	while(1){	

		sleep(1);

		prev_life = cur_life;

		while (prev_life->next != NULL)
		{
			tmp = prev_life->next;	
			if(tmp->value->fr())
			{       		
				prev_life->next = tmp->next;
	             		delete tmp->value;
                		delete tmp;
			}
			else prev_life = tmp;
            		
        }

    	next_life = new node();
		next_life->value = cur_life->value->next_state();
		next_life->next = cur_life;
		cur_life = next_life;



    		
	}
	_exit(0);
}



int main()
{
	cur_life = new node();
	cur_life->value = new life();
	cur_life->next = NULL;
	void* child_stack = (void*) ((char*)malloc(65536) + 65536);
	if (clone(process, child_stack, CLONE_VM, NULL) == -1) 
	{
		perror("Clone error");
	        _exit(1);
	}	

	int sock, client_sock, clen;
	struct sockaddr_in serv_addr, client_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(4001);
	bind(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

	while(1)
	{
		clen = sizeof(client_addr);
		listen(sock, 10);
		client_sock = accept(sock, (sockaddr*) &client_addr, (socklen_t*)& clen);
		child_stack = (void*) ((char*)malloc(65536) + 65536);
		if(clone(client_handler, child_stack, CLONE_VM | CLONE_FILES, (void *)client_sock) == -1 ) 
		{
			perror("Clone error");
			_exit(1);
		}
	}


	while(1) 
	{
		int state;
		int pid = waitpid(-1,&state, WNOHANG | __WCLONE);

		printf("Thread %d died\r\n",pid);

	}

	return 0;

}