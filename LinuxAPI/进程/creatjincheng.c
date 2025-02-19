#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int pid ,pid2;
	pid =getpid();
	
	printf("before fork:pid = %d\n",pid);

	fork();
	
	pid2=getpid();
	printf("after fork:pid= %d \n",pid2);
	if (pid==getpid())
	{
		printf("this is father print\n");
	}
	else{
		printf("this is child print,chile pid =%d \n",getpid());
	}
	printf("pid= %d  current_pid= %d\n",pid,getpid());
	
	exit(0);
}
