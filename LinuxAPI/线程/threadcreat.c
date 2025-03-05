#include<stdio.h>
#include <pthread.h>
void *func1(void *arg)
{
	static int ret=10;
	printf("t1: %ld thread is creat\n",(unsigned long)pthread_self());
	printf("param is %d\n",*((int *)arg));
	pthread_exit((void *)&ret);
}
int main()
{
	int ret;
	int param=100;
	pthread_t t1;

	int *pret;
	ret=pthread_create(&t1,NULL,func1,(void *)&param);
	if (ret ==0)
	{
		printf("create t1 success \n");
	}
	printf("main:%ld thread is creat\n",(unsigned long)pthread_self());
	pthread_join(t1,(void **)&pret);
	printf("t1 quit : %d \n",*pret);
}
