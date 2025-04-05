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

	if (argc<2)
	{
		printf("Usage: %s xxx.db\n",argv[0]);
	}
	if(sqlite3_open(argv[1],&db)==SQLITE_OK)
	{
		printf("open %s success \n",argv[1]);
	}
	sqlite3_exec(db,"select * from stu",callback,NULL,NULL);
	sqlite3_close(db);
	printf("done\n");
	return 0;
}
