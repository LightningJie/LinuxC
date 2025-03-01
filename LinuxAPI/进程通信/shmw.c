#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
int main()
{
	char *shmaddr;
	key_t key;
	key=ftok(".",1);
	int shmid=shmget(key,1024*4,IPC_CREAT|0666);
	if(shmid==-1)
	{
		printf("shmget error");
		exit(-1);
	}
	shmaddr=shmat(shmid,0,0);
	printf("chmat ok\n");
	strcpy(shmaddr,"wangwenjie");

	sleep(10);
	shmdt(shmaddr);
	shmctl(shmid,IPC_RMID,0);
	printf("quit\n");
	return 0;
}
