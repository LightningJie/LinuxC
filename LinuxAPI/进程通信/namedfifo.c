#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

//       int mkfifo(const char *pathname, mode_t mode);
int main()
{
	char buf[1024]={0};
	if ((mkfifo("./file",0600)==-1)&& errno!=EEXIST)
	{
		printf("mkfifo failuer\n");
		perror("why");
	}
	int fd = open("./file",O_RDONLY);
	printf("open success\n");
	
	int nread=read(fd,buf,30);
	printf("read %d byte from fifo,context:%s \n",nread,buf);
	
	close(fd);
	return 0;
}
