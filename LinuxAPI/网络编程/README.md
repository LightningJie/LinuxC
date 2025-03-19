# 网络编程

## TCP和UDP的比较
1. TCP面向连接（如打电话要先拨号建立连接）;UDP是无连接的，即发送数据之前       不需  要建立连接
2.  TCP提供可靠的服务。也就是说，通过TCP连接传送的数据，无差错，不丢失，不重复，且按序到达;UDP尽最大努力交付，即不保证可靠交付
3.  TCP面向字节流，实际上是TCP把数据看成一连串无结构的字节流;UDP是面向报文的
UDP没有拥塞控制，因此网络出现拥塞不会使源主机的发送速率降低（对实时应用很有用，如IP电话，实时视频会议等）
4.  每一条TCP连接只能是点到点的;UDP支持一对一，一对多，多对一和多对多的交互通信
5.  TCP首部开销20字节;UDP的首部开销小，只有8个字节
6.  TCP的逻辑通信信道是全双工的可靠信道，UDP则是不可靠信道

## 端口号作用
一台拥有IP地址的主机可以提供许多服务，比如Web服务、FTP服务、SMTP服务等

这些服务完全可以通过1个IP地址来实现。那么，主机是怎样区分不同的网络服务呢？显然不能只靠IP地址，因为IP 地址与网络服务的关系是一对多的关系。

实际上是通过“IP地址+端口号”来区 分不同的服务的。
端口提供了一种访问通道，
服务器一般都是通过知名端口号来识别的。例如，对于每个TCP/IP实现来说，FTP服务器的TCP端口号都是21，每个Telnet服务器的TCP端口号都是23，每个TFTP(简单文件传送协议)服务器的UDP端口号都是69。

## 字节序

### Little endian 小端字节序
低字节序放在低地址          
例如32位操作系统中，一个字为：0x 01 02 03 04（01为高序，04为低序）
~~~
内存地址  
    4003 | 01   |
    4002 | 02   |
    4001 | 03   |
    4000 | 04   |
~~~
### Big endian   大端字节序
高字节序，放在低地址
例如32位操作系统中，0x 01 02 03 04
~~~
内存地址  
    4003 | 04   |
    4002 | 03   |
    4001 | 02   |
    4000 | 01   |
~~~
### 网络字节序=大端字节序 
### 计算机字节序=小端字节序

### 字节序转换API

~~~C
#include <arpa/inet.h>

uint16_t htons(uint16_t host16bitvalue);    //返回网络字节序的值
uint32_t htonl(uint32_t host32bitvalue);    //返回网络字节序的值
uint16_t ntohs(uint16_t net16bitvalue);     //返回主机字节序的值
uint32_t ntohl(uint32_t net32bitvalue);     //返回主机字节序的值
~~~
h代表host，n代表net，s代表short（两个字节），l代表long（4个字节），通过上面的4个函数可以实现主机字节序和网络字节序之间的转换。有时可以用INADDR_ANY，INADDR_ANY指定地址让操作系统自己获取



## socket编程
套接字是一种通信端点，它允许程序通过网络发送和接收数据。它类似于电话插孔，程序可以通过它与另一个套接字（在本地或远程主机上）建立连接并交换数据。套接字可以支持多种通信协议，如 TCP、UDP 或 Unix 域套接字。
socket() 系统调用用于创建一个新的套接字，并返回一个套接字描述符（类似于文件描述符）。这个描述符可以用于后续的网络操作，如绑定地址、监听连接、发送和接收数据等。
### 服务器端
1. 创建套接字 
   socket()
2. 为套接字添加信息（IP地址和端口号）
   bind()
3. 监听网络连接
   listen()
4. 监听到有客户端接入，接受一个连接
   accept()
5. 数据交互
   read() write()
6. 关闭套接字，断开连接
   close()
### 客户端


### socket() 的函数原型
~~~c
#include <sys/types.h>
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
~~~

- domain（地址族）：
指定套接字使用的协议族。常见的值包括：
AF_INET：IPv4 协议族。
AF_INET6：IPv6 协议族。
AF_UNIX 或 AF_LOCAL：用于本地进程间通信的 Unix 域套接字。
AF_UNSPEC：不指定协议族，通常用于查询操作。
- type（套接字类型）：
指定套接字的通信类型。常见的值包括：
SOCK_STREAM：面向连接的可靠字节流（TCP）。
SOCK_DGRAM：无连接的不可靠数据报（UDP）。
SOCK_RAW：原始套接字，直接操作 IP 数据包。
SOCK_SEQPACKET：面向连接的可靠数据报。
SOCK_RDM：可靠数据报（很少使用）。
- protocol（协议）：
指定使用的协议。通常情况下，如果协议族和套接字类型已经确定，protocol 参数可以设置为 0，让系统自动选择默认协议。例如：
对于 AF_INET 和 SOCK_STREAM，默认协议是 TCP。
对于 AF_INET 和 SOCK_DGRAM，默认协议是 UDP。
- 返回值
成功时，socket() 返回一个非负整数，称为套接字描述符（Socket Descriptor），类似于文件描述符。
失败时，返回 -1，并设置 errno 以指示错误原因。
### bind() 的函数原型
~~~c
#include <sys/socket.h>
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
~~~
- sockfd：由 socket() 系统调用返回的套接字文件描述符。
- addr：指向 sockaddr 结构体的指针，该结构体包含了要绑定的地址信息。在传参时候，sockaddr_in会强制类型转换为sockaddr
- addrlen：addr 结构体的大小。
- 返回值
成功时返回 0。
失败时返回 -1，并设置 errno 以指示错误原因
~~~c
struct sockaddr {
    unsigned short sa_family;  // 地址族（2字节）
    char sa_data[14];          // 地址数据（14字节）
};
struct sockaddr_in {
    short sin_family;       // 地址族，必须是 AF_INET
    unsigned short sin_port; // 端口号，网络字节序
    struct in_addr sin_addr; // IPv4 地址
    char sin_zero[8];        // 填充字段，通常不使用
};
struct sockaddr_in6 {
    short sin6_family;       // 地址族，必须是 AF_INET6
    unsigned short sin6_port; // 端口号，网络字节序
    unsigned long sin6_flowinfo; // 流信息
    struct in6_addr sin6_addr;   // IPv6 地址
    unsigned long sin6_scope_id; // 范围 ID
};
~~~

