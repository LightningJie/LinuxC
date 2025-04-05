#include <stdio.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <stdlib.h>
#include <unistd.h>
#define Trig 0
#define Echo 1

double getDistance()
{
	double dis;
	struct timeval start;
	struct timeval stop;
	pinMode (Trig,OUTPUT);
	pinMode (Echo,INPUT);
	digitalWrite(Trig,LOW);
	usleep(5);
	digitalWrite(Trig,HIGH);
	usleep(10);
	digitalWrite(Trig,LOW);

	while(!digitalRead(Echo));
	gettimeofday(&start,NULL);
	while(digitalRead(Echo));
	gettimeofday(&stop,NULL);

	long diffTime =1000000*(stop.tv_sec-start.tv_sec)+(stop.tv_usec-start.tv_usec);
	dis =(double)diffTime/1000000*34000/2;
	return dis;

	
}

int main()
{
	double dis;
	
	if(wiringPiSetup()==-1)
	{
		fprintf(stderr,"%s","initWringPi error");
		exit(-1);	
	}
	printf("开始测量\n");
	while(1){
		dis =getDistance();
		printf("dis：%lf\n",dis);
		usleep(1000000);
	}
	return 0;
	
}
