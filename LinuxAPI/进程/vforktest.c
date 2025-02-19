#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int re;
	int cnt=0;

	re=vfork();
	
	if (re>0)
	{
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
				exit(0);
			}
	       }	       
	}
	
	exit(0);
}
