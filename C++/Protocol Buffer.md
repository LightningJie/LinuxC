# Protobuf

## 1. Protobuf 概述

protobuf 也叫 protocol buffer 是 google 的一种`数据交换的格式` ，它`独立于语言，独立于平台` 。google 提供了多种语言的实现：java、c#、`c++` 、go 和 python 等，每一种实现都包含了相应语言的编译器以及库文件。

由于它是一种 `二进制的格式` ，比使用 xml 、json 进行数据交换快许多。可以把它用于分布式应用之间的数据通信或者异构环境下的数据交换。作为一种效率和兼容性都很优秀的二进制数据传输格式，可以用于诸如网络传输、配置文件、数据存储等诸多领域。

### 1.1 源码安装

github 源代码下载地址:https://github.com/protocolbuffers/protobuf/releases

本篇文章中只介绍如何基于 `Linux` 完成 `protbuf` 的源码安装，基于提供的官方地址大家可以把自己的心仪的源码包下载到本地。接下来介绍安装步骤：

温馨提示：以下按照步骤只适用于 3.21.12 及以下版本，高于该版本的安装方式就不一样了。

```shell
# 以 protobuf 3.21.12 为例
# 自行下载源码包, 解压缩
$ tar zxvf protobuf-cpp-3.21.12.tar.gz 
# 进入到解压目录
$ cd protobuf-3.21.12/
# 构建并安装
$ ./configure         # 检查安装环境, 生成 makefile
$ make                # 编译
$ sudo make install   # 安装
```

### 1.2 测试

```shell
$ protoc --version
protoc: error while loading shared libraries: libprotoc.so.32: cannot open shared object file: No such file or directory
```

**执行逻辑**：`protoc`是可执行文件，但运行时需加载`libprotoc.so`中的函数（如版本检查逻辑）。若库文件不在默认搜索路径（如`/usr/local/lib`未注册），系统会报错。

Linux系统在运行时加载共享库（如`.so`文件）时，**默认搜索路径为`/lib`和`/usr/lib`**



通过输出的信息可以看到找不到动态库，此时可以通过 `find` 进行搜索

```shell
$ sudo find /usr/local/ -name libprotoc.so     
/usr/local/lib/libprotoc.so
```

此时存储动态库的路径就找到了。
使用 find 命令进行搜索的时候，如果从 /usr/local/ 目录中搜索不到需要的文件信息，可以从根目录 / 开始搜索。

其实在通过 `make install` 安装的时候通过终端输出的日志信息也可以找到动态库对应的存储路径：

```shell
......................................................................
libtool: install: (cd /usr/local/lib && { ln -s -f libprotoc.so.32.0.12 libprotoc.so.32 || { rm -f libprotoc.so.32 && ln -s libprotoc.so.32.0.12 libprotoc.so.32; }; })
libtool: install: (cd /usr/local/lib && { ln -s -f libprotoc.so.32.0.12 libprotoc.so || { rm -f libprotoc.so && ln -s libprotoc.so.32.0.12 libprotoc.so; }; })
......................................................................
```

以上是从安装日志中找到的对我们有用的信息，和搜索的结果一样，可以看到这个动态库位于`/usr/local/lib` 目录，接下来需要需要将这目录添加到`/etc/ld.so.conf` 这个配置文件中：

```shell
$ sudo vim /etc/ld.so.conf
```

打开文件后把目录添加到第二行，保存退出。

最后，需要更新配置，在终端执行如下命令：

```shell
$ sudo ldconfig
```

配置完成之后， 再次进行测试：

```shell
$ protoc --version
libprotoc 3.21.12
```

现在就可以看到安装的 `protobuf` 的版本号了。

### 1.3 如何使用

在学习 protbuf 之前，我们需要搞清楚使用 protbuf 进行数据的序列化主要有哪几个步骤：

确定数据格式，数据可简单可复杂，比如：

```c++
// 要序列化的数据
// 第一种: 单一数据类型
int number;
// 第二种: 复合数据类型
struct Person{
    int id;
    string name;
    string sex;	
    int age;
};
```

1. 创建一个新的文件，文件名随意指定，文件后缀为`.proto`

2. 根据 `protobuf` 的语法，编辑 `.proto` 文件

