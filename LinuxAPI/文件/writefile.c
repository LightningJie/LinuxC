#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
int main()
{
	int fd;
	char *buf="wangwenjie";
	fd=open("./file1",O_RDWR);
	if (fd ==-1)
	{
		printf("open file1 false!\n");
		fd=open("./file1",O_RDWR|O_CREAT,0600);
	}
	printf("Success open :fd = %d\n",fd);
	write(fd,buf,strlen(buf));
	printf("*p=%ld\n", strlen(buf));
	close(fd);
	return 0;
}
