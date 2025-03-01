#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <unistd.h>
union semun {
               int              val;    /* Value for SETVAL */
               struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
               unsigned short  *array;  /* Array for GETALL, SETALL */
               struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
           };
void p(int id)
{
	struct sembuf set;
	set.sem_num=0;
	set.sem_op=-1;
	set.sem_flg=SEM_UNDO;
	semop(id,&set,1);
	printf("p操作 get key\n");

}
void v(int id)
{
        struct sembuf set;
        set.sem_num=0;
        set.sem_op=1;
        set.sem_flg=SEM_UNDO;
        semop(id,&set,1);
        printf("v操作 put back key\n");

}
int main(int argc,char const * argv)
{
	key_t key;
	int semid;
	key=ftok(".",2);
	semid=semget(key,1,IPC_CREAT|0666);
	union semun initsem;
	initsem.val=0;
	semctl(semid,0,SETVAL,initsem);
	int pid = fork();
	if (pid>0)
	{
		p(semid);
		printf("this is father\n");
		v(semid);
	}else if (pid ==0)
	{
		printf("this is child\n");
		v(semid);
	}else
	{
		printf("fork error\n");
	}

}
