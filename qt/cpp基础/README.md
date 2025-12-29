## cpp基础

### 1.1 命名空间

#### 1.1.1 命名空间的定义

```c++
namespace 名称 {
    // 声明和定义内容
}
```

```c++
namespace MyNamespace {
    int value = 10;
    void print() {
        std::cout << "Value: " << value << std::endl;
    }
}
```

#### 1.1.2 命名空间的使用

- **直接使用命名空间中的成员**：通过`命名空间::成员`的方式访问命名空间中的内容。例如：

  ```c++
  MyNamespace::print();
  ```

- **使用`using`声明**：可以将命名空间中的某个成员引入到当前作用域中，这样可以直接使用该成员而不需要加命名空间前缀。语法如下：

  ```cpp
  using MyNamespace::print;
  print(); // 直接调用
  ```

- **使用`using`指令**：可以将整个命名空间的内容引入到当前作用域中。语法如下：

  ```cpp
  using namespace MyNamespace;
  print(); // 直接调用
  ```

  **注意**：使用`using namespace`时要谨慎，尤其是在大型项目中，因为它可能会导致命名冲突。

#### 1.1.3 std

`std`（标准）命名空间是C++标准库的核心组成部分，它包含了C++标准库中定义的所有类、函数、对象和模板等。这些内容被组织在`std`命名空间中，以避免与其他库或用户定义的标识符发生冲突。以下是`std`命名空间中包含的主要内容的概述：

**1. 输入输出流**

`std`命名空间提供了用于输入输出操作的流对象和相关操作符，这些功能定义在`<iostream>`、`<fstream>`和`<sstream>`等头文件中：

- **输入输出流对象**：
  - `std::cin`：标准输入流，用于从控制台读取输入。
  - `std::cout`：标准输出流，用于向控制台输出内容。
  - `std::cerr`：标准错误输出流，用于输出错误信息。
  - `std::clog`：标准日志输出流，用于输出日志信息。
- **文件流**：
  - `std::ifstream`：用于从文件读取数据。
  - `std::ofstream`：用于向文件写入数据。
  - `std::fstream`：用于同时支持文件读写操作。
- **字符串流**：
  - `std::istringstream`：用于从字符串读取数据。
  - `std::ostringstream`：用于向字符串写入数据。
  - `std::stringstream`：用于同时支持字符串的读写操作。
- **格式化操作符**：
  - `std::endl`：用于换行并刷新缓冲区。
  - `std::flush`：用于刷新输出缓冲区。
  - `std::setw`、`std::setprecision`等：用于格式化输出。

**2. 标准模板库（STL）**

`std`命名空间是标准模板库（STL）的核心所在，STL提供了丰富的数据结构和算法，这些内容定义在`<vector>`、`<list>`、`<map>`、`<algorithm>`等头文件中：

- **容器**：
  - **序列容器**：
    - `std::vector`：动态数组。
    - `std::list`：双向链表。
    - `std::deque`：双端队列。
    - `std::array`：固定大小的数组（C++11引入）。
  - **关联容器**：
    - `std::set`：基于红黑树的有序集合。
    - `std::multiset`：允许重复元素的有序集合。
    - `std::map`：基于红黑树的键值对映射。
    - `std::multimap`：允许重复键的键值对映射。
    - `std::unordered_set`、`std::unordered_map`等：基于哈希表的容器（C++11引入）。
  - **容器适配器**：
    - `std::stack`：栈。
    - `std::queue`：队列。
    - `std::priority_queue`：优先队列。
- **迭代器**：
  - 提供了多种迭代器类型（如`std::iterator`）和迭代器操作（如`std::begin`、`std::end`）。
- **算法**：
  - 提供了大量通用算法，如`std::sort`（排序）、`std::find`（查找）、`std::reverse`（反转）、`std::copy`（复制）等。

**3. 字符串**

`std`命名空间提供了`std::string`和`std::wstring`类，用于处理字符串操作，这些内容定义在`<string>`头文件中：

- `std::string`：用于处理8位字符的字符串。
- `std::wstring`：用于处理宽字符的字符串。
- 提供了字符串操作方法，如`std::string::size`、`std::string::find`、`std::string::substr`等。

**4. 动态内存管理**

`std`命名空间提供了智能指针和动态内存管理相关的工具，这些内容定义在`<memory>`头文件中：

- **智能指针**：
  - `std::unique_ptr`：独占所有权的智能指针。
  - `std::shared_ptr`：共享所有权的智能指针。
  - `std::weak_ptr`：弱引用智能指针。
- **动态内存分配**：
  - `std::allocator`：用于动态内存分配。
  - `std::make_shared`、`std::make_unique`：用于创建智能指针。

**5. 数值和数学运算**

`std`命名空间提供了丰富的数学函数和数值操作，这些内容定义在`<cmath>`、`<limits>`、`<numeric>`等头文件中：

- **数学函数**：
  - `std::sin`、`std::cos`、`std::tan`等：三角函数。
  - `std::sqrt`、`std::pow`等：平方根和幂运算。
  - `std::abs`、`std::fabs`等：绝对值。
- **数值操作**：
  - `std::numeric_limits`：用于获取数值类型的极限值（如最大值、最小值等）。
  - `std::accumulate`、`std::iota`等：数值操作算法。

**6. 时间和日期**

`std`命名空间提供了时间日期相关的类和函数，这些内容定义在`<chrono>`和`<ctime>`头文件中：

- **C++11时间库（`<chrono>`）**：
  - `std::chrono::duration`：表示时间间隔。
  - `std::chrono::time_point`：表示时间点。
  - `std::chrono::system_clock`：系统时钟。
- **C风格时间函数（`<ctime>`）**：
  - `std::time`、`std::localtime`、`std::strftime`等。

**7. 错误处理**

`std`命名空间提供了异常处理和错误处理机制，这些内容定义在`<exception>`、`<stdexcept>`等头文件中：

- **异常类**：
  - `std::exception`：所有标准异常的基类。
  - `std::runtime_error`、`std::logic_error`等：标准异常类。
- **断言**：
  - `std::abort`、`std::assert`等。

**8. 其他常用功能**

- **类型特性**：
  - `std::is_integral`、`std::is_floating_point`等：类型特征检查。
- **函数对象**：
  - `std::function`：用于封装可调用对象。
  - `std::bind`：用于绑定函数和参数。
- **线程支持（C++11）**：
  - `std::thread`：线程类。
  - `std::mutex`、`std::condition_variable`等：同步机制。

### 1.2 cpp基本数据类型

cpp中增添了bool类型，可以直接用，C语言中需要引入头文件 <stdbool.h>`才能使用。

### 1.3 内联函数

| 特性             | C语言                  | C++语言                            |
| ---------------- | ---------------------- | ---------------------------------- |
| **内联展开**     | 建议性，编译器可以忽略 | 强制性，编译器通常会内联展开       |
| **默认链接属性** | 外部链接（`extern`）   | 内部链接（`internal`）             |
| **类成员函数**   | 不适用                 | 类成员函数默认内联                 |
| **模板函数**     | 不适用                 | 模板函数默认内联                   |
| **语法**         | `inline`关键字         | `inline`关键字，类成员函数默认内联 |

### 1.4 auto区别

**1. C语言中的`auto`**

在C语言中，`auto`是一个存储类关键字，用于显式声明变量为“自动存储期”（Automatic Storage Duration）。这意味着变量在函数调用时被分配内存，在函数返回时自动释放内存。实际上，C语言中函数内部定义的局部变量默认就是自动存储期，因此`auto`关键字在C语言中很少被使用，因为它通常是多余的。

**C语言中的`auto`示例**

```c
#include <stdio.h>

