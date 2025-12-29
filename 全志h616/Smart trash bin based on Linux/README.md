# 基于Linux的智能垃圾桶

## 一、功能需求

- 靠近时，垃圾桶开启 2 秒，2 秒后关闭

- 垃圾桶开启带滴滴声

- 垃圾桶开启超过 10 秒，滴滴声警报

- 语音控制垃圾桶开关盖

- 回顾二阶段的 Socket 编程，实现 Socket 客户端发送指令远程打开 / 关闭垃圾桶，并显示垃圾桶状态

- 统计当天垃圾桶开关盖次数及开关盖指令来源并记录在文件中

- 统计当天垃圾桶开关盖次数及开关盖指令来源并记录在数据库中

- 拓展需求：图像识别垃圾分类功能

- 项目定位：着急找工作的，学到这里可以浅尝一波，在校生校招没有问题。

- 图像处理使用京东 SDK - 只支持 Python 和 Java 接口，目的是引入 C 语言的 Python 调用，感受大厂做的算法 bug

  此接口是人工智能接口，京东识别模型是通过训练后的模型，精准度取决于训练程度，人工智能范畴在常规嵌入式设备负责执行居多，嵌入式设备负责数据采集，然后转发给人工智能识别后，拿到结果进行执行器动作。

## 二、环境准备

### 2.1 安装镜像

`Orangepizero2_3.0.6_ubuntu_jammy_desktop_xfce_linux5.16.17` 

只能选取该镜像，该镜像比较新，自带python3.10 且比较稳定。

**安装wiringOP库**

```bash
cd wiringOP
sudo ./build

#测试是否安装成功
gpio readall
```

**打开串口S5 和i2c3**

```bash
sudo vim /boot/orangepiEnv.txt
#加上
overlays=uart5 i2c3
```

### 2.2 安装python3.9（略）

**1.安装编译python3.9的包所需要的工具**

```bash
sudo apt install -y build-essential zlib1g-dev libncurses5-dev libgdbm-dev libnss3-dev libssl-dev libsqlite3-dev libreadline-dev libffi-dev curl libbz2-dev
```

**2.下载python3.9的二进制安装包**

```
wget https://www.python.org/ftp/python/3.9.7/Python-3.9.7.tgz
```

**解压**

```
tar xvf Python-3.9.7.tgz
```

**对编译环境进行检查和配置，为后续的编译工作做好准备**

```bash
./configure --enable-shared --enable-optimizations
```

`--enable-optimizations`此选项用于开启编译优化。在启用该选项后，编译过程中会使用额外的优化参数，像自动检测并启用特定于 CPU 架构的优化，同时还会运行测试套件以进一步优化 Python 解释器的性能。这样做能够显著提升 Python 程序的运行速度，不过编译时间也会相应延长。

**编译**

```bash
make -j4
```

`-j4` 意味着同时开启 4 个编译任务并行执行。这样能充分利用多核 CPU 的性能(全志h616为4核)，加快编译速度。

**安装**

```
sudo make install
```

这样就把python3.9安装到了`/usr/local/bin`目录下

区别乌班图自带python安装路径`/usr/bin/`

**删除自带python2.7的软链接**

```
sudo rm -f /usr/bin/python
```

`-f` 是 `--force` 的缩写形式，其作用是强制删除文件或目录，忽略不存在的文件，并且不会给出提示信息。

**建立软连接到python3.9**

```
sudo ln -s /usr/local/bin/python3.9 /usr/bin/python
```

**安装pip**

```bash
sudo apt install -y python-pip python3-pip
```

配置pip镜像源

```bash
 cd ~
 mkdir .pip
 cd .pip
 vim pip.conf
```

```
[global]
timeout = 6000
index-url = https://pypi.tuna.tsinghua.edu.cn/simple
trusted-host = pypi.tuna.tsinghua.edu.cn
```

由于系统版本太低 `Orangepizero2_3.0.6_ubuntu_jammy_desktop_xfce_linux5.16.17` 升级该系统镜像，自带python3.10版本

## 三、技术详解

### 3.1 C语言调用python

验证是否安装`libpython3` ，libpython3为其他程序提供调用 Python 功能的接口。借助 `libpython3`，开发者能够在 C、C++ 等语言编写的程序中嵌入 Python 解释器，从而实现 Python 代码和其他语言代码的混合编程。比如，在 C 程序里调用 Python 函数、执行 Python 脚本等。

