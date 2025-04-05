#include <stdio.h>
#include <sys/time.h>
void cutTest()
{
	int i,j;
	for(i=0;i<100 ;i++)
	{
		for (j=0;j<1000;j++)
		{}
	}
}
int main()
{
	struct timeval timestart;
	struct timeval timestop;
	gettimeofday(&timestart,NULL);
	cutTest();
	gettimeofday(&timestart,NULL);
	long diffTime =10000000*(timestop.tv_sec -timestart.tv_sec)+(timestop.tv_usec-timestart.tv_usec);
	printf("diffTime %ld\n",diffTime);
}