3. 使用`protoc` 命令将`proto`文件转化为相应的`C++ 文件`

   - 源文件: `xxx.pb.cc` –> xxx 对应的名字和 .proto 文件名相同
   - 头文件: `xxx.pb.h` –> xxx 对应的名字和 .proto 文件名相同
   
4. 需要将生成的 c++ 文件添加到项目中，通过文件中提供的类 API 实现数据的序列化 / 反序列化

protobuf 中的数据类型 和 C++ 数据类型对照表:

| Protobuf 类型 | C++ 类型              | 备注                                                         |
| ------------- | --------------------- | ------------------------------------------------------------ |
| `double`      | `double`              | 64 位浮点数                                                  |
| `float`       | `float`               | 32 位浮点数                                                  |
| `int32`       | `int`                 | 32 位整数                                                    |
| `int64`       | `long`                | 64 位整数                                                    |
| `uint32`      | `unsigned int`        | 32 位无符号整数                                              |
| `uint64`      | `unsigned long`       | 64 位无符号整数                                              |
| `sint32`      | `signed int`          | 32 位整数，处理负数效率比 int32 更高                         |
| `sint64`      | `signed long`         | 64 位整数，处理负数效率比 int64 更高                         |
| fixed32       | unsigned int (32 位)  | 总是 4 个字节。如果数值总是比总是比 228 大的话，这个类型会比 uint32 高效。 |
| fixed64       | unsigned long (64 位) | 总是 8 个字节。如果数值总是比总是比 256 大的话，这个类型会比 uint64 高效。 |
| sfixed32      | int (32 位)           | 总是 4 个字节                                                |
| sfixed64      | long (64 位)          | 总是 8 个字节                                                |
| `bool`        | `bool`                | 布尔类型                                                     |
| string        | string                | 一个字符串必须是 UTF-8 编码或者 7-bit ASCII 编码的文本       |
| `bytes`       | `string`              | 处理多字节的语言字符、如中文，建议 protobuf 中字符型类型使用 `bytes` |
| `enum`        | `enum`                | 枚举                                                         |
| `message`     | `object of class`     | 自定义的消息类型                                             |

## 2. Protobuf 语法

### 2.1 基本使用

总体流程：

```
编写.proto -> 生成.cc .h 文件-> 链接protobuf动态库（libprotobuf.so）
```



假设我定义了这样一个结构体，现在要基于这个结构体完成数据的序列化，结构体原型如下：

```c++
struct Person{
    int id;
    string name;
    string sex;	// man woman
    int age;
};
```

先编写一个CMakeList.txt

```cmake
cmake_minimum_required(VERSION 3.22)
project(test_protobuf)
set(CMAKE_CXX_STANDARD 17)

aux_source_directory(./ SRC)

add_library(test_protobuf ${SRC} )
target_link_libraries(test_protobuf protobuf)
```

接下来，我们需要新建一个文件，给它起个名字，后缀指定为 `.proto` ，在文件的第一行需要指定 Protobuf 的版本号，有两个版本 Protobuf 2 和 Protobuf 3，此处我们使用的是版本 3。

```protobuf
syntax="proto3";
```

接着需要定义一个消息体，其格式如下：

```protobuf
message 名字	{
    // 类中的成员, 格式    数据类型 成员名字 = 1;
    数据类型 成员名字 = 2;
    数据类型 成员名字 = 3;	
    ......    	
    ......
}
```

`message后面的名字` 就是生成的类的名字，自己指定一个合适的名字即可
`等号后面的编号` 要 `从1开始` ，每个成员都有一个唯一的编号，不能重复，一般连续编号即可。

基于上面的语法，上面结构体对应的 `.proto` 文件的内容可以写成这样：

```protobuf
// Person.proto
syntax = "proto3";
// 在该文件中对要序列化的结构体进行描述
message Person{
    int32 id = 1;
    bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
}
```

`.proto` 文件编辑好之后就可以使用 `protoc` 工具将其转换为 C++ 文件了。

```shell
$ protoc -I path .proto文件 --cpp_out=输出路径(存储生成的c++文件)
```