```
dpkg -l |grep libpython3

dpkg -l |grep libpython3 |grep dev
```

如果没有安装，则安装

```
sudo apt install libpython3.10-dev
```

测试是否正常运行

```c
#include "Python.h"
int main()
{
    Py_Initialize();
    PyRun_SimpleString("print('funny')");
    Py_Finalize();
}

```

```bash
gcc testpydev.c -o testpydev -I /usr/include/python3.10 -l python3.10
```

### 3.2 c语言调用python函数

```c
#if 1、包含Python.h头文件，以便使用Python API。 
2、使用void Py_Initialize()初始化Python解释器， 
3、使用PyObject *PyImport_ImportModule(const char *name)和PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)获取sys.path对象，并利用int PyList_Append(PyObject *list, PyObject *item)将当前路径. 添加到sys.path中，以便加载当前的Python模块(当前有Python文件即python模块)。 
4、使用PyObject *PyImport_ImportModule(const char *name)函数导入Python模块，并检查是否有错误。 
5、使用PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)函数获取Python函数对象，并检查是否可调用。 
6、使用PyObject *Py_BuildValue(const char *format, ...)函数创建一个Python元组，作为Python函数的参数,没有参数时不需要调用 
7、使用PyObject *PyObject_CallObject(PyObject *callable, PyObject *args)函数调用Python函数，并获取返回值。 
8、使用int PyArg_Parse(PyObject *args, const char *format, ...)函数将返回值转换为C类型，并检查是否有错误,没有返回值时不需要调用。 
9、使用void Py_DECREF(PyObject *o)函数释放所有引用的Python对象。 
10、结束时调用void Py_Finalize()函数关闭Python解释器。相关的函数说明参考网站（网站左上角输入函数名即可开始搜索）： https://docs.python.org/zh-cn/3/c-api/import.html 
#endif
```

- 无参调用详细代码请见`nopara.c` 文件中
- 有参调用详细请见`para.c` 文件中

​	执行完 C 语言调用 Python 代码的程序后，目录下出现 `__pycache__` 目录。Python 为了提高模块的加载速度，会在第一次导入模块时将编译后的字节码文件缓存起来。`__pycache__` 目录就是用于存放这些编译后的字节码文件的地方。下次再导入同一个模块时，Python 会优先检查 `__pycache__` 目录中是否存在对应的字节码文件，如果存在且该字节码文件的修改时间比源 `.py` 文件新（或者二者修改时间相同），就会直接加载字节码文件，而不是重新编译源文件，从而加快了模块的导入速度。

**PyObject *Py_BuildValue(const char *format, ...)**

| 格式代码 | C 语言参数类型                     | Python 对象类型 | 示例代码                                                     |
| -------- | ---------------------------------- | --------------- | ------------------------------------------------------------ |
| i        | int                                | int             | int num = 42; PyObject *py_num = Py_BuildValue("i", num);    |
| l        | long                               | int             | long l_num = 12345L; PyObject *py_l_num = Py_BuildValue("l", l_num); |
| L        | long long                          | int             | long long ll_num = 987654321LL; PyObject *py_ll_num = Py_BuildValue("L", ll_num); |
| f        | float                              | float           | float f_num = 3.14f; PyObject *py_f_num = Py_BuildValue("f", f_num); |
| d        | double                             | float           | double d_num = 2.71828; PyObject *py_d_num = Py_BuildValue("d", d_num); |
| s        | char*（以 '\0' 结尾）              | bytes           | char *str = "hello"; PyObject *py_str = Py_BuildValue("s", str); |
| s#       | char* 和 int（字符串和长度）       | bytes           | char *str = "hello"; int len = 5; PyObject *py_str = Py_BuildValue("s#", str, len); |
| u        | Py_UNICODE*（以 '\0' 结尾）        | str             | 需引入 Python 宽字符处理相关，如 Py_UNICODE *u_str = ...; PyObject *py_u_str = Py_BuildValue("u", u_str); |
| u#       | Py_UNICODE* 和 int（字符串和长度） | str             | 类似 s#，只是处理宽字符，如 Py_UNICODE *u_str = ...; int u_len = ...; PyObject *py_u_str = Py_BuildValue("u#", u_str, u_len); |
| ()       | 无                                 | 空元组 tuple    | PyObject *empty_tuple = Py_BuildValue("()");                 |
| (...)    | 对应格式代码的参数列表             | 元组 tuple      | int num = 1; char *str = "a"; PyObject *tuple = Py_BuildValue("(is)", num, str); |
| []       | 无                                 | 空列表 list     | PyObject *empty_list = Py_BuildValue("[]");                  |
| [ ... ]  | 对应格式代码的参数列表             | 列表 list       | int num = 1; PyObject *list = Py_BuildValue("[i]", num);     |
| {}       | 无                                 | 空字典 dict     | PyObject *empty_dict = Py_BuildValue("{}");                  |
| { ... }  | 键值对对应的参数列表               | 字典 dict       | char *key = "key"; int value = 1; PyObject *dict = Py_BuildValue("{s:i}", key, value); |

