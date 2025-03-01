#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
int main(int argc,char ** argv)
{
	int signum;
	int pid;
	char cmd[128]={0};
	signum=atoi(argv[1]);
	pid = atoi(argv[2]);

	printf("num=%d,pid=%d\n",signum,pid);
	
	sprintf(cmd,"kill -%d %d",signum,pid);
	system(cmd);
	printf("send signal ok\n");
	return 0;
}

