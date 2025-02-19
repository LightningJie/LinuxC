#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
int main()
{
	pid_t re=-2;
	int data;
	while(1){
		printf("please input a data\n");
		scanf("%d",&data);
		if (data==1)
		{
		
			re =fork();
			if(re>0)
			{
				wait(NULL);
			}		
			if(re==0) 
			{
				int fdsrc;
				char *readbuf;
				fdsrc=open("config.txt",O_RDWR);
				int size=lseek(fdsrc,0,SEEK_END);
				lseek(fdsrc,0,SEEK_SET);
				readbuf=malloc(sizeof(char)*size+8);
				int n_read=read(fdsrc,readbuf,size);
				char *p=strstr(readbuf,"LENG=");
				if (p==NULL)
				{
					printf("not found\n");
					exit(-1);
				}
				p=p+strlen("LENG=");
				*p='5';
				lseek(fdsrc,0,SEEK_SET);
				int n_write=write(fdsrc,readbuf,strlen(readbuf));

				close(fdsrc);
				printf("changedata right\n");
				exit(0);
			}
		}
		else{
			printf("wait,do nothing\n");
		}
	}
	exit(0);
}