### 3.3 阿里云垃圾分类模型

购买阿里云图像识别资源包（试用5000次免费）后续可采用自己训练模型

**安装图像识别SDK**

```
pip install alibabacloud_imagerecog20190930
```

**配置环境变量**

```bash
export ALIBABA_CLOUD_ACCESS_KEY_ID="<access_key_id> 
export ALIBABA_CLOUD_ACCESS_KEY_SECRET="<access_key_secret>
```

替换成自己的AccessKey即可  SDK 会自动从环境变量中读取访问密钥

export增加的是临时变量，仅在当前 shell 会话中有效。重启或者关闭shell 会丢失。

~/.bashrc 增加



**拷贝官方样例代码**

```python
# -*- coding: utf-8 -*-
# 引入依赖包
# pip install alibabacloud_imagerecog20190930

import os
import io
from urllib.request import urlopen
from alibabacloud_imagerecog20190930.client import Client
from alibabacloud_imagerecog20190930.models import ClassifyingRubbishAdvanceRequest
from alibabacloud_tea_openapi.models import Config
from alibabacloud_tea_util.models import RuntimeOptions

config = Config(
  # 创建AccessKey ID和AccessKey Secret，请参考https://help.aliyun.com/document_detail/175144.html。
  # 如果您用的是RAM用户的AccessKey，还需要为RAM用户授予权限AliyunVIAPIFullAccess，请参考https://help.aliyun.com/document_detail/145025.html
  # 从环境变量读取配置的AccessKey ID和AccessKey Secret。运行代码示例前必须先配置环境变量。
  access_key_id=os.environ.get('ALIBABA_CLOUD_ACCESS_KEY_ID'),
  access_key_secret=os.environ.get('ALIBABA_CLOUD_ACCESS_KEY_SECRET'),
  # 访问的域名
  endpoint='imagerecog.cn-shanghai.aliyuncs.com',
  # 访问的域名对应的region
  region_id='cn-shanghai'
)
#场景一：文件在本地
#img = open(r'/tmp/ClassifyingRubbish1.jpg', 'rb')
#场景二：使用任意可访问的url
url = 'http://viapi-test.oss-cn-shanghai.aliyuncs.com/viapi-3.0domepic/imagerecog/ClassifyingRubbish/ClassifyingRubbish1.jpg'
img = io.BytesIO(urlopen(url).read())
classifying_rubbish_request = ClassifyingRubbishAdvanceRequest()
classifying_rubbish_request.image_urlobject = img
runtime = RuntimeOptions()
try:
  # 初始化Client
  client = Client(config)
  response = client.classifying_rubbish_advance(classifying_rubbish_request, runtime)
  # 获取整体结果
  print(response.body)
except Exception as error:
  # 获取整体报错信息
  print(error)
  # 获取单个字段
  print(error.code)
```

测试

```bash
gcc -o garbagetest garbagetest.c garbage.h garbage.c -I /usr/include/python3.10 -l python3.10
```

### 3.4 USB摄像头配置

检查是否加载usb摄像头模块

```bash
lsmod

#显示 uvcvideo              102400  0
```

检查USB摄像头的设备节点信息是否为/dev/videoX

```
sudo apt install -y v4l-utils
u4l2-ctl --list-device
```

测试USB摄像头拍照

```bash
sudo apt-get install -y fswebcam

sudo fswebcam -d /dev/video1 --no-banner -r 128x720 -S 5 ./image.jpg
```

测试USB摄像头视频流

