#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
struct msgbuf {
               long mtype;       /* message type, must be > 0 */
               char mtext[128];    /* message data */
};
int main()
{
	struct msgbuf readbuf;
	struct msgbuf sendbuf={988,"thankyou"};
	key_t key;
	key=ftok(".",16);
	printf("key=%X\n",key);
	int msgID=msgget(key,IPC_CREAT|0777);
	printf("msgID=%X\n",msgID);
	if (msgID==-1)
	{
		printf("msg_queue create error\n");
	}
	msgrcv(msgID,&readbuf,sizeof(readbuf.mtext),888,0);
	printf("read from queue:%s\n",readbuf.mtext);

	msgsnd(msgID,&sendbuf,strlen(sendbuf.mtext),0);
        printf("send over");
	
	msgctl(msgID,IPC_RMID,NULL);
	return 0;
}