void example() {
    auto int x = 10; // 显式声明为自动存储期
    printf("x = %d\n", x);
}

int main() {
    example();
    return 0;
}
```

在这个例子中，`auto`关键字是多余的，因为`x`作为函数内部的局部变量，默认就是自动存储期。

**2. C++语言中的`auto`**

在C++中，`auto`的作用完全不同于C语言。C++11引入了`auto`作为类型推导关键字，用于让编译器自动推导变量的类型。它极大地简化了代码，尤其是在处理复杂类型（如模板、迭代器等）时。

**C++中的`auto`示例**

```cpp
#include <iostream>
#include <vector>

int main() {
    auto x = 10; // x 的类型是 int
    auto y = 3.14; // y 的类型是 double
    auto z = std::vector<int>{1, 2, 3}; // z 的类型是 std::vector<int>

    for (auto it = z.begin(); it != z.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

在这个例子中，`auto`关键字让编译器自动推导变量的类型，避免了显式写出复杂的类型名称。

**3. 关键区别**

| 特性         | C语言中的`auto`                        | C++语言中的`auto`               |
| ------------ | -------------------------------------- | ------------------------------- |
| **用途**     | 声明自动存储期变量                     | 类型推导关键字                  |
| **是否常用** | 很少使用，因为局部变量默认是自动存储期 | 非常常用，用于简化代码          |
| **语法**     | `auto 类型 变量名;`                    | `auto 变量名 = 初始化表达式;`   |
| **标准引入** | C语言标准（自C语言诞生以来）           | C++11标准引入作为类型推导关键字 |

### 1.5 static

`static`关键字在C++中有多种用途，具体作用取决于它修饰的对象类型：

- **修饰局部变量**：改变变量的存储位置和生命周期，使其在函数调用结束后仍然有效。
- **修饰全局变量**：限制变量的作用域，使其只能在定义它的文件中访问。
- **修饰函数**：限制函数的作用域，使其只能在定义它的文件中调用。
- **修饰类成员变量**：表示变量是类级别的，属于类本身，而不是某个对象。
- **修饰类成员函数**：表示函数是类级别的，属于类本身，而不是某个对象。

### 1.6 拷贝构造函数

如果没定义，c++默认定义浅拷贝构造函数。

1.**显式拷贝初始化**：用一个已存在的对象初始化另一个新对象。

```cpp
MyClass obj1;
MyClass obj2 = obj1;  // 调用拷贝构造函数
```

2.**函数参数传递**：对象作为值参数传递给函数时。

```cpp
void func(MyClass obj) { /* ... */ }
func(obj1);  // 实参到形参的传递会调用拷贝构造函数
```

3.**函数返回对象**：函数按值返回对象时。

```cpp
MyClass createObj() {
    MyClass temp;
    return temp;  // 可能调用拷贝构造函数（具体取决于编译器优化）
}
```

4.**容器操作**：向容器（如 `vector`、`list`）添加对象时。

```cpp
std::vector<MyClass> vec;
vec.push_back(obj1);  // 调用拷贝构造函数复制对象到容器中
```

5.**数组初始化**：用已存在对象初始化数组元素。

```cpp
MyClass arr[2] = {obj1, obj2};  // 每个元素初始化时调用拷贝构造函数
```

| **拷贝构造函数**                      | **拷贝赋值运算符**                               |
| ------------------------------------- | ------------------------------------------------ |
| **创建新对象**：在对象初始化时调用    | **修改已有对象**：对已存在的对象进行赋值         |
| 语法：`MyClass(const MyClass& other)` | 语法：`MyClass& operator=(const MyClass& other)` |
| 调用时机： `MyClass obj2 = obj1;`     | 调用时机： `obj2 = obj1;`（`obj2` 已存在）       |

### 1.7 new用法

**1. 单个对象的动态分配**

使用`new`可以动态分配单个对象，并调用其构造函数进行初始化。

```cpp
类型* 指针变量 = new 类型(构造参数);
```

```C
int* ptr = new int; //C语言中，int *p = (int *)malloc(sizeof(int));
```

```C
int* arr = new int[10]; //C语言中，int *arr = (int *)malloc(sizeof(int)*10);
```



```cpp
#include <iostream>
using namespace std;

class MyClass {
public:
    int value;
    MyClass(int v) : value(v) {
        cout << "MyClass(" << value << ") constructed." << endl;
    }
    ~MyClass() {
        cout << "MyClass(" << value << ") destructed." << endl;
    }
};

int main() {
    MyClass* obj = new MyClass(10); // 动态分配对象并初始化
    cout << "Object value: " << obj->value << endl;

    delete obj; // 释放内存
    return 0;
}
```

**2. 动态分配数组**

使用`new`可以动态分配数组，并可以选择初始化数组元素。

```cpp
类型* 指针变量 = new 类型[数组大小];
类型* 指针变量 = new 类型[数组大小]{初始化列表};
```

```cpp
#include <iostream>
using namespace std;

int main() {
    // 动态分配数组，未初始化
    int* arr1 = new int[5];
    cout << "Array1 elements: ";
    for (int i = 0; i < 5; ++i) {
        arr1[i] = i + 1; // 手动初始化
        cout << arr1[i] << " ";
    }
    cout << endl;

    // 动态分配数组，初始化
    int* arr2 = new int[5]{1, 2, 3, 4, 5};
    cout << "Array2 elements: ";
    for (int i = 0; i < 5; ++i) {
        cout << arr2[i] << " ";
    }
    cout << endl;

    delete[] arr1; // 释放数组内存
    delete[] arr2; // 释放数组内存
    return 0;
}
```

**3. 动态分配对象数组**

可以动态分配一个对象数组，并初始化每个对象。

```cpp
类型* 指针变量 = new 类型[数组大小](构造参数);
```

```cpp
#include <iostream>
using namespace std;

class MyClass {
public:
    int value;
    MyClass(int v) : value(v) {
        cout << "MyClass(" << value << ") constructed." << endl;
    }
    ~MyClass() {
        cout << "MyClass(" << value << ") destructed." << endl;
    }
};

int main() {
    // 动态分配对象数组
    MyClass* arr = new MyClass[3]{MyClass(1), MyClass(2), MyClass(3)};
    cout << "Array elements: ";
    for (int i = 0; i < 3; ++i) {
        cout << arr[i].value << " ";
    }
    cout << endl;

    delete[] arr; // 释放数组内存
    return 0;
}
```

**4. 使用`new`分配多维数组**

可以使用`new`动态分配多维数组。

```cpp
类型** 指针变量 = new 类型*[行数][列数];
```

```cpp
#include <iostream>
using namespace std;

int main() {
    // 动态分配二维数组
    int** arr = new int*[3];
    for (int i = 0; i < 3; ++i) {
        arr[i] = new int[4];
    }

    // 初始化二维数组
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            arr[i][j] = i * 4 + j;
        }
    }

    // 打印二维数组
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 4; ++j) {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }

    // 释放二维数组
    for (int i = 0; i < 3; ++i) {
        delete[] arr[i];
    }
    delete[] arr;
    return 0;
}
```

**5. 使用`new`分配自定义类型**

可以使用`new`分配自定义类型的对象，并调用其构造函数。

```cpp
类型* 指针变量 = new 类型(构造参数);
```

```cpp
#include <iostream>
using namespace std;

class MyClass {
public:
    int value;
    MyClass(int v) : value(v) {
        cout << "MyClass(" << value << ") constructed." << endl;
    }
    ~MyClass() {
        cout << "MyClass(" << value << ") destructed." << endl;
    }
};

int main() {
    MyClass* obj = new MyClass(20); // 动态分配对象并初始化
    cout << "Object value: " << obj->value << endl;

    delete obj; // 释放内存
    return 0;
}
```

**9. 使用`new`分配内存并手动初始化**

可以使用`new`分配内存后，手动初始化对象。

```cpp
类型* 指针变量 = new 类型;
指针变量->成员变量 = 值;
```

### 1.7 移动语义

C++ 的移动语义是 C++11 引入的一项重要特性，它主要用于解决资源的高效转移问题，避免不必要的深拷贝，从而提升程序的性能。

**基本概念：**

- **左值**：指的是有名字、可以取地址的表达式，通常是变量或者可以被赋值的对象。比如变量、数组元素、函数返回的左值引用等。
- **右值**：指的是没有名字、不可以取地址的临时对象，通常是字面量、临时表达式结果或者函数返回的临时对象。例如，字面常量、函数返回的临时对象等。
- **左值引用**：使用 `&` 来声明，它只能绑定到左值。
- **右值引用**：使用 `&&` 来声明，它只能绑定到右值。

**右值引用**

移动语义的核心依托于右值引用，右值引用是一种新的引用类型，它可以绑定到右值（临时对象）上，==使用 `&&` 来表示==。

```cpp
#include <iostream>
// 左值引用
void printValue(int& value) {
    std::cout << "Left value: " << value << std::endl;
}
// 右值引用
void printValue(int&& value) {
    std::cout << "Right value: " << value << std::endl;
}
int main() {
    int x = 10;
    printValue(x);  // 调用左值引用版本
    printValue(20); // 调用右值引用版本
    return 0;
}
```

在上述代码里，`printValue(int& value)` 能够接收左值，而 `printValue(int&& value)` 则可以接收右值。

**移动构造函数与移动赋值运算符**

移动语义通常借助移动构造函数和移动赋值运算符来实现。

**移动构造函数**

移动构造函数的作用是从一个临时对象（右值）中转移资源，而非进行深拷贝。

```cpp
#include <iostream>
#include <vector>

class MyVector {
private:
    std::vector<int>* data;
public:
    // 构造函数
    MyVector() : data(new std::vector<int>()) {}

    // 析构函数
    ~MyVector() {
        delete data;
    }

    // 移动构造函数
    MyVector(MyVector&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    // 移动赋值运算符
    MyVector& operator=(MyVector&& other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            other.data = nullptr;
        }
        return *this;
    }
};

int main() {
    MyVector v1;
    MyVector v2 = std::move(v1); // 调用移动构造函数
    return 0;
}
```

在上述代码中，`MyVector(MyVector&& other)` 属于移动构造函数，它把 `other` 的资源指针转移给当前对象，接着将 `other` 的指针置为 `nullptr`，防止析构时重复释放资源。

移动赋值运算符

移动赋值运算符的功能是把一个临时对象（右值）的资源转移到当前对象。

```cpp
MyVector& operator=(MyVector&& other) noexcept {
    if (this != &other) {
        delete data;
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
```

在上述代码中，`operator=(MyVector&& other)` 是移动赋值运算符，它先释放当前对象的资源，再把 `other` 的资源指针转移给当前对象，最后将 `other` 的指针置为 `nullptr`。

`std::move`

`std::move` 是一个标准库函数，它的作用是将一个左值强制转换为右值引用，进而可以调用移动构造函数或移动赋值运算符。

```cpp
MyVector v1;
MyVector v2 = std::move(v1); // 调用移动构造函数
```

在上述代码中，`std::move(v1)` 把左值 `v1` 转换为右值引用，从而调用移动构造函数。

移动语义的优势

移动语义能够避免不必要的深拷贝，提升程序的性能。特别是在处理大型对象或者容器时，移动语义的优势会更加明显。

```cpp
#include <iostream>
#include <vector>

std::vector<int> createLargeVector() {
    std::vector<int> largeVector(1000000);
    return largeVector;
}

int main() {
    std::vector<int> result = createLargeVector(); // 使用移动语义
    return 0;
}
```

在上述代码中，`createLargeVector` 函数返回一个 `std::vector` 对象，由于使用了移动语义，避免了对 `largeVector` 的深拷贝，提高了性能。

综上所述，C++ 的移动语义通过右值引用、移动构造函数、移动赋值运算符和 `std::move` 函数，实现了资源的高效转移，避免了不必要的深拷贝，提升了程序的性能。

### 1.8 noexcept

在 C++ 里，`noexcept` 是一个异常说明符，它的作用是向编译器和代码使用者表明一个函数不会抛出异常。下面结合你给出的代码，详细讲解 `noexcept` 的含义与作用。

`noexcept` 有两种使用方式：

**不带参数的 `noexcept`**

当函数声明或定义时使用 `noexcept` 且不带参数，就表示该函数不会抛出任何异常。例如：

```cpp
void func() noexcept {
    // 函数体
}
```

在这个例子中，`func` 函数使用了 `noexcept`，意味着它不会抛出异常。

**带参数的 `noexcept`**

`noexcept` 可以接受一个常量表达式作为参数，这个常量表达式的结果为 `true` 时，函数不会抛出异常；结果为 `false` 时，函数可能会抛出异常。例如：

```cpp
void func2() noexcept(true) {
    // 不会抛出异常
}

void func3() noexcept(false) {
    // 可能会抛出异常
}
```

`noexcept` 在你提供代码中的作用

在给出的代码里，移动构造函数和移动赋值运算符都使用了 `noexcept`：

```cpp
// 移动构造函数
MyVector(MyVector&& other) noexcept : data(other.data) {
    other.data = nullptr;
}
// 移动赋值运算符
MyVector& operator=(MyVector&& other) noexcept {
    if (this != &other) {
        delete data;
        data = other.data;
        other.data = nullptr;
    }
    return *this;
}
```

这里使用 `noexcept` 的主要原因和作用如下：

性能优化

当编译器知道一个函数不会抛出异常时，就可以进行一些额外的优化。例如，在调用这个函数时，不需要生成用于异常处理的代码，这样可以减少代码的体积，提高运行效率。

容器操作的安全性

在标准库容器（如 `std::vector`）中，当进行元素的移动操作（像 `std::vector` 的 `resize`、`push_back` 等操作）时，如果移动构造函数或移动赋值运算符标记为 `noexcept`，容器就会优先使用移动操作，因为移动操作比拷贝操作更高效。如果没有 `noexcept` 标记，容器可能会因为担心移动过程中抛出异常导致数据不一致，而选择使用拷贝操作。

异常处理的影响

如果一个标记为 `noexcept` 的函数实际上抛出了异常，程序会调用 `std::terminate` 函数，这通常会导致程序异常终止。所以，在使用 `noexcept` 时，要确保函数确实不会抛出异常。

综上所述，在你的代码中，`noexcept` 向编译器和代码使用者表明移动构造函数和移动赋值运算符不会抛出异常，有助于提高性能和保证容器操作的安全性。

### 1.9 nullptr

在 C++ 里，`nullptr` 是 C++11 引入的一个关键字，用于表示空指针。`nullptr` 是一个空指针常量，它可以被隐式转换为任意类型的指针，并且不会像传统的 `NULL` 那样在某些情况下引发歧义。在 C++ 里，指针是用于存储内存地址的变量，当指针不指向任何有效的对象时，就可以将其赋值为 `nullptr`。

**与 `NULL` 的对比**

在 C++11 之前，通常使用 `NULL` 来表示空指针。`NULL` 一般被定义为 `0` 或者 `(void*)0`。不过，这种定义在函数重载等场景下会产生一些问题。

```cpp
#include <iostream>

void func(int x) {
    std::cout << "Called with int: " << x << std::endl;
}

void func(char* ptr) {
    std::cout << "Called with char*: " << (ptr ? "non-null" : "null") << std::endl;
}

int main() {
    func(0); // 调用 func(int)
    // func(NULL); // 存在歧义，可能调用 func(int) 或 func(char*)
    func(nullptr); // 明确调用 func(char*)
    return 0;
}
```

在上述代码中，当使用 `NULL` 时，由于它通常被定义为 `0`，调用 `func(NULL)` 会产生歧义，编译器不知道应该调用 `func(int)` 还是 `func(char*)`。而使用 `nullptr` 时，编译器能明确知道要调用 `func(char*)`，因为 `nullptr` 是专门的空指针类型。

作用

- **类型安全**：`nullptr` 具有明确的类型，它可以被隐式转换为任意指针类型，但不能被隐式转换为整数类型。这有助于避免因错误地将空指针用作整数而引发的问题。
- **避免歧义**：在函数重载、模板编程等场景下，`nullptr` 可以避免因 `NULL` 的二义性而导致的编译错误。

使用场景

- **初始化指针**：在声明指针变量时，可以将其初始化为 `nullptr`，表示该指针目前不指向任何有效的对象。

```cpp
int* ptr = nullptr;
```

- **检查指针是否为空**：在使用指针之前，通常需要检查它是否为空，以避免访问空指针导致的未定义行为。

```cpp
if (ptr != nullptr) {
    // 使用指针
}
```

- **函数参数传递**：当函数接受指针作为参数时，可以传递 `nullptr` 表示不传递有效的对象。

```cpp
void process(int* ptr) {
    if (ptr != nullptr) {
        // 处理指针指向的对象
    }
}

int main() {
    process(nullptr);
    return 0;
}
```

综上所述，`nullptr` 是 C++11 引入的用于表示空指针的关键字，它提供了更好的类型安全性和避免了 `NULL` 带来的歧义问题。

### 1.10 引用&初始化列表

引用必须在创建时绑定到一个对象，并且此后无法重新绑定到其他对象。构造函数体执行时，对象的所有成员变量已经完成初始化（默认初始化或无初始化），此时再对引用赋值会导致编译错误。

**1. 引用类型成员变量**

**错误示例**（在构造函数体内赋值）：

```cpp
class Example {
private:
    int& ref;  // 引用类型成员变量
public:
    Example(int x) {
        ref = x;  // 错误！引用必须在初始化列表中绑定
    }
};
```

**正确写法**（使用初始化列表）：

```cpp
class Example {
private:
    int& ref;
public:
    Example(int& x) : ref(x) {}  // 正确：在初始化列表中绑定引用
};
```

**2. 常量类型成员变量**

常量一旦初始化就不能被修改。构造函数体中的赋值操作属于修改已初始化的常量，这是不允许的。

**错误示例**（在构造函数体内赋值）：

```cpp
class Example {
private:
    const int value;  // 常量类型成员变量
public:
    Example(int x) {
        value = x;  // 错误！常量不能被赋值
    }
};
```

**正确写法**（使用初始化列表）：

```cpp
class Example {
private:
    const int value;
public:
    Example(int x) : value(x) {}  // 正确：在初始化列表中初始化常量
};
```

**3. 初始化列表的执行时机**

初始化列表在对象的**内存分配完成后、构造函数体执行前**执行。此时，所有成员变量（包括引用和常量）会按照它们在类中声明的顺序被初始化。而构造函数体内的代码只能修改已经初始化的成员变量。

**4. 对比：普通成员变量的初始化与赋值**

普通成员变量可以在初始化列表中初始化，也可以在构造函数体内赋值。例如：

```cpp
class Example {
private:
    int x;  // 普通成员变量
public:
    // 方式1：初始化列表（更高效，直接初始化）
    Example(int value) : x(value) {}

    // 方式2：构造函数体赋值（先默认初始化，再赋值）
    Example(int value) {
        x = value;  // 等价于：先默认初始化 x，再赋值
    }
};
```

但对于引用和const常量，只有初始化列表一种方式。

### 1.11 const用法

在 C++ 中，`const` 是一个强大的类型修饰符，用于声明对象或变量具有不可修改的属性。以下是其核心应用场景的总结：

**1. 常量变量**

- **作用**：禁止修改变量的值，增强代码安全性。

- 示例：

  ```cpp
  const int MAX_SIZE = 100;  // 编译期常量
  MAX_SIZE = 200;            // 错误：不能修改 const 变量
  ```

- **注意**：基本类型的 `const` 变量通常是编译期常量，但需结合 `constexpr` 明确指定。

**2. 常量指针与引用**

- 顶层 `const`：指针 / 引用本身不可变。

  ```cpp
  int x = 10;
  int* const ptr = &x;  // 指针不可变
  ptr = &y;             // 错误
  *ptr = 20;            // 正确：可以修改指向的值
  ```

- 底层 `const`：指向 / 引用的对象不可变。

  ```cpp
  const int* ptr = &x;  // 对象不可变
  *ptr = 20;            // 错误
  ptr = &y;             // 正确：可以修改指针
  ```

- 常量引用：常用于函数参数，避免拷贝且禁止修改实参。

  ```cpp
  void func(const std::string& str) { /* ... */ }
  ```

**3. 常量成员函数**

- **作用**：声明函数不会修改对象状态，允许 `const` 对象调用。

- 语法：在函数声明后加`const`

  ```cpp
  class MyClass {
  public:
      int getValue() const { return value; }  // 常量成员函数
      void setValue(int v) { value = v; }     // 非常量函数
  private:
      int value;
  };
  
  const MyClass obj;
  obj.getValue();  // 正确
  obj.setValue(10); // 错误：const 对象不能调用非常量函数
  ```

- **注意**：常量与非常量成员函数可重载（基于 `this` 指针的常量性）。

- **允许常量对象调用函数**

  如果没有常量成员函数，`const` 对象将无法调用任何成员函数（因为非 `const` 函数可能修改对象）

**4. 常量对象**

- 特性：==只能调用常量成员函数==，成员变量不可修改（除非`mutable`）。

  ```cpp
  class Logger {
  public:
      void log(const std::string& msg) const {
          messages.push_back(msg);  // 错误：const 函数不能修改成员
      }
  private:
      std::vector<std::string> messages;
  };
  ```

- `mutable` 例外：允许在const函数中修改的成员。

  ```cpp
  class Logger {
  public:
      void log(const std::string& msg) const {
          messages.push_back(msg);  // 正确：messages 是 mutable
      }
  private:
      mutable std::vector<std::string> messages;
  };
  ```

**5. 常量表达式 (`constexpr`)**

- **作用**：在编译期计算值，用于模板参数、数组大小等。

- 语法：

  ```cpp
  constexpr int square(int x) { return x * x; }
  constexpr int SIZE = square(5);  // 编译期常量
  int arr[SIZE];                   // 合法
  ```

- **与 `const` 的区别**：`constexpr` 必须在编译期确定值，而 `const` 可以在运行期初始化。

**6. 常量迭代器**

- `const_iterator`：禁止修改容器元素。

  ```cpp
  std::vector<int> vec = {1, 2, 3};
  std::vector<int>::const_iterator it = vec.begin();
  *it = 10;  // 错误：不能修改元素
  ```

- `cbegin()`/`cend()`：强制返回

  ```cpp
  for (auto it = vec.cbegin(); it != vec.cend(); ++it) {
      // 只读遍历
  }
  ```

**7. 函数返回值为 `const`**

- 作用：防止返回值被修改（常用于自定义类型）。

  ```cpp
  const Point operator+(const Point& p1, const Point& p2);
  Point a, b;
  (a + b) = c;  // 错误：返回值是 const
  ```

**8. 常量转换 (`const_cast`)**

- 慎用：移除 const属性，可能导致未定义行为。

  ```cpp
  const int x = 10;
  int& ref = const_cast<int&>(x);
  ref = 20;  // 未定义行为：修改 const 对象
  ```

### 1.12 this关键字

1. **区分成员变量与参数**

当函数参数与成员变量同名时，使用 `this` 明确访问成员：

```cpp
class Point {
private:
    int x, y;
public:
    void setPosition(int x, int y) {
        this->x = x;  // 左侧：成员变量，右侧：参数
        this->y = y;
    }
};
```

2. **返回当前对象引用**

实现链式调用（如 `obj.func1().func2()`）：

```cpp
class StringBuilder {
public:
    StringBuilder& append(const char* str) {
        // 追加字符串到内部缓冲区
        return *this;  // 返回当前对象的引用
    }
    StringBuilder& append(int num) {
        // 追加数字到内部缓冲区
        return *this;
    }
};

// 使用示例
StringBuilder().append("Hello").append(123);
```

3. **在常量成员函数中使用**

`this` 在常量成员函数中为 `const ClassName*` 类型：

```cpp
class MyClass {
public:
    void printAddress() const {
        std::cout << this;  // this 类型为 const MyClass*
    }
};
```

4. **传递当前对象给其他函数**

```cpp
class Logger {
public:
    void log(const MyClass* obj) { /* ... */ }
};

class MyClass {
public:
    void sendToLogger(Logger& logger) {
        logger.log(this);  // 将当前对象传递给 Logger
    }
};
```

**深入理解**

1. **类型与常量性**

- 在非常量成员函数中，`this` 类型为 `ClassName*`。
- 在常量成员函数中，`this` 类型为 `const ClassName*`（确保不修改对象）。

2. **显式使用 `this` 的场景**

- 访问隐藏的成员变量（如参数与成员同名时）。
- 返回当前对象引用（实现链式调用）。
- 指针运算（如在数组类中访问当前元素后的元素）。

3. **注意事项**

- 空指针风险：若通过空指针调用成员函数，this 会成为空指针，导致未定义行为。

  ```cpp
  MyClass* ptr = nullptr;
  ptr->func();  // 未定义行为：this 为空
  ```

- 不可赋值：this 是常量指针，不能被赋值。

  ```CPP
  this = other;  // 错误：不能修改 this 指针
  ```

**静态成员函数没有 `this` 指针**

- 静态成员属于类本身，不依赖于任何对象实例。调用静态函数时无需通过对象，因此没有 “当前对象” 可供 `this` 指向。

### 1.13 override

 C++11引入

在 C++ 中，`override` 是一个特殊的标识符（从 C++11 开始引入），用于显式声明一个虚函数将**重写（override）基类中的虚函数**。它的主要作用是提高代码的可读性和安全性，避免因函数签名不匹配而导致的意外行为。

**`override` 的作用**

1. **明确重写意图**：
   告诉编译器：“我正在尝试重写基类的虚函数”。如果基类中不存在匹配的虚函数，编译器会报错。
2. **防止函数签名错误**：
   如果派生类中的函数签名（如参数类型、返回值、常量性等）与基类中的虚函数不一致，编译器会指出错误，而不是默默地创建一个新的函数（隐藏基类函数）。

```cpp
#include <iostream>
#include <string>

class Animal {
protected:
    std::string name;
    int age;

public:
    Animal(std::string n, int a) : name(n), age(a) {}

    virtual void makeSound() {
        std::cout << name << " makes a sound." << std::endl;
    }

    virtual void display() {
        std::cout << "Animal: " << name << ", Age: " << age << std::endl;
    }
};

class Lion : public Animal {
public:
    Lion(std::string n, int a) : Animal(n, a) {}
    void makeSound() override {  // 明确重写基类的虚函数
        std::cout << name << " roars." << std::endl;
    }
    void display() override {  // 明确重写基类的虚函数
        std::cout << "Lion: " << name << ", Age: " << age << std::endl;
    }
};
```

- `override` 表明 `Lion` 类中的 `makeSound()` 和 `display()` 函数**必须重写基类 `Animal` 中的虚函数**。
- 如果基类 `Animal` 中没有声明同名的虚函数，或者函数签名不匹配（如参数不同），编译器会报错。

**为什么需要 `override`？**

1. **避免意外隐藏基类函数**

如果派生类中的函数签名与基类稍有不同（如拼写错误、参数类型不一致），编译器会认为这是一个新函数，而非重写。此时使用 `override` 会立即暴露问题。

**示例（无 `override` 时的隐患）**：

```cpp
class Base {
public:
    virtual void func(int x) { /* ... */ }
};

class Derived : public Base {
public:
    void func(double x) { /* ... */ }  // 错误：参数类型不同，实际隐藏了基类函数
};
```

- 此处 `Derived::func(double)` 与 `Base::func(int)` 参数类型不同，会隐藏基类函数，但编译器不会报错。
- 若添加 `override`，编译器会立即指出错误。

2. **提高代码可读性**

通过 `override`，其他开发者可以清晰地知道：“这个函数是基类虚函数的具体实现”。

**使用 `override` 的条件**

1. **基类函数必须是虚函数**：
   `override` 只能用于重写基类的 `virtual` 函数。
2. **函数签名必须严格匹配**：
   - 函数名、参数列表、返回值类型（协变返回值除外）、常量性（`const`）必须与基类完全一致。
3. **访问权限不能更严格**：
   例如，基类的虚函数是 `public`，派生类中不能将其重写为 `private`。

**其他相关关键字**

1. **`final`**：
   用于禁止进一步重写或继承。例如：

   ```cpp
   class Base {
   public:
       virtual void func() final { /* ... */ }  // 禁止派生类重写
   };
   ```

2. **纯虚函数（`= 0`）**：
   定义抽象基类，强制派生类实现该函数：

   ```cpp
   class Animal {
   public:
       virtual void makeSound() = 0;  // 纯虚函数，必须在派生类中实现
   };
   ```

### 1.14 虚析构函数

为什么 如果类中有虚函数，通常应该将析构函数也声明为虚的 

在 C++ 中，**如果类中定义了虚函数，通常应该将析构函数也声明为虚函数**。这是为了确保在通过基类指针删除派生类对象时，能够正确调用派生类的析构函数，避免内存泄漏。

**为什么需要虚析构函数？**

1. **问题场景：基类指针指向派生类对象**

当通过基类指针删除派生类对象时，如果基类析构函数**不是虚函数**，则只会调用基类的析构函数，而**不会调用派生类的析构函数**，导致派生类资源无法释放。

**示例（无虚析构函数导致内存泄漏）**：

```cpp
class Base {
public:
    ~Base() { std::cout << "Base destructor" << std::endl; } // 非虚析构函数
    virtual void func() { std::cout << "Base func" << std::endl; }
};

class Derived : public Base {
private:
    int* data;
public:
    Derived() { data = new int[10]; }
    ~Derived() { 
        delete[] data; // 释放动态分配的内存
        std::cout << "Derived destructor" << std::endl; 
    }
};

int main() {
    Base* ptr = new Derived(); // 基类指针指向派生类对象
    delete ptr; // 只调用 Base 的析构函数，Derived 的析构函数未被调用
    // 导致 Derived::data 内存泄漏！
    return 0;
}
```

**输出结果**：

```plaintext
Base destructor
```

2. **虚析构函数的作用**

如果将基类的析构函数声明为**虚函数**，则通过基类指针删除派生类对象时，会先调用派生类的析构函数，再调用基类的析构函数，确保资源正确释放。

**修正示例（虚析构函数）**：

```cpp
class Base {
public:
    virtual ~Base() { std::cout << "Base destructor" << std::endl; } // 虚析构函数
    virtual void func() { std::cout << "Base func" << std::endl; }
};

class Derived : public Base {
private:
    int* data;
public:
    Derived() { data = new int[10]; }
    ~Derived() { 
        delete[] data; 
        std::cout << "Derived destructor" << std::endl; 
    }
};

int main() {
    Base* ptr = new Derived();
    delete ptr; // 先调用 Derived 的析构函数，再调用 Base 的析构函数
    return 0;
}
```

**输出结果**：

```plaintext
Derived destructor
Base destructor
```

**虚析构函数的底层机制**

- **虚函数表（VTable）**：每个包含虚函数的类都有一个虚函数表，存储虚函数的地址。
- **虚析构函数的特殊性**：当基类析构函数为虚函数时，派生类的虚函数表会记录派生类的析构函数地址。因此，通过基类指针调用 `delete` 时，会根据对象的实际类型（而非指针类型）找到正确的析构函数。

**什么时候不需要虚析构函数？**

1. **基类不会被继承**：如果类不打算作为基类，无需虚析构函数。
2. **不会通过基类指针删除派生类对象**：如果代码中从不使用基类指针管理派生类对象，析构函数可以不是虚的。

**示例（无需虚析构函数）**：

```cpp
class Utility { // 工具类，不打算被继承
public:
    ~Utility() { /* ... */ } // 非虚析构函数
};
```

**虚析构函数的特殊处理**

当基类的析构函数被声明为 `virtual` 时：

1. **基类的虚函数表包含基类析构函数的地址**
   - 例如：`Base::~Base()` 的地址会被记录在 `Base` 类的虚函数表中。
2. **派生类的虚函数表会覆盖析构函数的地址**
   - 派生类的虚函数表中，析构函数的位置会被替换为**派生类析构函数的地址**。
     （注意：C++ 编译器会将析构函数视为特殊的虚函数，即使名称不同也会在虚表中占据一个槽位）

**运行时行为：通过基类指针删除派生类对象**

考虑以下代码：

```cpp
Base* ptr = new Derived();
delete ptr; // 调用哪个析构函数？
```

**执行步骤**：

1. **找到对象的虚函数表**
   - 虽然 `ptr` 是 `Base*` 类型，但它指向的实际对象是 `Derived`，因此 `ptr->vptr` 指向 `Derived` 类的虚函数表。
2. **从虚函数表中获取析构函数地址**
   - `Derived` 类的虚函数表中，析构函数槽位存储的是 `Derived::~Derived()` 的地址。
3. **调用正确的析构函数**
   - 首先执行 `Derived::~Derived()`，释放派生类的资源。
   - 派生类析构函数执行完毕后，自动调用基类的析构函数 `Base::~Base()`（这是 C++ 的规则，无论析构函数是否为虚函数）。

**对比：非虚析构函数的问题**

若基类析构函数**不是虚函数**：

1. **基类和派生类的虚函数表中均不包含析构函数地址**
   - 析构函数调用不通过虚函数表，而是**根据指针类型静态绑定**。
2. **通过基类指针删除派生类对象时**
   - 编译器直接调用 `Base::~Base()`（因为指针类型是 `Base*`），而**不会调用 `Derived::~Derived()`**。
   - 导致派生类的资源（如动态分配的内存、文件句柄）未被释放，造成内存泄漏。

**示例图解**

1. **虚析构函数的情况**

```cpp
class Base {
public:
    virtual ~Base() { /* ... */ } // 虚析构函数
};

class Derived : public Base {
public:
    ~Derived() { /* ... */ }
};

Base* ptr = new Derived();
delete ptr; // 调用 Derived::~Derived()
```

**虚函数表结构**：

```plaintext
Base 类的虚函数表：
[0] Base::~Base()
[1] Base::otherVirtualFunc()

Derived 类的虚函数表：
[0] Derived::~Derived()  <-- 覆盖基类析构函数
[1] Derived::otherVirtualFunc()  <-- 若重写
```

2. **非虚析构函数的情况**

```cpp
class Base {
public:
    ~Base() { /* ... */ } // 非虚析构函数
};

class Derived : public Base {
public:
    ~Derived() { /* ... */ }
};

Base* ptr = new Derived();
delete ptr; // 只调用 Base::~Base()
```

**虚函数表结构**：

```plaintext
Base 类的虚函数表：
[0] Base::otherVirtualFunc()

Derived 类的虚函数表：
[0] Derived::otherVirtualFunc()  <-- 若重写

// 析构函数不通过虚函数表调用
```

**关键结论**

1. **虚析构函数的作用**
   - 确保通过基类指针删除派生类对象时，能根据对象的实际类型找到正确的析构函数。
2. **虚函数表的核心机制**
   - 通过将派生类析构函数的地址存入虚函数表，实现运行时动态绑定。
3. **内存安全的保障**
   - 派生类析构函数先执行，释放派生类资源；再自动调用基类析构函数，确保完整释放对象。

**总结**：虚析构函数是 C++ 实现多态删除的安全机制，通过虚函数表实现了析构函数的动态绑定，避免内存泄漏。

**纯虚析构函数**

如果需要将类设计为抽象基类（不能实例化），但没有其他纯虚函数，可以定义**纯虚析构函数**，但必须提供实现：

```cpp
class Base {
public:
    virtual ~Base() = 0; // 纯虚析构函数
};

// 必须提供纯虚析构函数的实现
Base::~Base() { std::cout << "Base pure virtual destructor" << std::endl; }
```

| **场景**                               | **建议**               |
| -------------------------------------- | ---------------------- |
| 类中定义了虚函数                       | 将析构函数声明为虚函数 |
| 类作为基类，且可能通过基类指针删除对象 | 必须使用虚析构函数     |
| 类不打算被继承或不涉及多态删除         | 无需虚析构函数         |

**核心原则**：当类中存在虚函数时，虚析构函数是保证多态删除安全的最简单方法。即使目前不需要派生类，为基类添加虚析构函数也能避免未来扩展时的潜在风险。

### 1.15 基类指针指向派生类对象

在 C++ 中，使用**基类指针指向派生类对象**是实现 **多态（Polymorphism）** 的核心机制。这种用法允许通过统一的接口处理不同的派生类对象，是面向对象编程的重要特性。以下从多个角度解释其用途和必要性：

**为什么需要基类指针指向派生类对象？**

**实现多态（运行时绑定）**

通过基类指针调用虚函数时，实际执行的是派生类的重写版本，这取决于对象的**实际类型**（而非指针类型）。这是 C++ 实现动态多态的基础。

**示例**：

```cpp
class Shape {
public:
    virtual void draw() { std::cout << "Drawing Shape" << std::endl; }
};

class Circle : public Shape {
public:
    void draw() override { std::cout << "Drawing Circle" << std::endl; }
};

class Square : public Shape {
public:
    void draw() override { std::cout << "Drawing Square" << std::endl; }
};

int main() {
    Shape* shapes[2];
    shapes[0] = new Circle(); // 基类指针指向派生类对象
    shapes[1] = new Square();

    for (int i = 0; i < 2; i++) {
        shapes[i]->draw(); // 根据实际类型调用对应的 draw()
    }
    // 输出：
    // Drawing Circle
    // Drawing Square
    return 0;
}
```

- **关键点**：通过基类指针 `Shape*` 可以统一处理不同的派生类对象，调用各自的 `draw()` 实现。

2. **容器中存储不同派生类对象**

若要将多个派生类对象存储在同一个容器（如数组、`vector`）中，必须使用基类指针或引用。

**示例**：

```cpp
std::vector<Shape*> shapes;
shapes.push_back(new Circle());
shapes.push_back(new Square());

for (auto shape : shapes) {
    shape->draw(); // 多态调用
}
```

- **派生类指针无法替代**：若使用 `Circle*` 或 `Square*`，则容器只能存储单一类型的对象，失去了多态性。

3. **函数参数统一接口**

通过基类指针作为参数，可以编写接受任意派生类对象的通用函数。

**示例**：

```cpp
void renderShape(Shape* shape) {
    shape->draw(); // 调用实际对象的 draw()
}

renderShape(new Circle()); // 传递 Circle 对象
renderShape(new Square()); // 传递 Square 对象
```

**为什么不直接用派生类指针？**

1. **失去多态性**

派生类指针只能访问派生类自身的成员，无法通过基类接口统一调用。

**示例**：

```cpp
Circle* circlePtr = new Circle();
Shape* shapePtr = new Circle();

circlePtr->draw(); // 只能调用 Circle::draw()
shapePtr->draw();  // 可根据实际类型动态绑定
```

2. **无法统一管理对象**

若使用派生类指针，需要为每个派生类单独创建容器或函数，代码冗余且扩展性差。

**反例**：

```cpp
// 必须为每个派生类单独创建容器
std::vector<Circle*> circles;
std::vector<Square*> squares;

// 必须为每个派生类单独编写处理函数
void renderCircle(Circle* c) { c->draw(); }
void renderSquare(Square* s) { s->draw(); }
```

**基类指针的局限性**

1. **只能访问基类中定义的成员**
   - 若派生类有额外成员（如 `Circle::getRadius()`），基类指针无法直接访问，需通过类型转换（如 `dynamic_cast`）。
2. **内存管理需谨慎**
   - 若基类析构函数不是虚函数，通过基类指针删除派生类对象会导致内存泄漏。

**其他实现多态的方式**

1. **引用（Reference）**

与指针类似，基类引用也可绑定到派生类对象，实现多态调用：

```cpp
Circle circle;
Shape& shapeRef = circle; // 基类引用绑定派生类对象
shapeRef.draw(); // 调用 Circle::draw()
```

2. **智能指针（Smart Pointer）**

现代 C++ 推荐使用 `std::unique_ptr` 或 `std::shared_ptr` 管理动态对象，避免手动内存管理：

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>());
shapes.push_back(std::make_unique<Square>());

for (const auto& shape : shapes) {
    shape->draw(); // 多态调用
} // 自动释放内存
```

| **场景**                             | **选择**                | **原因**                                           |
| ------------------------------------ | ----------------------- | -------------------------------------------------- |
| 需要通过统一接口调用不同派生类的方法 | 基类指针 / 引用         | 实现多态，根据对象实际类型调用函数                 |
| 在容器中存储不同派生类对象           | 基类指针 / 智能指针     | 容器要求元素类型一致，基类指针可指向任意派生类对象 |
| 编写通用函数处理多种派生类对象       | 基类指针 / 引用作为参数 | 函数无需关心具体派生类类型，提高代码复用性         |

**核心思想**：基类指针是实现 “接口与实现分离” 的关键工具，让代码更具扩展性和可维护性。

### 1.16 智能指针

**智能指针**是 C++ 标准库提供的类模板，用于自动管理动态分配的内存，避免手动 `delete` 带来的内存泄漏风险。它的核心原理是**资源获取即初始化（RAII）**：当智能指针离开作用域时，会自动释放其管理的内存。

C++ 提供了三种主要的智能指针：

1. **`std::unique_ptr`**：独占所有权，同一时间只能有一个智能指针指向该对象。
2. **`std::shared_ptr`**：共享所有权，通过引用计数管理对象，最后一个持有者释放时销毁对象。
3. **`std::weak_ptr`**：弱引用，不控制对象生命周期，用于解决 `shared_ptr` 的循环引用问题。

**`std::unique_ptr` 的特性**

1. **独占所有权**

同一时间只能有一个 `unique_ptr` 指向某个对象。当 `unique_ptr` 被销毁或重置时，它管理的对象会被自动删除。

```cpp
std::unique_ptr<int> ptr1 = std::make_unique<int>(42); // 创建智能指针
// std::unique_ptr<int> ptr2 = ptr1; // 错误！不能复制 unique_ptr
std::unique_ptr<int> ptr2 = std::move(ptr1); // 必须通过移动语义转移所有权
```

2. **自动释放内存**

当 `unique_ptr` 离开作用域（如函数返回、块结束）时，其析构函数会自动调用 `delete`。

```cpp
void func() {
    std::unique_ptr<int> ptr = std::make_unique<int>(100);
    // ptr 离开作用域时，自动释放内存
} // 无需手动 delete
```

3. **轻量级实现**

`unique_ptr` 的开销通常与原始指针相同，没有额外的引用计数开销。

**代码示例解析**

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>());
shapes.push_back(std::make_unique<Square>());

for (const auto& shape : shapes) {
    shape->draw(); // 多态调用
} // 自动释放内存
```

1. **容器存储智能指针**

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
```

- 创建一个存储 `unique_ptr<Shape>` 的向量，用于管理不同类型的 `Shape` 对象。
- 由于 `unique_ptr` 不可复制，容器内部通过移动语义管理元素。

2. **添加对象到容器**

```cpp
shapes.push_back(std::make_unique<Circle>());
```

- `std::make_unique<Circle>()`：C++14 引入的工厂函数，创建并返回一个 `unique_ptr<Circle>`。
- `push_back`：通过移动语义将 `unique_ptr` 添加到容器中。

3. **多态调用**

```cpp
for (const auto& shape : shapes) {
    shape->draw(); // 多态调用
}
```

- `shape` 的类型是 `const std::unique_ptr<Shape>&`，即指向 `Shape` 的常量智能指针引用。
- `shape->draw()`：通过基类指针调用虚函数，实现多态（实际调用 `Circle::draw()` 或 `Square::draw()`）。

4. **自动内存管理**

```cpp
} // 自动释放内存
```

- 当 `shapes` 向量离开作用域时，其析构函数会销毁所有元素（`unique_ptr`）。
- 每个 `unique_ptr` 销毁时，会自动调用其管理对象的析构函数（如 `Circle::~Circle()`），确保内存释放。

**为什么使用智能指针？**

1. **避免内存泄漏**

手动管理内存时，若忘记 `delete` 或因异常导致 `delete` 未执行，会造成内存泄漏。智能指针通过 RAII 自动释放内存。

**对比示例（手动管理内存）**：

```cpp
std::vector<Shape*> shapes;
shapes.push_back(new Circle()); // 必须手动 delete
shapes.push_back(new Square());

