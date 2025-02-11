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
- 参数
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
- 判断文件是否存在

~~~ c
int main()
{
        int fd;
        fd=open("./file1",O_RDWR|O_CREAT|O_EXCL,0600);
        if (fd ==-1)
        {       
                printf("open file1 has existed!\n");
        }   
        return 0; 
}          
~~~
- 每次写时都加到文件的尾端

## 读写write /read

~~~ c
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
ssize_t read(int fd, void *buf, size_t count);
~~~
注意，第二个参数并没有规定是字符串指针还是整型指针

## 光标定位 lseek
~~~ c
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
~~~
- 第二个参数是偏移量
- 第三个参数是文件头，文件当前光标位置，文件尾
SEEK_SET：The file offset is set to offset bytes.
SEEK_CUR：The file offset is set to its current location plus offset bytes.
SEEK_END：The file offset is set to the size of the file plus offset bytes.
## 关闭       close

## 创建 
~~~ c
 #include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>      
 int creat(const char *pathname, mode_t mode);
~~~
- 参数
  S_IRUSR   可读
  S_IWUSR   可写
  S_IXUSR   可执行
  S_IRWXU   可读、写、执行
## 文件描述符
- 1、对于内核而言，所有打开文件都由文件描述符引用。文件描述符是一个非负整数。当打开一个现存文件或者创建一个新文件时，内核向进程返回一个文件描述符。当读写一个文件时，用open和creat返回的文件描述符标识该文件，将其作为参数传递给read和write按照惯例，UNIX shell使用**文件描述符0与进程的标准输入相结合**，**文件描述符1与标准输出相结合**，**文件描述符2与标准错误输出相结合**。STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO这几个宏代替了0、1、2这几个魔数。
~~~ c
        read(0,buf,5);//从键盘读取5个字节数
        write(1,buf,5);//输出到shell里5个字节
~~~
- 2、文件描述符，这个数字在一个进程中表示一个特定含义，当我们open一个文件时，操作系统在内存中构建了一些数据结构来表示这个动态文件，然后返回给应用程序一个数字作为文件描述符，这个数字就和我们内存中维护的这个动态文件的这些数据结构绑定上了，以后我们应用程序如果要操作这个动态文件，只需要用这个文件描述符区分。
- 3、**文件描述符的作用域就是当前进程**，出了这个进程文件描述符就没有意义了。open函数打开文件，打开成功返回一个文件描述符，打开失败，返回-1。

## 动态内存/静态内存
文件平时是存放在块设备中的文件系统文件中的，我们把这种文件叫**静态文件**，当我们去open打开一个文件时，linux内核做的操作包括:内核在进程中建立一个打开文件的数据结构，记录下我们打开的这个文件;内核在内存中申请一段内存，并且将静态文件的内容从块设备中读取到内核中特定地址管理存放(叫**动态文件**)。read和write都是对动态文件进行操作的。

## 手工实现 linux CP
cp指令：
- cp src.c des.c

思路：
- 1.打开src.c 
- 2.读src.c 到buf
- 3.创建des.c
- 4.将buf 写入des.c
- 5.close两个文件
  
补充知识：
- 字符串定义
~~~ c
char *buf//本质是一个指针，指向字符串
~~~
- 字符串数组
~~~ c
char **BUF;//本质是指针的指针，指向指针
~~~

## 文件操作函数 open 和 fopen 的区别
### 来源：
- open 是 UNIX 系统调用函数（包括 LINUX 等），返回的是文件描述符（File Descriptor），它是文件在文件描述符表里的索引。
- fopen 是 ANSIC 标准中的 C 语言库函数，在不同的系统中应该调用不同的内核 API。返回的是一个指向文件结构的指针。
- PS：从来源来看，两者是有千丝万缕的联系的，毕竟 C 语言的库函数还是需要调用系统 API 实现的。
### 移植性
- fopen 是 C 标准函数，因此拥有良好的移植性。
- open 是 UNIX 系统调用，移植性有限。如 Windows 下相似的功能使用 API 函数 CreateFile。
### 适用范围
- open 返回文件描述符，适用于 UNIX 系统下的一切设备（如网络套接字、硬件设备等）以及普通正规文件。
- fopen 主要用于操纵普通正规文件。
### 文件 IO 层次
- open 属于低级 IO 函数，离系统内核更近，运行在内核态。
- fopen 属于高级 IO 函数，运行在用户态。
### 缓冲
1. 缓冲文件系统：
特点：在内存开辟一个“缓冲区”，为程序中的每一个文件使用。
读写操作通过缓冲区进行，提高效率。
相关函数：fopen, fclose, fread, fwrite, fgetc, fgets, fputc, fputs, freopen, fseek, ftell, rewind 等。
2. 非缓冲文件系统：
依赖于操作系统，通过操作系统的功能对文件进行读写。
不设文件结构体指针，只能读写二进制文件。
相关函数：open, close, read, write, getc, getchar, putc, putchar 等。
一句话总结：open 无缓冲，fopen 有缓冲。
### 性能与效率
- 使用 fopen 函数时，由于用户态下已有缓冲，减少了用户态和内核态的切换，因此在顺序访问文件时效率较高。
- 使用 open 函数时，每次文件读写都需要进行内核态和用户态的切换，但在随机访问文件时可能表现更佳。
通过以上总结梳理，相信读者对 open 和 fopen 及其系列函数有了更全面清晰的认识，并能在合适的场合下选择使用更合适的函数以提高效率。


