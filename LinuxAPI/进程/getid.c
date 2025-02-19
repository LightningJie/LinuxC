#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main()
{
	int a,b;
	a= getpid();
	b= getppid();
	printf("pid= %d  ppid= %d\n",a,b);
	while(1);
	exit(0);
}
