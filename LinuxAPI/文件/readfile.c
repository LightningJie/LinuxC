#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	int fd,n_write;
	char *buf="wangwenjie";
	char *readbuf;
	fd=open("./file1",O_RDWR);
	if (fd ==-1)
	{
		printf("open file1 false!\n");
		fd=open("./file1",O_RDWR|O_CREAT,0600);
	}
	printf("Success open :fd = %d\n",fd);
	n_write=write(fd,buf,strlen(buf));
	readbuf=malloc(sizeof(char)*n_write);
	close(fd);
	fd=open("./file1",O_RDWR);
	read(fd,readbuf,n_write);
	printf("readbuf:%s\n",readbuf);
	close(fd);
	return 0;
}
