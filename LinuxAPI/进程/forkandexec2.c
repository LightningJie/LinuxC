#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
	pid_t re=-2;
	int data=10;
	while(1){
		printf("please input a data\n");
		scanf("%d",&data);
		if (data==1)
		{
		
			re =fork();
			if(re>0)
			{
				wait(NULL);
			}		
			if(re==0) 
			{
				if(execl("./forkandexec","forkandexec",NULL)==-1)
				{
					printf("execl failed!\n");
				        perror("why:");

				}
				exit(0);
			}
		}
		else{
			printf("wait,do nothing\n");
		}
	}
	exit(0);
}
