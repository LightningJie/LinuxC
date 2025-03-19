#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc,char **argv)
{
	int s_fd,c_fd;
	int n_read;
	char readBuf[128];
	char msg[128]={0};
	struct sockaddr_in s_addr;
	struct sockaddr_in c_addr;

	memset(&s_addr,0,sizeof(struct sockaddr_in));
	memset(&c_addr,0,sizeof(struct sockaddr_in));
	s_fd=socket(AF_INET,SOCK_STREAM,0);
	if (s_fd==-1)
	{
		perror("socket:");
		exit(-1);
	}
	s_addr.sin_family=AF_INET;
	s_addr.sin_port=htons(atoi(argv[2]));
	inet_aton(argv[1],&s_addr.sin_addr);

	bind(s_fd,(struct sockaddr*) &s_addr,sizeof(struct sockaddr_in));
	listen(s_fd,10);
	socklen_t clen=sizeof(struct sockaddr_in);
	while(1)
	{
		c_fd=accept(s_fd,(struct sockaddr *)&c_addr,&clen);
		if(c_fd==-1)
		{
			perror("accept:");
		}
		printf("get connect : %s \n",inet_ntoa(c_addr.sin_addr));
		if(fork()==0)
		{
			if(fork()==0)
			{
				memset(msg,0,sizeof(msg));
				printf("input: ");
				gets(msg);
				write(c_fd,msg,strlen(msg));
				exit(0);
			}
			n_read=read(c_fd,readBuf,128);
			if(n_read==-1)
			{
				perror ("read");
			}
			else{
				printf("get message:%s \n",readBuf);
			}
			exit(0);
		}
	}
	return 0;
}