for (auto shape : shapes) {
    shape->draw();
}

// 必须手动释放每个对象，否则内存泄漏
for (auto shape : shapes) {
    delete shape;
}
```

2. **异常安全**

即使代码因异常提前退出，智能指针仍会释放内存。

```cpp
void func() {
    std::unique_ptr<Shape> shape = std::make_unique<Circle>();
    // 若此处抛出异常，shape 仍会自动释放
} // 无需 try-catch-finally
```

3. **清晰表达所有权语义**

`unique_ptr` 明确表示独占所有权，使代码意图更清晰。

**智能指针的常见操作**

1. **创建智能指针**

```cpp
std::unique_ptr<int> ptr1 = std::make_unique<int>(42); // 推荐方式
std::unique_ptr<int> ptr2(new int(42)); // 等效但不推荐（异常安全问题）
```

2. **访问对象**

```cpp
ptr->method(); // 像原始指针一样使用
*ptr = 100;    // 解引用
```

3. **释放所有权**

```cpp
ptr.reset(); // 释放并删除对象
ptr = nullptr; // 等效于 reset()
```

4. **转移所有权**

```cpp
std::unique_ptr<int> ptr1 = std::make_unique<int>(42);
std::unique_ptr<int> ptr2 = std::move(ptr1); // ptr1 变为空
```

### 1.17 make_unique

`std::make_unique` 是==C++14==引入的标准库函数模板，用于**安全、便捷地创建 `std::unique_ptr` 对象**。它的主要作用是封装动态内存分配和 `unique_ptr` 的初始化，避免手动使用 `new` 带来的潜在风险。

1. **创建 `unique_ptr`**

```cpp
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args);
```

- **参数**：传递给 `T` 构造函数的参数。
- **返回值**：一个管理 `T` 类型对象的 `std::unique_ptr`。

 **示例**

```cpp
// 创建管理 int 的 unique_ptr
std::unique_ptr<int> ptr = std::make_unique<int>(42);

