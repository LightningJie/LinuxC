#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdlib.h>
int fd;
void *Sendhandler(void *arg)
{
	char *sendBuf;
	sendBuf=(char *)malloc(32*sizeof(char));
	while(1){
		memset(sendBuf,'\0',32);
		scanf("%s",sendBuf);
		while(*sendBuf!='\0')
		{
			serialPutchar (fd, *sendBuf++) ;
			
		}

	}
}

void *Revhandler(void *arg)
{	
	while(1){
		while (serialDataAvail (fd))
		{
			printf ("%c", serialGetchar (fd)) ;
			fflush (stdout) ;
		}
	}

}
int main ()
{
	int count ;
	unsigned int nextTime ;
	
	pthread_t idSend;
	pthread_t idRev;
	if ((fd = serialOpen ("/dev/ttyS5", 115200)) < 0)
	{
		fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
		return 1 ;
	}
	pthread_create(&idSend,NULL,Sendhandler,NULL);
	pthread_create(&idRev,NULL,Revhandler,NULL);

	if (wiringPiSetup () == -1)
	{
		fprintf (stdout, "Unable to start wiringPi: %s\n", strerror (errno)) ;
		return 1 ;
	}
	while(1){
		sleep(10);
	}
	printf ("\n") ;
	return 0 ;
}
