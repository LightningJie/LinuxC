#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char **argv)
{
	/*
	printf("total params:%d\n",argc);
	printf("No:1  params:%s\n",argv[0]);
	printf("No:2  params:%s\n",argv[1]);
	printf("No:3  params:%s\n",argv[2]);
	*/
	int fdsrc,fddes;
	char *readbuf;
	if (argc!=3)
	{
		printf("params count error\n");
		exit(-1);
	}
	fdsrc=open(argv[1],O_RDWR);
	int size=lseek(fdsrc,0,SEEK_END);
	lseek(fdsrc,0,SEEK_SET);
	readbuf=malloc(sizeof(char)*size+8);
	int n_read=read(fdsrc,readbuf,size);
	fddes=open(argv[2],O_RDWR|O_CREAT,0600);
	int n_write=write(fddes,readbuf,strlen(readbuf));
	
	close(fddes);
	close(fdsrc);
	return 0;
}