// 创建管理自定义对象的 unique_ptr
std::unique_ptr<Shape> circle = std::make_unique<Circle>(10.0); // 假设 Circle 构造函数接受半径

// 创建数组
std::unique_ptr<int[]> arr = std::make_unique<int[]>(5); // 动态数组，C++14 支持
```

**为什么需要 `make_unique`？**

1. **避免手动 `new`**

手动使用 `new` 初始化 `unique_ptr` 存在潜在风险：

```cpp
// 不推荐：手动使用 new
std::unique_ptr<int> ptr(new int(42));

// 更安全：使用 make_unique
auto ptr = std::make_unique<int>(42);
```

2. **异常安全**

在复杂表达式中，手动 `new` 可能导致内存泄漏。例如：

```cpp
// 潜在的内存泄漏（C++11 时代）
func(std::unique_ptr<Shape>(new Circle()), expensive_operation());

// 若 expensive_operation() 抛出异常，new Circle() 返回的指针会丢失，导致内存泄漏
```

使用 `make_unique` 则避免了这个问题：

```cpp
// 安全：make_unique 是原子操作
func(std::make_unique<Circle>(), expensive_operation());
```

3. **代码简洁**

`make_unique` 减少了重复书写类型名称的冗余：

```cpp
// 冗长
std::unique_ptr<std::vector<int>> vec(new std::vector<int>{1, 2, 3});