```bash
git clone https://gitee.com/leeboby/mjpg-streamer
sudo apt-get install -y cmake libjpeg8-dev

cd mjpg-streamer/mjpg-streamer-experimental
make -j4
sudo make install

#启动
#export LD_LIBRARY_PATH=.
#sudo ./mjpg_streamer -i "./input_uvc.so -d /dev/video0 -u -f 30"  -o "./output_http.so -w ./www"
根据上述命令修改start.sh文件
./start.sh
```

**获取照片**

```
wget http://192.168.136.52:8080/?action=snapshot -O /tmp/garbage.jpg
```

**开机自启动start.sh**

touch mjpg.sh

```
cd /home/orangepi/mjpg-streamer/mjpg-streamer-experimental
./start.sh
```

```
cd /etc/xdg/autostar
sudo cp im-launch.desktop mjpg.desktop

[Desktop Entry]
Name=mjpg
Exec= /home/orangepi/mjpg.sh
Type=Application
NoDisplay=true
```

重启终端检查是否设置成功

```
sudo reboot -f
ps ax | grep mjpg
```

### 3.5 语音模块配置

配置官网：https://www.smartpi.cn/#/

### 3.6VSCode远程连接

安装remote development插件

连接

```bash
ssh orangepi@192.168.136.52 
```

### 3.7 编译main.c

把串口改为不需要sudo的

```bash
sudo chmod 777 /dev/ttyS5
```

```
gcc -o test garbage.c garbage.h uartTool.c uartTool.h main.c -I /usr/include/python3.10 -lpython3.10
```

### 3.8 增加垃圾桶开关盖功能

```
gcc -o test pwm.c pwm.h garbage.c garbage.h uartTool.c uartTool.h main.c -I /usr/include/python3.10 -lpython3.10 -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
sudo -E ./test	
```

### 3.9 优化代码（线程）

将代码分装成三个线程：

- 香橙派读取语音模块数据线程。
  - 读取到语音模块发送的串口数据，发送线程信号给阿里云交互线程。
- 阿里云交互线程
  - 收到线程信号，执行拍照和垃圾识别
  - 并发执行三个线程：
    - 垃圾桶开盖线程
    - 语音播报类别线程
    - OLED显示线程
- 网络线程

```
gcc -o test pwm.c pwm.h garbage.c garbage.h uartTool.c uartTool.h main.c -I /usr/include/python3.10 -lpython3.10 -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
sudo -E ./test	
```

### 3.10 增加oled显示



## 补充

### 软件安装流程介绍

#### 1. `./configure`

`./configure` 是一个脚本，通常存在于开源软件包的根目录下。它的主要作用是检查当前系统的环境，确保软件可以在该系统上正确编译，并根据用户提供的选项进行配置。同时自动生成Makefile文件

当你运行 `./configure` 脚本时，它会执行一系列检查，包括：

- **编译器检查**：确认系统中是否安装了合适的编译器（如 GCC），并且版本是否兼容。
- **依赖库检查**：检查软件运行所需的依赖库是否存在，以及其版本是否满足要求。
- **系统特性检查**：检测系统的特性和功能，例如是否支持某些系统调用或库函数。

常见选项

- `--prefix=DIR`：指定软件的安装目录，默认情况下，软件会被安装到 ==`/usr/local`==。
- `--enable-optimizations`：开启编译优化，以提高软件的性能。
- `--with-ssl`：启用 SSL 支持，通常用于网络相关的软件。

示例

```bash
./configure --prefix=/usr/local/myapp --enable-optimizations
```

这个命令将软件安装到 `/usr/local/myapp` 目录，并开启编译优化。

#### 2. `make`

`make` 是一个自动化构建工具，它根据 `Makefile` 文件中的规则来编译软件。`Makefile` 是一个文本文件，其中定义了源文件、目标文件和编译规则之间的依赖关系。

当你运行 `make` 命令时，它会读取当前目录下的 `Makefile` 文件，并根据其中的规则来决定哪些文件需要重新编译。如果源文件发生了变化，`make` 会自动重新编译受影响的目标文件，从而提高编译效率。

常见选项

- `-jN`：并行执行 N 个编译任务，利用多核 CPU 的优势加快编译速度。例如，`make -j4` 表示同时执行 4 个编译任务。

示例

```bash
make -j4
```

这个命令会并行执行 4 个编译任务，加快编译过程。

#### 3. `make install`

