# 线程
## 进程与线程
　典型的UNIX/Linux进程可以看成只有一个控制线程：一个进程在同一时刻只做一件事情。有了多个控制线程后，在程序设计时可以把进程设计成在同一时刻做不止一件事，每个线程各自处理独立的任务。　　
**进程是程序执行时的一个实例，是担当分配系统资源（CPU时间、内存等）的基本单位**。在面向线程设计的系统中，进程本身不是基本运行单位，而是线程的容器。程序本身只是指令、数据及其组织形式的描述，进程才是程序（那些指令和数据）的真正运行实例。

线程是操作系统能够进行运算调度的最小单位。它被包含在进程之中，是进程中的实际运作单位。一条线程指的是进程中一个单一顺序的控制流，一个进程中可以并发多个线程，每条线程并行执行不同的任务。**线程包含了表示进程内执行环境必须的信息，其中包括进程中表示线程的线程ID、一组寄存器值、栈、调度优先级和策略、信号屏蔽字、errno常量以及线程私有数据**。进程的所有信息对该进程的所有线程都是共享的，包括可执行的程序文本、程序的全局内存和堆内存、栈以及文件描述符。

**进程——资源分配的最小单位，线程——程序执行的最小单位**

**进程有独立的地址空间**，一个进程崩溃后，在保护模式下不会对其它进程产生影响，而线程只是一个进程中的不同执行路径。线程有自己的堆栈和局部变量，但**线程没有单独的地址空间**，一个线程死掉就等于整个进程死掉，所以多进程的程序要比多线程的程序健壮，但在进程切换时，耗费资源较大，效率要差一些。但对于一些要求同时进行并且又要共享某些变量的并发操作，用线程比较方便，进程还需要进程间的通信。

　　从函数调用上来说，进程创建使用fork()操作；线程创建使用clone()操作。

**使用线程的理由**
　　
- 理由之一是和进程相比，它是一种非常"节俭"的多任务操作方式。我们知道，在Linux系统下，启动一个新的进程必须分配给它独立的地址空间，建立众多的数据表来维护它的代码段、堆栈段和数据段，这是一种"昂贵"的多任务工作方式。而运行于一个进程中的多个线程，它们彼此之间使用相同的地址空间，共享大部分数据，启动一个线程所花费的空间远远小于启动一个进程所花费的空间，而且，线程间彼此切换所需的时间也远远小于进程间切换所需要的时间。据统计，总的说来，一个进程的开销大约是一个线程开销的30倍左右，当然，在具体的系统上，这个数据可能会有较大的区别。

- 理由之二是线程间方便的通信机制。对不同进程来说，它们具有独立的数据空间，要进行数据的传递只能通过通信的方式进行，这种方式不仅费时，而且很不方便。线程则不然，由于同一进程下的线程之间共享数据空间，所以一个线程的数据可以直接为其它线程所用，这不仅快捷，而且方便。当然，数据的共享也带来其他一些问题，有的变量不能同时被两个线程所修改，有的子程序中声明为static的数据更有可能给多线程程序带来灾难性的打击，这些正是编写多线程程序时最需要注意的地方。
  
## 多线程开发的最基本概念
### 线程
线程操作又分线程的创建，退出，等待

1. 线程创建
~~~c
#include <pthread.h>
int pthread_create(pthread_t *restrict tidp, const pthread_attr_t *restrict attr, void *(*start_rtn)(void *), void *restrict arg);
// 返回：若成功返回0，否则返回错误编号
~~~
- 当pthread_create成功返回时，由tidp指向的内存单元被设置为新创建线程的线程ID。attr参数用于定制各种不同的线程属性，暂可以把它设置为NULL，以创建默认属性的线程。
- **新创建的线程从start_rtn函数的地址开始运行**，该函数只有一个无类型指针参数arg。如果需要向start_rtn函数传递的参数不止一个，那么需要把这些参数放到一个结构中，然后把这个结构的地址作为arg参数传入。
 `void *(*start_rtn)(void *)`是函数的指针，函数指针的用法如下
 ~~~
                    返回值类型 (*指针名)(参数列表);
 ~~~
