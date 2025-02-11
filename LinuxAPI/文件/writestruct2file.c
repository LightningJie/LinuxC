#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Test
{
	int a;
	char c;
};
int main(int argc,char **argv)
{

	int fdsrc;
	struct Test data ={100,'a'};
	struct Test data2;
	if (argc!=2)
	{
		printf("params count error\n");
		exit(-1);
	}
	fdsrc=open(argv[1],O_RDWR);
	int n_write=write(fdsrc,&data,sizeof(struct Test));
	lseek(fdsrc,0,SEEK_SET);
	int n_read=read(fdsrc,&data2,sizeof(struct Test));
	printf("read:%d, %c\n",data2.a,data2.c);
	close(fdsrc);
	return 0;
}
