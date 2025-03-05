#include<stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
int g_data=0;
pthread_mutex_t mutex;
pthread_cond_t cond;
void *func1(void *arg)
{
	printf("t1: %ld thread is creat\n",(unsigned long)pthread_self());
	printf("t1: param is %d\n",*((int *)arg));
	while(1)
	{
		pthread_cond_wait(&cond,&mutex);
		if (g_data==3)
		{
			printf("t1 quit because g=3\n");
			printf("t1 print %d \n",g_data);

			g_data=0;
			sleep(1);
		}
	}
}
void *func2(void *arg)
{
	printf("t2: %ld thread is creat\n",(unsigned long)pthread_self());
	printf("t2: param is %d\n",*((int *)arg));
	while(1)
	{

		printf("t2 : %d\n",g_data);
		
		pthread_mutex_lock(&mutex);
		g_data++;
		if (g_data==3)
		{
			pthread_cond_signal(&cond);
		}
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

}
int main()
{
	int ret1,ret2;
	int param1=100;
	int param2=200;
	pthread_t t1;
	pthread_t t2;
	pthread_mutex_init(&mutex,NULL);
	ret1=pthread_create(&t1,NULL,func1,(void *)&param1);
	if (ret1 ==0)
	{
		printf("create t1 success \n");
	}
	ret2=pthread_create(&t2,NULL,func2,(void *)&param2);
	if (ret2 ==0)
	{
		printf("create t2 success \n");
	}
	printf("main:%ld thread is creat\n",(unsigned long)pthread_self());
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	pthread_mutex_destroy(&mutex);
}