`make install` 用于将编译好的软件安装到系统中。它会根据 `Makefile` 中的规则，将编译生成的可执行文件、库文件、配置文件等复制到指定的安装目录。

`make install` 通常需要管理员权限，因为它可能会将文件复制到系统的受保护目录（如 `/usr/local/bin`、`/usr/local/lib` 等）。在执行 `make install` 之前，`make` 命令会确保所有的目标文件都已经编译完成。

注意事项

- **权限问题**：由于 `make install` 可能会修改系统文件，因此通常需要使用 `sudo` 来获取管理员权限。
- **卸载问题**：使用 `make install` 安装的软件没有统一的卸载方法，一般需要手动删除安装的文件和目录。因此，在安装软件之前，最好记录下安装的路径。

示例

```bash
sudo make install
```

这个命令会以管理员权限执行 `make install`，将软件安装到系统中。

完整示例

假设你要编译和安装一个名为 `myapp` 的开源软件，完整的步骤如下：

```bash
# 进入软件包的根目录
cd myapp

# 配置编译选项
./configure --prefix=/usr/local/myapp

# 编译软件
make -j4

# 安装软件
sudo make install
```

### /usr 文件介绍

在 Linux 系统中，`/usr` 是一个非常重要的文件夹，它通常用于存储系统级的应用程序、库文件、文档等。以下是对 `/usr` 及其子文件夹 `local` 的详细介绍：

#### `/usr` 文件夹

- **含义**：`/usr` 是 “Unix Software Resource” 的缩写，该目录包含了几乎所有的系统应用程序、库文件、头文件、手册页和其他文档。它类似于 Windows 系统中的 `Program Files` 目录，但具有更丰富的内容和更明确的结构。

- 内容分类

  ：

  - **/usr/bin**：存储用户可执行的命令和应用程序。这些命令可以在系统的任何位置通过命令行调用，例如 `ls`、`cp`、`mv` 等常用命令都位于此目录。
  - **/usr/lib**：包含系统和应用程序所需的共享库文件。共享库是一些可被多个程序共享使用的代码和数据模块，能有效节省磁盘空间和内存，并提高程序的可维护性和可扩展性。
  - **/usr/include**：存储 C/C++ 等编程语言的头文件。头文件包含了函数、变量、数据结构等的声明，在编译程序时，编译器需要包含这些头文件来获取相关的声明信息。
  - **/usr/share**：存放各种共享数据，如帮助文件、字体文件、图标文件、语言包等。这些数据可以被多个应用程序共享使用。
  - **/usr/sbin**：主要存储系统管理命令和守护进程。这些命令通常需要管理员权限才能执行，用于系统的管理和维护，如 `ifconfig`、`route` 等网络管理命令，以及 `httpd`、`sshd` 等守护进程的启动脚本。

#### `/usr/local` 文件夹

- **含义**：`/usr/local` 是用于存放本地安装的软件和文件的目录。与 `/usr` 目录不同，`/usr/local` 主要用于存放用户自行安装的、非系统默认的软件和相关文件。
- 内容分类：
  - **/usr/local/bin**：用户自行安装的可执行程序通常会被放置在此目录。这些程序可以是从源代码编译安装的软件，也可以是从第三方获取的可执行文件。例如，用户安装的一些开发工具、特定的命令行工具等。
  - **/usr/local/lib**：存放用户自行安装的软件所使用的共享库文件。当用户安装一些需要额外库支持的软件时，这些库文件会被安装到这个目录。
  - **/usr/local/include**：如果用户安装的软件包含头文件，那么这些头文件会被放置在 `/usr/local/include` 目录下。例如，当安装一些开源的开发库时，其头文件会被安装到此目录，以便在编译使用该库的程序时能够找到相应的头文件。
  - **/usr/local/share**：用于存储用户自行安装软件的共享数据，如文档、配置文件、帮助文件等。不同软件的共享数据会在该目录下有各自的子目录进行存放。

`/usr` 文件夹是系统级软件和资源的主要存放位置，而 `/usr/local` 则为用户提供了一个独立的空间，用于安装和管理自己的软件，不会与系统默认的软件和文件产生冲突。这样的目录结构有助于保持系统的稳定性和可维护性，同时也方便用户进行个性化的软件安装和配置。

那etc 和dev 是什么文件夹，

