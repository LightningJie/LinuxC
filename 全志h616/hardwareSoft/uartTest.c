#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "wiringSerial.h"
#include <pthread.h>
#include "uartTool.h"
int fd;
void *sendSerial(void *arg)
{
	char *sendBuf;
	sendBuf=(char *)malloc(32*sizeof(char));
	while(1){
		memset(sendBuf,'\0',32);
		scanf("%s",sendBuf);
		serialSendstring(fd,sendBuf);
	}
}

void *readSerial(void *arg)
{
	char buffer[32];
	while(1){
		memset(buffer,'\0',sizeof (buffer));
		serialGetstring(fd,buffer);
		printf("GET:%s\n",buffer);
	}

}

int main(int argc,char **argv)
{
	char deviceName[32]={'\0'};
	pthread_t readt;
	pthread_t sendt;
	if(argc<2)
	{
		printf("uage:%s  /dev/ttyS?\n",argv[0]);
		return -1;
	}
	strcpy(deviceName,argv[1]);
	if((fd=myserialOpen(deviceName,115200))==-1)
	{
		printf("open %s error \n",deviceName);
		return -1;
	}
	pthread_create(&readt,NULL,readSerial,NULL);
	pthread_create(&sendt,NULL,sendSerial,NULL);

	while(1){
		sleep(10);
	}
	return 0;

}
