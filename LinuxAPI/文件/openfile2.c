 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <stdio.h>

int main()
{
	int fd;
	fd=open("./file1",O_RDWR);
	if (fd ==-1)
	{
		printf("open file1 false!\n");
		fd=open("./file1",O_RDWR|O_CREAT,0600);
	}
	return 0;
}
