#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
//       int mkfifo(const char *pathname, mode_t mode);
int main()
{
	char *str="mssage from fifo";
	int fd = open("./file",O_WRONLY);
	printf("open success\n");
	
	write(fd,str,strlen(str));
	close(fd);
	return 0;
}