// 简洁
auto vec = std::make_unique<std::vector<int>>(1, 2, 3);
```

**`make_unique` 的优势**

1. **自动化推导对象类型**

无需显式指定模板参数类型：

```cpp
auto ptr = std::make_unique<int>(42); // 自动推导为 unique_ptr<int>
```

2. **统一数组处理**

支持创建动态数组（C++14 起）：

```cpp
auto arr = std::make_unique<int[]>(5); // 管理 int[5] 数组
arr[0] = 100;
```

3. **与容器无缝配合**

在容器中存储 `unique_ptr` 时更安全：

```cpp
std::vector<std::unique_ptr<Shape>> shapes;
shapes.push_back(std::make_unique<Circle>()); // 安全转移所有权
```

**注意事项**

1. **无法自定义删除器**

`make_unique` 创建的 `unique_ptr` 使用默认删除器（`delete`）。若需要自定义删除器（如关闭文件句柄），仍需手动使用 `new`：

```cpp
// 手动指定删除器
std::unique_ptr<FILE, decltype(&fclose)> file(
    fopen("test.txt", "r"),
    &fclose
);
```

2. **私有 / 受保护构造函数**

若类的构造函数是私有 / 受保护的，`make_unique` 可能无法访问（可通过友元解决）。

3. **与 `make_shared` 的区别**

- `make_unique` 创建 `unique_ptr`，管理单个对象。
- `make_shared` 创建 `shared_ptr`，共享对象所有权，并优化内存分配（单次分配同时存储对象和引用计数）。

**历史背景**

- **C++11**：仅引入 `make_shared`，未包含 `make_unique`（可能是标准制定时的疏忽）。

- **C++14**：补充 `make_unique`，使其与 `make_shared` 对称。

- C++11 兼容性：若需在 C++11 中使用类似功能，可自定义实现：

  ```cpp
  template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
      return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }
  ```

### 1.18 运算符重载

在 C++ 中，**运算符重载（Operator Overloading）** 允许为自定义类型重新定义运算符的行为，但**不会影响原始运算符的基本语义**或内置类型的使用方式。

### 1.19 range-base-for-loop

C++11 引入的**范围 - based for 循环（Range-based for loop）**，用于简化遍历容器或序列的语法。其核心作用是**自动迭代容器中的每个元素**，无需手动管理迭代器或索引。

```cpp
for (int num : myVector) {
    std::cout << num << " ";
}
```

**等价于传统 for 循环**：

```cpp
for (auto it = myVector.begin(); it != myVector.end(); ++it) {
    int num = *it;
    std::cout << num << " ";
}
```

**关键点**：

1. **自动迭代**：`myVector` 可以是任何支持 `begin()` 和 `end()` 的容器（如 `vector`、`array`、`list` 等）。
2. **类型推导**：`int num` 声明迭代变量，其类型需与容器元素类型匹配（或可隐式转换）。
3. **元素访问**：每次循环，`num` 自动绑定到当前元素的**副本**（除非使用引用）。

**引用与常量引用**

若需修改元素或避免拷贝，可使用**引用（`&`）\**或\**常量引用（`const &`）**：

```cpp
// 修改元素（使用引用）
for (int& num : myVector) {
    num *= 2; // 直接修改容器中的元素
}

