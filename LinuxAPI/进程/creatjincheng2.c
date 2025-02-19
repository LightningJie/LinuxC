#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	pid_t re=-2;
	re =fork();
	if (re>0)
	{
		printf("in father print fork return = %d\n",re);
		printf("this is father,father pid=%d \n",getpid());
	}
	else {
		printf("in child print fork return = %d \n",re);
		printf("this is child print,child pid =%d \n",getpid());
	}
	exit(0);
}