在 `protoc` 命令中， `-I` 参数后面可以跟随一个或多个路径，用于告诉编译器在哪些路径下查找导入的文件或依赖的文件，使用绝对路径或相对路径都是没问题的。

如果有多个路径，可以使用多个 `-I` 参数或在一个参数中使用冒号（`:` ）分隔不同的路径。如果只有一个路径`-I` 参数可以省略。

例如， `protoc -I path1 -I path2` 或 `protoc -I path1:path2` 都表示告诉编译器在 `path1` 和 `path2` 路径下查找导入的文件或依赖的文件。

我们可以在 `.proto` 文件所在目录执行 `protoc` 命令，并生成到当前目录：

```shell
$ protoc ./Person.proto --cpp_out=.
# 或者使用 -I 参数
$ protoc -I ./ Person.proto --cpp_out=.
```

### 2.2 repeated 限定修饰符

在使用 Protocol Buffers（Protobuf）中，可以使用`repeated` 关键字作为限定修饰符来表示一个字段可以有多个值，即重复出现的字段。 `repeated` 关键字可以用于以下数据类型：`基本数据类型` 、 `枚举类型` 和`自定义消息类型` 。

比如要序列化的数据中有一个数组，结构如下：

```c
// 要序列化的数据
struct Person{
    int id;
    string name[10];
    string sex;	
    int age;
};
```

Protobuf 的消息体就可以写成下面的样子:

```protobuf
message Person{
    int32 id = 1;
    repeated bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
}
```

使用 `repeated` 关键字定义的字段在 `Protobuf` 序列化和反序列化时会被当作一个集合或数组来处理。这个`name` 可以作为一个动态数组来使用。

### 2.3 枚举

在 Protocol Buffers 中，枚举类型用于定义一组特定的取值。下面定义一个枚举，并将其添加到 `Person` 中：

```c
// 要序列化的数据
// 枚举
enum Color{
    Red = 5,	// 可以不给初始值, 默认为0
    Green,
    Yellow,
    Blue
};
// 要序列化的数据
struct Person{
    int id;
    string name[10];
    string sex;	
    int age;
    // 枚举类型
    Color color;
};
```

以下是如何在 Protobuf 中定义和使用枚举类型，其语法如下：

```plaintext
enum 名字{
    元素名 = 0;	// 枚举中第一个原素的值必须为0
    元素名 = 数值;
}
```

枚举元素之间使用分号间隔 `;` ，并且需要注意一点`proto3 中的第一个枚举值必须为 0，第一个元素以外的元素值可以随意指定。`

上面例子中的数据在 `.proto` 文件中可以写成如下格式:

```protobuf
// 定义枚举类型
enum Color{
    Red = 0;
    Green = 3;		// 第一个元素以外的元素值可以随意指定
    Yellow = 6;
    Blue = 9;
}
// 在该文件中对要序列化的结构体进行描述
message Person{
    int32 id = 1;
    repeated bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
    // 枚举类型
    Color color = 5;
}
```

### 2.4 proto 文件的导入

在 Protocol Buffers 中，可以使用 `import` 语句在当前`.ptoto` 中导入其它的 `.proto` 文件。这样就可以在一个`.proto` 文件中引用并使用其它文件中定义的消息类型和枚举类型。

语法格式如下:

```protobuf
import "要使用的proto文件的名字";
```

假设现在我有一个 `proto` 文件 `Address.proto` ，里边记录了地址信息：

```protobuf
syntax = "proto3";
// 地址信息
message Address{
    bytes addr = 1;
    bytes number = 2;
}
```

我现需要再上面定义的 `Person.proto` 中添加这个人的地址信息，因此就需要用到 `Address.proto` 中定义的消息体：

```protobuf
syntax = "proto3";
// 使用另外一个proto文件中的数类型, 需要导入这个文件
import "Address.proto";
// 在该文件中对要序列化的结构体进行描述
// 定义枚举类型
enum Color{
    Red = 0;
    Green = 3;		// 第一个元素以外的元素值可以随意指定
    Yellow = 6;
    Blue = 9;
}
// 在该文件中对要序列化的结构体进行描述
message Person{
    int32 id = 1;
    repeated bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
    // 枚举类型
    Color color = 5;
    // 添加地址信息, 使用的是外部proto文件中定义的数据类型
    Address addr = 6;
}
```

