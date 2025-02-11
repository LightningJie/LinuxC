 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 #include <stdio.h>

int main()
{
	int fd;
	fd=open("./file1",O_RDWR|O_CREAT|O_EXCL,0600);
	if (fd ==-1)
	{
		printf("open file1 has existed!\n");
	}
	return 0;
}
