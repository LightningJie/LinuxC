#include<stdio.h>
#include<unistd.h>
#include<string.h>
int main()
{
	int fd[2];
	int pid;
	char buf[128];
	if (pipe(fd)==-1)
	{
		printf("create pipe failed\n");
	}
	pid=fork();
	if(pid==-1)
	{
		printf("child create error\n");
	}
	if(pid >0)
	{
		printf("this is father\n");
		close(fd[0]);
		write(fd[1],"hello from father",strlen("hello from father"));
	}
	else if(pid ==0)
	{
		printf("this is child \n");
		close(fd[1]);
		read(fd[0],buf,128);
		printf("read from father: %s \n",buf);
	
	}
	
	return 0;
}