`import` 语句中指定的文件路径可以是相对路径或绝对路径。如果文件在相同的目录中，只需指定文件名即可。

- 导入的文件将会在编译时与当前文件一起被编译。
- 导入的文件也可以继续导入其他文件，形成一个文件依赖的层次结构。

### 2.5 包 (package)

在 Protobuf 中，可以使用 `package` 关键字来定义一个消息所属的包（package）。包是用于组织和命名消息类型的一种机制，类似于命名空间的概念。

在一个 `.proto` 文件中，可以通过在顶层使用 `package` 关键字来定义包：

```protobuf
syntax = "proto3";
package mypackage;
message MyMessage {
  // ...
}
```

在这个示例中，我们使用 `package` 关键字将 `MyMessage` 消息类型定义在名为 `mypackage` 的包中。包名作为一个标识符来命名，可以使用任何有效的标识符，按惯例使用小写字母和下划线。

使用包可以避免不同 `.proto` 文件中的消息类型名称冲突，同时也可以更好地组织和管理大型项目中的消息定义。可以将消息类型的名称定义在特定的包中，并使用限定名来引用这些类型。

下面有两个 `proto` 文件，分别给他们添加一个 `package` ：

#### proto 文件 - `Address.proto`

protobuf











```protobuf
syntax = "proto3";
// 添加命名空间 Dabing
package Dabing;
// 地址信息, 这个Address类属于命名空间: Dabing
message Address{
    bytes addr = 1;
    bytes number = 2;
}
```

#### proto 文件 - `Person.proto`



```protobuf
syntax = "proto3";
// 使用另外一个proto文件中的数类型, 需要导入这个文件
import "Address.proto";
// 指定命名空间 ErBing
package ErBing;
// 以下的类 Person 和枚举 Color 都属于命名空间 ErBing
// 在该文件中对要序列化的结构体进行描述
// 定义枚举类型
enum Color{
    Red = 0;
    Green = 3;		// 第一个元素以外的元素值可以随意指定
    Yellow = 6;
    Blue = 9;
}
// 在该文件中对要序列化的结构体进行描述
message Person{
    int32 id = 1;
    repeated bytes name = 2;
    bytes sex = 3;	
    int32 age = 4;
    // 枚举类型
    Color color = 5;
    // 添加地址信息, 使用的是外部proto文件中定义的数据类型
    // 如果这个外边类型属于某个命名空间, 语法格式:
    // 命名空间的名字.类名 变量名=编号;
    Dabing.Address addr = 6;
}
```

## 3. 序列化和反序列化

### 3.1 .pb.h 头文件

通过 `protoc` 命令对 `.proto` 文件的转换，得到的头文件中有一个类，这个类的名字和 `.proto` 文件中`message关键字后边指定的名字相同` ， `.proto` 文件中 `message` 消息体的成员就是生成的类的私有成员。

那么如何访问生成的类的私有成员呢？ 可以调用生成的类提供的公共成员函数，这些函数有如下规律：

- 清空 (初始化) 私有成员的值: `clear_变量名()`
- 获取类私有成员的值: `变量名()`
- 给私有成员进行值的设置: `set_变量名(参数)`
- 得到类私有成员的 `地址` , 通过这块地址 `读/写当前私有成员变量的值` : `mutable_变量名()`

如果这个变量是数组类型:

- 数组中元素的个数: `变量名_size()`
- 添加一块内存，存储新的元素数据: `add_变量名() 、add_变量名(参数)`

### 3.2 序列化

序列化是指将数据结构或对象转换为可以在储存或传输中使用的二进制格式的过程。在计算机科学中，序列化通常用于将内存中的对象持久化存储到磁盘上，或者在分布式系统中进行数据传输和通信。

Protobuf 中为我们提供了相关的用于数据序列化的 API，如下所示：