// 只读访问（使用常量引用，避免拷贝）
for (const int& num : myVector) {
    std::cout << num << " "; // 高效读取元素
}
```

**适用范围**

范围 for 循环适用于所有实现了 `begin()`/`end()` 的类型：

1. **标准库容器**：`vector`、`list`、`map`、`set` 等。

2. 数组：

   ```cpp
   int arr[] = {1, 2, 3};
   for (int num : arr) { /* ... */ }
   ```

3. 初始化列表：

   ```cpp
   for (int num : {1, 2, 3}) { /* ... */ }
   ```

4. **自定义类型**：若类实现了 `begin()`/`end()` 成员或全局函数。

**注意事项**

1. **类型匹配**：迭代变量类型需与元素类型兼容：

   ```cpp
   std::vector<double> vec = {1.1, 2.2};
   for (int num : vec) { // 潜在精度丢失（double → int）
       std::cout << num; // 输出 1, 2
   }
   ```

2. **空容器安全**：循环会自动处理空容器，不会执行循环体。

3. **性能考量**：

   - 对基本类型（如 `int`），传值（`int num`）和传引用（`int&`）性能差异可忽略。
   - 对复杂对象（如 `std::string`），推荐使用 `const &` 避免拷贝。

**与传统 for 循环的对比**

| **特性**       | **范围 - based for 循环**                       | **传统 for 循环**                    |
| -------------- | ----------------------------------------------- | ------------------------------------ |
| **语法简洁性** | ✅ 无需手动管理迭代器或索引                      | ❌ 需要维护 `i` 或迭代器              |
| **安全性**     | ✅ 避免越界访问（自动处理 `end()`）              | ❌ 可能因索引错误导致越界             |
| **适用范围**   | ✅ 仅适用于支持 `begin()`/`end()` 的对象         | ✅ 可用于任意循环逻辑（如嵌套计数器） |
| **修改容器**   | ❌ 无法在循环中添加 / 删除元素（可能失效迭代器） | ✅ 可通过谨慎操作修改容器             |

**总结**

范围 - based for 循环是 C++11 的重要语法糖，用于**简化容器遍历**，提高代码可读性和安全性。其核心规则：

- **语法**：`for (类型 变量名 : 容器)`
- **引用传递**：使用 `&` 或 `const &` 避免拷贝或修改元素。
- **适用场景**：遍历容器的每个元素，无需复杂控制逻辑。