假设我们有一个函数：
~~~c
void print_message(int num) {
    printf("The number is: %d\n", num);
}
~~~
- 这个函数的返回值类型是 void，参数是一个 int。如果我们要定义一个指向这个函数的指针，可以这样写：
~~~c
void (*func_ptr)(int);  // 定义一个函数指针
~~~
- void 是返回值类型。
- (*func_ptr) 表示这是一个指针。
- (int) 是函数的参数列表。
1. 线程退出
　　单个线程可以通过以下三种方式退出，在不终止整个进程的情况下停止它的控制流：
　　1）线程只是从启动例程中返回，返回值是线程的退出码。
　　2）线程可以被同一进程中的其他线程取消。
　　3）线程调用pthread_exit：
~~~c
#include <pthread.h>
int pthread_exit(void *rval_ptr);
~~~
- rval_ptr是一个无类型指针，与传给启动例程的单个参数类似。进程中的其他线程可以通过调用pthread_join函数访问到这个指针。

1. 线程等待
~~~c
#include <pthread.h>
int pthread_join(pthread_t thread, void **rval_ptr);
// 返回：若成功返回0，否则返回错误编号
~~~
调用这个函数的线程将一直阻塞，直到指定的线程调用pthread_exit、从启动例程中返回或者被取消。如果例程只是从它的启动例程返回i，rval_ptr将包含返回码。如果线程被取消，由rval_ptr指定的内存单元就置为PTHREAD_CANCELED。

可以通过调用pthread_join自动把线程置于分离状态，这样资源就可以恢复。如果线程已经处于分离状态，pthread_join调用就会失败，返回EINVAL。

如果对线程的返回值不感兴趣，可以把rval_ptr置为NULL。在这种情况下，调用pthread_join函数将等待指定的线程终止，但并不获得线程的终止状态。

4. 线程脱离
　　一个线程或者是可汇合（joinable，默认值），或者是脱离的（detached）。当一个可汇合的线程终止时，它的线程ID和退出状态将留存到另一个线程对它调用pthread_join。脱离的线程却像守护进程，当它们终止时，所有相关的资源都被释放，我们不能等待它们终止。如果一个线程需要知道另一线程什么时候终止，那就最好保持第二个线程的可汇合状态。
　　pthread_detach函数把指定的线程转变为脱离状态。
~~~c
#include <pthread.h>
int pthread_detach(pthread_t thread);
// 返回：若成功返回0，否则返回错误编号
　　本函数通常由想让自己脱离的线程使用，就如以下语句：

pthread_detach(pthread_self());
~~~
5. 线程ID获取及比较
~~~c
#include <pthread.h>
pthread_t pthread_self(void);
// 返回：调用线程的ID
~~~
　　对于线程ID比较，为了可移植操作，我们不能简单地把线程ID当作整数来处理，因为不同系统对线程ID的定义可能不一样。我们应该要用下边的函数：

~~~c
#include <pthread.h>
int pthread_equal(pthread_t tid1, pthread_t tid2);
// 返回：若相等则返回非0值，否则返回0
~~~

对于多线程程序来说，我们往往需要对这些多线程进行同步。同步（synchronization）是指在一定的时间内只允许某一个线程访问某个资源。而在此时间内，不允许其它的线程访问该资源。我们可以通过互斥锁（mutex）
### 互斥锁
互斥锁则包括 4 种操作，分别是创建，销毁，加锁和解锁

### 创建及销毁互斥锁
~~~c
#include <pthread.h>
int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
// 返回：若成功返回0，否则返回错误编号
~~~
要用默认的属性初始化互斥量，只需把attr设置为NULL。　　

### 加锁及解锁
~~~c
#include <pthread.h>
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
// 返回：若成功返回0，否则返回错误编号
~~~
如果线程不希望被阻塞，它可以使用pthread_mutex_trylock尝试对互斥量进行加锁。如果调用pthread_mutex_trylock时互斥量处于未锁住状态，那么pthread_mutex_trylock将锁住互斥量，不会出现阻塞并返回0，否则pthread_mutex_trylock就会失败，不能锁住互斥

### 互斥量和信号量的区别
在 Linux 中，信号量（Semaphore）和互斥量（Mutex）的 API 主要分为 POSIX 标准接口和 System V 接口。
1. 互斥量（Mutex）API
互斥量主要用于线程间的同步，其 API 主要基于 POSIX 线程库（pthread）。以下是常用的互斥量 API：
~~~c

pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)//初始化互斥量。
pthread_mutex_destroy(pthread_mutex_t *mutex)//销毁互斥量。
锁操作
pthread_mutex_lock(pthread_mutex_t *mutex)//加锁，阻塞直到获取锁。
pthread_mutex_trylock(pthread_mutex_t *mutex)//尝试加锁，若锁已被占用则立即返回。
pthread_mutex_unlock(pthread_mutex_t *mutex)//解锁。
~~~
2. 信号量（Semaphore）API
信号量分为有名信号量和无名信号量，分别用于进程间同步和线程间同步。
- POSIX 信号量 API
    有名信号量：
~~~c
sem_t *sem_open(const char *name, int oflag, mode_t mode, unsigned int value)：创建或打开一个有名信号量。
int sem_close(sem_t *sem)：关闭信号量。
int sem_unlink(const char *name)：删除信号量。
~~~
无名信号量：
~~~c
int sem_init(sem_t *sem, int pshared, unsigned int value)：初始化无名信号量。
int sem_destroy(sem_t *sem)：销毁无名信号量。
信号量操作
int sem_wait(sem_t *sem)：P 操作，等待信号量值大于 0 并将其减 1。
int sem_trywait(sem_t *sem)：尝试 P 操作，若信号量值为 0 则立即返回。
int sem_post(sem_t *sem)：V 操作，增加信号量值。
~~~
- System V 信号量 API
System V 信号量主要用于进程间同步，其 API 包括：
~~~c
int semget(key_t key, int nsems, int semflg)：创建或获取一个信号量集。
int semop(int semid, struct sembuf *sops, size_t nsops)：对信号量执行 P 或 V 操作。
int semctl(int semid, int semnum, int cmd, ...)：控制信号量集的属性，如删除信号量集。
~~~


### 条件。
条件操作有 5 种操作：创建，销毁，触发，广播和等待。
条件变量是线程另一可用的同步机制。条件变量给多个线程提供了一个会合的场所。条件变量与互斥量一起使用时，允许线程以无竞争的方式等待特定的条件发生。
　　条件本身是由互斥量保护的。线程在改变条件状态前必须首先锁住互斥量，其他线程在获得互斥量之前不会察觉到这种改变，因为必须锁定互斥量以后才能计算条件。
　　条件变量使用之前必须首先初始化，pthread_cond_t数据类型代表的条件变量可以用两种方式进行初始化，可以把常量PTHREAD_COND_INITIALIZER赋给静态分配的条件变量，但是如果条件变量是动态分配的，可以使用pthread_cond_destroy函数对条件变量进行去除初始化（deinitialize）。

1.  创建及销毁条件变量
~~~c
#include <pthread.h>
int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
int pthread_cond_destroy(pthread_cond_t cond);
// 返回：若成功返回0，否则返回错误编号
~~~
　　除非需要创建一个非默认属性的条件变量，否则pthread_cont_init函数的attr参数可以设置为NULL。

2. 等待
~~~c
#include <pthread.h>
int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
int pthread_cond_timedwait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex, cond struct timespec *restrict timeout);
// 返回：若成功返回0，否则返回错误编号
~~~
　　pthread_cond_wait等待条件变为真。如果在给定的时间内条件不能满足，那么会生成一个代表一个出错码的返回变量。传递给pthread_cond_wait的互斥量对条件进行保护，调用者把锁住的互斥量传给函数。函数把调用线程放到等待条件的线程列表上，然后对互斥量解锁，这两个操作都是原子操作。这样就关闭了条件检查和线程进入休眠状态等待条件改变这两个操作之间的时间通道，这样线程就不会错过条件的任何变化。pthread_cond_wait返回时，互斥量再次被锁住。

　　pthread_cond_timedwait函数的工作方式与pthread_cond_wait函数类似，只是多了一个timeout。timeout指定了等待的时间，它是通过timespec结构指定。

3. 触发
~~~c
#include <pthread.h>
int pthread_cond_signal(pthread_cond_t cond);
int pthread_cond_broadcast(pthread_cond_t cond);
// 返回：若成功返回0，否则返回错误编号
~~~
　　这两个函数可以用于通知线程条件已经满足。pthread_cond_signal函数将唤醒等待该条件的某个线程，而pthread_cond_broadcast函数将唤醒等待该条件的所有进程。