### inet_aton函数原型
inet_aton 是一个用于将 IPv4 地址从点分十进制格式（如 192.168.1.1）转换为网络字节序的 32 位二进制值的函数
~~~c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int inet_aton(const char *cp, struct in_addr *inp);
~~~
- cp：指向点分十进制格式的 IPv4 地址字符串（如 "192.168.1.1"）。
- inp：指向 struct in_addr 结构的指针，用于存储转换后的 32 位网络字节序整数。
- 返回值
成功时返回非零值（通常为 1）。
如果输入的 IP 地址格式不正确，则返回 0。

### inet_ntoa函数原型
inet_ntoa() 是一个用于将 IPv4 地址从 32 位网络字节序整数转换为点分十进制格式字符串的函数。它在网络编程中非常常用，尤其是在需要将 IP 地址以人类可读的形式展示时。

~~~c
char *inet_ntoa(struct in_addr in);
~~~
- 参数：
in：一个 struct in_addr 类型的变量，包含一个 32 位的 IPv4 地址。
- 返回值：
成功时返回一个指向静态分配的字符串的指针，该字符串表示点分十进制格式的 IP 地址（如 192.168.1.1）。
如果失败，则返回 NULL。

### listen函数原型
listen() 是一个网络编程中的系统调用，用于将一个套接字转换为被动模式（监听模式），使其能够接受传入的连接请求。它是服务器端编程中非常重要的一步，通常在创建套接字并绑定地址后调用。
~~~c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
~~~
- sockfd：
由 socket() 系统调用返回的套接字描述符。这个套接字必须已经通过 bind() 绑定到一个地址和端口。
- backlog：
指定系统为该套接字维护的未完成连接队列的最大长度。当新的连接请求到达时，如果队列已满，系统会拒绝该连接。


### accept函数原型
accept() 是一个网络编程中的系统调用，用于服务器端接受客户端的连接请求。它是 **TCP 服务器编程**中非常重要的一步，通常在调用 listen() 后使用。accept() 会从监听队列中取出一个已完成的连接请求，并为该连接创建一个新的套接字。
accept() 的主要作用是从监听套接字的连接队列中取出一个已完成的连接请求，并为该连接创建一个新的套接字。新套接字用于与客户端进行数据传输，而原来的监听套接字继续等待新的连接请求。
~~~c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
~~~
- sockfd：
由 socket() 创建并经过 listen() 转换为监听状态的套接字描述符。
- addr：
指向 struct sockaddr 的指针，用于存储客户端的地址信息。如果不需要获取客户端的地址信息，可以传入 NULL。
- addrlen：
指向 socklen_t 类型的指针，表示 addr 的大小。调用前应设置为 sizeof(struct sockaddr) 或其他具体地址结构体的大小。调用后，addrlen 会被更新为实际存储的地址长度。
- 返回值
成功：返回一个新的套接字描述符，用于与客户端通信。
失败：返回 -1，并设置 errno 以指示错误原因。

### 数据收发函数
1. 字节流读取函数
~~~c
ssize_t read(int sockfd, void *buffer, size_t count);
ssize_t write(int sockfd, const void *buffer, size_t count);
~~~
2. TCP套接字
~~~c
ssize_t send(int sockfd, const void *buffer, size_t len, int flags);
~~~
sockfd：套接字描述符。
buffer：要发送的数据。
len：数据长度。
flags：控制发送行为的标志（如 MSG_NOSIGNAL 防止信号中断）。
返回值：实际发送的字节数，或在出错时返回 -1。
~~~c
ssize_t recv(int sockfd, void *buffer, size_t len, int flags);
~~~
sockfd：套接字描述符。
buffer：存储接收数据的缓冲区。
len：缓冲区大小。
flags：控制接收行为的标志（如 MSG_PEEK 查看数据而不消费）。
返回值：实际接收的字节数，或在出错时返回 -1。

### 客户端的connect函数
connect() 的主要作用是将客户端套接字与服务器套接字建立连接。它会向服务器发送一个连接请求，服务器通过 accept() 接受连接后，客户端和服务器之间就可以进行数据传输了。
~~~c
#include <sys/socket.h>
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
~~~
- sockfd：
由 socket() 创建的套接字描述符。客户端需要先创建一个套接字，然后才能使用 connect() 发起连接。
- addr：
指向 struct sockaddr 的指针，包含服务器的地址信息（如 IP 地址和端口号）。通常使用 struct sockaddr_in 或 struct sockaddr_in6 来表示 IPv4 或 IPv6 地址。
- addrlen：
addr 结构体的大小，通常为 sizeof(struct sockaddr_in) 或 sizeof(struct sockaddr_in6)。

