 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <stdio.h>

int main()
{
	int fd;
	fd=open("./file1",O_RDWR);
	printf("fd= %d \n",fd);
	return 0;
}
