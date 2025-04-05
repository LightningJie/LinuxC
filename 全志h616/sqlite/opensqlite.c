#include <stdio.h>
#include <sqlite3.h>

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
	sqlite3_close(db);
	printf("done\n");
	return 0;
}
