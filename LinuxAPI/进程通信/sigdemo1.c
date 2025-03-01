#include <signal.h>
#include <stdio.h>
//typedef void (*sighandler_t)(int);
//sighandler_t signal(int signum, sighandler_t handler);
void handler(int signum)
{
	printf("get signum=%d\n",signum);
	printf("never quit\n");
}

int main()
{
	signal(SIGINT,handler);	
	while(1);
	return 0;
}