```c++
// 头文件目录: google\protobuf\message_lite.h
// --- 将序列化的数据 数据保存到内存中
// 将类对象中的数据序列化为字符串, c++ 风格的字符串, 参数是一个传出参数
bool SerializeToString(std::string* output) const;
// 将类对象中的数据序列化为字符串, c 风格的字符串, 参数 data 是一个传出参数
bool SerializeToArray(void* data, int size) const;
// ------ 写磁盘文件, 只需要调用这个函数, 数据自动被写入到磁盘文件中
// -- 需要提供流对象/文件描述符关联一个磁盘文件
// 将数据序列化写入到磁盘文件中, c++ 风格
// ostream 子类 ofstream -> 写文件
bool SerializeToOstream(std::ostream* output) const;
// 将数据序列化写入到磁盘文件中, c 风格
bool SerializeToFileDescriptor(int file_descriptor) const;
```

### 3.2 反序列化

反序列化是指将序列化后的二进制数据重新转换为原始的数据结构或对象的过程。通过反序列化，我们可以将之前序列化的数据重新还原为其原始的形式，以便进行数据的读取、操作和处理。

Protobuf 中为我们提供了相关的用于数据序列化的 API，如下所示：

```c++
// 头文件目录: google\protobuf\message_lite.h
bool ParseFromString(const std::string& data) ;
bool ParseFromArray(const void* data, int size);
// istream -> 子类 ifstream -> 读操作
// wo ri
// w->写 o: ofstream , r->读 i: ifstream
bool ParseFromIstream(std::istream* input);
bool ParseFromFileDescriptor(int file_descriptor);
```

## 4. Protoc 外部工具扩展

### 4.1 QtCreator

参数具体内容如下:

```shell
-I %{CurrentDocument:Path} --cpp_out=%{CurrentDocument:Path} %{CurrentDocument:FilePath}
```

`CurrentDocument:Path` - IDE 中打开的当前文件的路径 （绝对路径）
`CurrentDocument:FilePath` - IDE 中打开的当前文件的名字（绝对路径）

### 4.2 CLion

关于参数按照如下进行指定即可:

```shell
-I $FileDir$ $FileName$ --cpp_out=$FileDir$
```

`FileDir` - IDE 中打开的当前文件的路径 （绝对路径）
`FileName` - IDE 中打开的当前文件的名字（绝对路径）

## 5. 示例程序

### 5.1 .proto 文件

#### Address.proto

```protobuf
syntax = "proto3";
package Dabing;
message Address{
  int32 num = 1;
  bytes addr = 2;
}
```

#### Person.proto

```protobuf
syntax = "proto3";
import "Address.proto";
package Erbing;
enum Color{
    Red = 0;
    Green = 5;
    Yellow = 6;
    Blue = 9;
}
message Person{
    int32 id = 1;    
    repeated bytes name = 2;
    bytes sex = 3;
    int32 age = 4;
    Dabing.Address addr = 5;
    Color color = 6;
}
```

### 5.2 测试文件

#### MyTest.h

```c++
#ifndef SERVER_DDZ_MYTEST_H
#define SERVER_DDZ_MYTEST_H

class MyTest{
public:
    void test();
};

#endif //SERVER_DDZ_MYTEST_H
```

#### MyTest.cpp

```c++
#include "MyTest.h"
#include "Person.pb.h"
//using namespace Dabing;
//using namespace Erbing;

void MyTest::test(){
    // 序列化
    Erbing::Person p;
    p.set_id(10);
    p.set_age(32);
    p.set_sex("man");
    p.add_name();
    p.set_name(0,"路飞");
    p.add_name("艾斯");
    p.add_name("萨博");
    p.mutable_addr()->set_addr("北京市长安区天安门");
    p.mutable_addr()->set_num(1001);
    p.set_color(Erbing::Color::Blue);

    // 序列化对象 p, 最终得到一个字符串
    std::string output;
    p.SerializeToString(&output);

    // 反序列化数据
    Erbing::Person pp;
    pp.ParseFromString(output);

    std::cout << pp.id()  << ", " << pp.sex() << ", " << pp.age() << std::endl;
    std::cout << pp.addr().addr() << ", " << pp.addr().num() << std::endl;
    int size = pp.name_size();
    for(int i=0; i<size; ++i)
    {
        std::cout << pp.name(i) << std::endl;
    }
    std::cout << pp.color() << std::endl;
}
```

#### main.cpp

```c++
#include "MyTest.h"

int main(int argc, char* argv[])
{
    MyTest t;
    t.test();
    return 0;
}
```