在 Linux 系统中，`/etc` 和 `/dev` 都是非常重要的系统文件夹，以下是它们的详细介绍：

#### `/etc` 文件夹

- **含义**：`/etc` 目录是 “etcetera” 的缩写，通常用于存放系统和应用程序的配置文件。这些配置文件包含了各种参数和设置，用于控制系统和软件的行为。
- 内容分类：
  - **系统配置文件**：例如 `fstab` 文件，用于定义文件系统的挂载信息，指定开机时自动挂载的分区和文件系统；`hosts` 文件用于配置主机名和 IP 地址的映射关系；`network` 或 `ifcfg-*` 等相关文件用于网络配置，如设置网络接口的 IP 地址、子网掩码、网关等信息。
  - **服务配置文件**：许多系统服务和应用程序都有各自在 `/etc` 下的配置文件。比如，`/etc/httpd/` 目录下存放着 Apache Web 服务器的配置文件，用于设置网站的根目录、访问权限、虚拟主机等信息；`/etc/ssh/` 目录下的配置文件用于配置 SSH 服务，如允许哪些用户登录、使用的认证方式等。
  - **用户和组配置文件**：`/etc/passwd` 文件记录了系统中的用户账户信息，包括用户名、用户 ID、组 ID、用户家目录等；`/etc/group` 文件则用于定义用户组的信息。

#### `/dev` 文件夹

- **含义**：`/dev` 是 “device” 的缩写，该目录包含了系统中所有设备的设备文件。这些设备文件提供了一种统一的方式来访问和控制硬件设备，用户和应用程序可以通过读写这些设备文件来与硬件设备进行交互。
- 内容分类：
  - **块设备文件**：如硬盘、分区、光盘等存储设备通常以块为单位进行数据传输和访问，它们在 `/dev` 目录下有对应的设备文件。例如，`/dev/sda` 通常表示第一个 SCSI 硬盘，`/dev/sda1` 表示该硬盘的第一个分区；`/dev/cdrom` 通常是光盘设备的符号链接，实际指向具体的光盘设备文件。
  - **字符设备文件**：字符设备是以字符为单位进行数据传输的设备，如串口、并口、键盘、鼠标等。例如，`/dev/ttyS0` 表示第一个串口设备，`/dev/input/mouse0` 表示第一个鼠标设备。
  - **特殊设备文件**：还有一些特殊的设备文件，如 `/dev/null`，它被称为空设备，任何写入到该设备的数据都会被丢弃，常用于丢弃不需要的输出或作为某些命令的输入黑洞；`/dev/zero` 设备则会不断地提供空字符（`\0`），常用于创建指定大小的空文件或填充内存区域。

`/etc` 文件夹主要用于存储系统和应用程序的配置信息，而 `/dev` 文件夹则提供了对硬件设备的抽象和访问接口，它们在 Linux 系统的运行和管理中都起着至关重要的作用。

### 软链接

软链接（符号链接）是一种特殊类型的文件，它包含了另一个文件或目录的路径名。软链接就像一个快捷方式，当你访问软链接时，系统会自动将访问重定向到它所指向的目标文件或目录。

**与硬链接的区别**

- **硬链接**：硬链接是文件的另一个名称，多个硬链接指向同一个文件的物理数据块。硬链接和原始文件没有本质区别，删除其中一个硬链接不会影响其他硬链接和文件数据。硬链接只能指向同一个文件系统内的文件，且不能对目录创建硬链接。
- **软链接**：软链接是一个独立的文件，它只包含目标文件的路径信息。删除软链接不会影响目标文件，而删除目标文件会使软链接变成无效链接（也称为断链）。软链接可以跨文件系统，并且可以指向目录。

**创建和删除软链接**

- **创建软链接**：可以使用 `ln -s` 命令来创建软链接。例如，要创建一个指向 `/path/to/target` 文件的软链接 `linkname`，可以执行以下命令：

```bash
ln -s /path/to/target linkname
```

- **删除软链接**：使用 `rm` 命令删除软链接，例如：

```bash
rm linkname
```

这里只需指定软链接的名称，而不是目标文件的名称。

**使用场景**

- **兼容性**：在系统中，为了保持向后兼容性，可能会存在多个版本的软件。通过创建软链接，可以让不同的程序使用统一的名称来访问不同版本的软件。例如，`python` 软链接指向 `python2.7` 或 `python3`，方便用户使用。
- **文件组织**：当需要在不同的目录中访问同一个文件时，可以创建软链接，避免复制文件带来的空间浪费。

