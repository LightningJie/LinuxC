#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	pid_t re=-2;
	int data;
	while(1){
		printf("please input a data\n");
		scanf("%d",&data);
		if (data==1)
		{
		
			re =fork();
			if (re>0)
			{
				//printf("this is father,father pid=%d \n",getpid());
					
			}

			else {
				while(1){
					printf("do net request,pid=%d\n",getpid());
					sleep(3);
				}
			}
		}
		else{
			printf("wait,do nothing\n");
		}
	}
	exit(0);
}
