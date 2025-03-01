#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[128];    /* message data */
};
int main()
{
	struct msgbuf sendbuf={888,"this is message from queue"};
	struct msgbuf readbuf;
	key_t key;
        key=ftok(".",16);
        printf("key=%X\n",key);

	int msgID=msgget(key,IPC_CREAT|0777);
	if (msgID==-1)
	{
		printf("msg_queue create error\n");
	}
	//printf("char mtext[128] sizeof(mtext)=%d  strlen(mtext)=%d\n",sizeof(sendbuf.mtext),strlen(sendbuf.mtext));
	msgsnd(msgID,&sendbuf,strlen(sendbuf.mtext),0);
	printf("send over");

	msgrcv(msgID,&readbuf,sizeof(readbuf.mtext),988,0);
	printf("retuen from read:%s\n",readbuf.mtext);
	
	msgctl(msgID,IPC_RMID,NULL);

	return 0;
}