**python文件两种执行**

1. 直接运行

```bash
python test.py
```

2. py文件中首行加入解释器路径

```python
#!/usr/bin/python
```

```bash
./test.py
```

### 密钥配置到环境变量

**为什么将密钥配置到环境变量里？**

将访问密钥以环境变量的形式存储，避免了将敏感信息硬编码在代码中。如果密钥被硬编码在代码里，一旦代码泄露，密钥也会随之暴露，可能会造成严重的安全风险。而使用环境变量，密钥信息存储在系统层面，代码中只需要从环境变量中读取密钥，这样可以降低密钥泄露的风险。

通过环境变量配置密钥，方便在不同的环境（如开发环境、测试环境、生产环境）中切换不同的密钥。你只需要在不同环境中设置不同的环境变量值，而不需要修改代码。

许多阿里云的 SDK（软件开发工具包）都支持从环境变量中读取访问密钥，这样编写的代码可以在不同的系统和环境中复用，无需每次都手动传递密钥参数。

### .bashrc

`.bashrc` 是 Linux 系统中用户主目录下（`~/.bashrc` ）的一个配置文件 ，用于定制 `bash` shell 环境，作用如下：

- **设置环境变量**：在 `.bashrc` 中定义环境变量，每次打开终端时会自动加载，比如可添加自定义路径到 `PATH` 变量，方便系统查找可执行文件 ；或设置 `JAVA_HOME` 等变量。
- **自定义别名**：能把长且复杂的命令设置成简短易记的别名，如常见的 `alias ll='ls -lha'` ，使用 `ll` 就能执行 `ls -lha` 命令，提高输入效率。
- **添加自定义函数**：可编写自定义函数实现特定功能，比如定义一个函数用于快速切换到常用工作目录。
- **修改提示符**：对终端提示符的样式、颜色等进行个性化设置，让终端显示更符合个人习惯 。

### Linux下pip安装目录

在 Linux 系统中，pip 安装的包默认存放在 `~/.local/lib` 目录下，而 Python 解释器位于 `/usr/bin`：

- **用户权限与隔离**：`~/.local/lib` 是用户主目录下的一个子目录，用于存放用户特定的本地软件包和数据。使用这个目录可以避免用户在没有管理员权限时对系统级别的 Python 安装目录（如 `/usr/lib/python`）进行写入操作，防止因权限不足导致安装失败，同时也实现了不同用户之间的软件包隔离，每个用户的软件包都安装在各自的 `~/.local/lib` 目录下，互不干扰。
- **系统规范与兼容性**：`/usr/bin` 是系统级别的可执行文件目录，通常用于存放系统范围内可共享的命令和工具。Python 解释器被安装在 `/usr/bin` 目录下，符合 Linux 系统的文件系统层次结构标准（FHS），使得 Python 可以作为系统级的工具被全局调用。这样，系统中的所有用户都可以在任何位置通过命令行输入 `python` 或 `python3` 来启动 Python 解释器，而不需要指定完整的路径。同时，将 Python 解释器放在系统级目录中，也便于系统进行统一的管理和维护，例如在系统更新时可以方便地对 Python 进行升级或修复。
- **虚拟环境的影响**：在虚拟环境中，情况会有所不同。当创建一个虚拟环境时，会在虚拟环境的目录下创建一个独立的 Python 解释器和相关的库目录。此时，使用 `pip` 安装的包会被安装在虚拟环境的 `lib` 目录下，而不是 `~/.local/lib` 目录。这是为了实现虚拟环境的隔离性，使得每个虚拟环境都有自己独立的 Python 运行环境和软件包依赖，避免不同项目之间的依赖冲突。

### 条件变量

```c
void *func1()
{
    pthread_mutex_lock(&mutex);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mutex); 
}
void *func2()
{
    pthread_mutex_lock(&mutex);
	pthread_cond_wait(&cond,&mutex);
	pthread_mutex_unlock(&mutex);
}
```

如果不加mutex互斥量，有可能func2正准备调用 pthread_cond_wait 进入等c待状态。

此时，func1调用 pthread_cond_signal 发送信号。

但是func2在收到信号之前就已经进入了 pthread_cond_wait 状态，这样就会错过这个信号，导致fun2一直等待，出现死锁的情况。

