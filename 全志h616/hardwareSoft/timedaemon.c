#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
//C 库函数 char *asctime(const struct tm *timeptr) 返回一个指向字符串的指针，它代表了结构 struct timeptr 的日期和时间。
//C 库函数 struct tm *localtime(const time_t *timer) 使用 timer 的值来填充 tm 结构。timer 的值被分解为 tm 结构，并用本地时区表示。
/*
   struct tm {
   int tm_sec; 秒，范围从 0 到 59
   int tm_min; 分，范围从 0 到 59
   int tm_hour; 小时，范围从 0 到 23
   int tm_mday; 一月中的第几天，范围从 1 到 31
   int tm_mon; 月份，范围从 0 到 11
   int tm_year; 自 1900 起的年数
   int tm_wday; 一周中的第几天，范围从 0 到 6
   int tm_yday; 一年中的第几天，范围从 0 到 365
   int tm_isdst; 夏令时
   };
   */
static bool flag = true;
void handler(int sig)
{
	printf("I got a signal %d\nI'm quitting.\n", sig);
	flag = false;
}
int main()
{
	time_t t;
	int fd;
	//创建守护进程
	if(-1 == daemon(0, 0))
	{
		printf("daemon error\n");
		exit(1);
	}
	//设置信号处理函数
	struct sigaction act;
	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	if(sigaction(SIGQUIT, &act, NULL))
	{
		printf("sigaction error.\n");
		exit(0);
	}
	//进程工作内容
	while(flag)
	{
		fd = open("/home/orangepi/daemon.log", O_WRONLY | O_CREAT | O_APPEND,
				0644);
		if(fd == -1)
		{
			printf("open error\n");
		}
		t = time(0);
		char *buf = asctime(localtime(&t));
		write(fd, buf, strlen(buf));
		close(fd);
		sleep(10);
	}
	return 0;
}

