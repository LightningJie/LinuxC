# Linux文件
## 打开open
~~~ c
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
 int open(const char *pathname, int flags);
 int open(const char *pathname, int flags, mode_t mode)
 int creat(const char *pathname, mode_t mode);
~~~
参数
~~~
Pathname:要打开的文件名（含路径,缺省为当前路径）
Flags:
        O_RDONLY 只读打开
        O_WRONLY 只写打开 
        O_RDWR  可读可写打开
    当我们附带了权限后，打开的文件就只能按照这种权限来操作。
    以上这三个常数中应当只指定一个。下列常数是可选择的：     
        O_CREAT 若文件不存在则创建它。使用此选项时，需要同时说明第三个参数mode，用其说明该新文件的存取许可权限。
        O_EXCL 如果同时指定了OCREAT，而文件已经存在，则出错。        
        O_APPEND 每次写时都加到文件的尾端。
        O_TRUNC 属性去打开文件时，如果这个文件中本来是有内容的，而且为只读或只写成功打开，则将其长度截短为0。

Mode:一定是在flags中使用了O_CREAT标志，mode记录待创建的文件的访问权限
~~~
## 读写write /read

~~~ c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
~~~

## 光标定位 lseek

## 关闭       close