### 为什么不互斥量和条件变量初始化也能用？

在 POSIX 标准中，`pthread_cond_t` 和 `pthread_mutex_t` 类型的变量有两种初始化方式：**显式初始化**和**隐式（零值）初始化**。你观察到的 “不初始化也能用” 的现象，实际上是因为 POSIX 允许 **静态存储期的变量通过零值隐式初始化**，但这有严格的适用条件，并非 “不初始化”，而是利用了语言特性和库规范的结合。

**一、为什么 “不初始化” 也能用？**

1. **静态存储期变量的零值初始化**

- 如果 `pthread_cond_t cond;` 和 `pthread_mutex_t mutex;` 是在 **全局作用域**或 **静态局部作用域**（使用 `static` 修饰）定义的，C 语言会将其初始化为 **零值**（所有字节为 0）。
- POSIX 标准明确规定：
  - 当 `pthread_mutex_t` 的值为零值时，等同于调用 `pthread_mutex_init(&mutex, NULL)`（默认属性初始化）。
  - 当 `pthread_cond_t` 的值为零值时，等同于调用 `pthread_cond_init(&cond, NULL)`（默认属性初始化）。
    这就是 “隐式初始化” 的依据。

2. **示例场景**

```c
pthread_cond_t cond;       // 全局变量，自动零初始化
pthread_mutex_t mutex;     // 全局变量，自动零初始化

int main() {
    pthread_mutex_lock(&mutex); // 直接使用，等价于已默认初始化
    // ...
    return 0;
}
```

这里的 `cond` 和 `mutex` 作为全局变量，被编译器自动填充为零值，POSIX 库保证其等同于默认初始化的有效状态。

**二、如果是局部变量（自动存储期）呢？**

如果变量在函数内部定义（局部变量），且未显式初始化：

```c
void func() {
    pthread_cond_t cond;       // 局部变量，未初始化（值不确定）
    pthread_mutex_t mutex;     // 局部变量，未初始化（值不确定）
    pthread_mutex_lock(&mutex); // 未定义行为！
}
```

- **此时行为未定义**：局部变量的初始值是随机的（垃圾值），直接使用会导致崩溃或不可预测的错误。

- 必须显式初始化：

  ```c
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // 静态初始化
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; // 静态初始化
  
  // 或动态初始化（适用于局部变量或运行时需要设置属性的场景）：
  pthread_cond_init(&cond, NULL);
  pthread_mutex_init(&mutex, NULL);
  ```

**四、总结：“能用” 的前提和风险**

| 场景                 | 隐式零值初始化是否允许？ | 风险 / 注意事项                                              |
| -------------------- | ------------------------ | ------------------------------------------------------------ |
| 全局 / 静态变量      | 允许（POSIX 保证）       | 仅适用于默认属性，若需自定义属性仍需显式初始化               |
| 局部变量（未初始化） | 不允许（未定义行为）     | 必须显式初始化（`PTHREAD_XXX_INITIALIZER` 或 `pthread_xxx_init`） |
| 动态分配（`malloc`） | 不允许                   | 必须调用 `pthread_xxx_init` 初始化                           |

**最佳实践：**

1. 显式初始化：无论变量作用域如何，显式使用

   ```
   PTHREAD_XXX_INITIALIZER
   ```

   或

   ```
   pthread_xxx_init
   ```

   更安全，避免依赖 “零值巧合”。

   ```c
   pthread_cond_t cond = PTHREAD_COND_INITIALIZER; // 推荐静态初始化
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
   ```

2. 动态初始化：对局部变量或需要自定义属性（如递归锁、非阻塞锁）时，必须调用初始化函数：

   ```c
   pthread_cond_init(&cond, NULL); // 默认属性
   pthread_mutex_init(&mutex, &attr); // 可指定属性
   ```

3. **清理资源**：动态初始化的变量需调用 `pthread_xxx_destroy` 释放资源（静态初始化的全局变量无需手动销毁，程序结束时自动释放）。

理解这一机制的关键是：POSIX 库利用了 C 语言的零值初始化特性，对静态存储期的变量提供了便利，但这是有严格适用范围的，并非 “不初始化也能用”，而是 “零值初始化被库规范定义为有效”。对于局部变量或动态分配的变量，必须显式初始化，否则会导致严重错误。
