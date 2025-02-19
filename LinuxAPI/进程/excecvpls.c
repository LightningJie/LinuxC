//文件execl.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//函数原型：int execl(const char *path, const char *arg, ...);

int main(void)
{
    printf("before execl\n");
    char *argv[]={"ls",NULL};
    if(execvp("ls",argv) == -1)
    {
        printf("execl failed!\n");      
    	perror("why:");
    }

    printf("after execl\n");
    return 0;
}
