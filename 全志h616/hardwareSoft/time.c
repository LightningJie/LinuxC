#include <stdio.h>
#include <sys/time.h>
int main()
{
	struct timeval tv;
	gettimeofday(&tv,NULL);
	printf("sec: %ld,usec=%ld\n",tv.tv_sec,tv.tv_usec);
}
