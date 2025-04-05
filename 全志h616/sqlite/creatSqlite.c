#include <stdio.h>
#include <sqlite3.h>

int callback(void *arg, int column_size, char *column_value[], char*column_name[])
{
	int i;
	for (i=0;i<column_size;i++)
	{
		printf("%s = %s\n",column_name[i],column_value[i]);
	}
	printf("\n");
	return 0;
}

int main(int argc,char **argv)
{
	sqlite3 *db;
	char *errorMes=NULL;
	int ret;
	if (argc<2)
	{
		printf("Usage: %s xxx.db\n",argv[0]);
	}
	if(sqlite3_open(argv[1],&db)==SQLITE_OK)
	{
		printf("open %s success \n",argv[1]);
	}

	ret = sqlite3_exec(db,"create table class01(id Integer,name char,score Integer);",NULL,NULL,&errorMes);
	if (ret!=SQLITE_OK)
	{
		printf("create table error : %s\n",errorMes);
	}
	else{
		printf("create table success \n");
	}
	
	sqlite3_exec(db,"insert into class01 values(18130106,'huang',99);",NULL,NULL,&errorMes);
	sqlite3_exec(db,"select * from class01",callback,NULL,&errorMes);
	
	sqlite3_close(db);
	printf("done\n");
	return 0;
}
