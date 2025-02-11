#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	int fd;
	char *buf="我在通过write写入到shell\n";
	char *buf1;
	write(1,buf,strlen(buf));
	buf1=malloc(sizeof(char)*5);
	read(0,buf1,5);
	printf("我通过read从键盘读取：%s\n",buf1);
	return 0;
}
