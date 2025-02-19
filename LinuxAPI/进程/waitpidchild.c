#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int main()
{
	int re;
	int cnt=0;
	int status=10;
	re=fork();
		
	if (re>0)
	{
		//wait(&status);
		waitpid(re,&status,WNOHANG);
		while(1)
		{
			printf("this is father print,pid=%d\n",getpid());
			sleep(1);
			printf("cnt=%d\n",cnt);
		}
	}
	else if(re==0) {
	       while(1)
	       {
	 		      
			printf("this is child print,chile pid =%d \n",getpid());
		       	sleep(1);
			cnt++;
			if(cnt==3)
			{
				exit(3);
			}
	       }	       
	}
	
	exit(0);
}
