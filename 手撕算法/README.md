面试准备一下就够了，应对百分之90的面试。

hot100+图论+重点补一下kruskal，prim，dijkstra +最短路，最小生成树这俩经典问题 +弗洛伊德算法

# 一、hot100知识点

## 1.1哈希表

在 C++ 中，**哈希表**（Hash Table）是一种通过哈希函数将键（key）映射到值（value）的高效数据结构，用于实现快速的数据插入、删除和查找操作。C++ 标准库提供了两种主要的哈希表实现：`std::unordered_map` 和 `std::unordered_set`，它们位于 `<unordered_map>` 和 `<unordered_set>` 头文件中。

**核心概念**

1. **哈希函数**
   将键（key）转换为一个固定范围的哈希值（通常是数组索引），理想情况下应均匀分布以减少冲突。
2. **冲突处理**
   当不同键映射到同一哈希值时，需通过以下方法解决：
   - **链地址法**：每个桶（bucket）是一个链表，冲突元素存储在链表中（C++ 默认实现）。
   - **开放寻址法**：通过探测（如线性探测、二次探测）找到下一个空槽。
3. **平均时间复杂度**
   - 插入、删除、查找：**O(1)**（理想情况下，无冲突时）。
   - 最坏情况（所有键冲突）：**O(n)**（退化为链表）。

**C++ 中的哈希表实现**

1. **`std::unordered_map`**

- 存储键值对（`key-value`），类似字典。

- 示例：

  ```cpp
  #include <iostream>
  #include <unordered_map>
  #include <string>
   
  int main() {
      std::unordered_map<std::string, int> wordCount;
      wordCount["apple"] = 3;
      wordCount["banana"] = 5;
   
      std::cout << wordCount["apple"] << std::endl; // 输出: 3
      return 0;
  }
  ```

2. **`std::unordered_set`**

- 仅存储唯一的键（key），类似集合。

- 示例：

  ```cpp
  #include <iostream>
  #include <unordered_set>
   
  int main() {
      std::unordered_set<int> nums = {1, 2, 3, 2};
      nums.insert(4);
   
      for (int num : nums) {
          std::cout << num << " "; // 输出顺序可能为: 3 2 1 4（无序）
      }
      return 0;
  }
  ```

**关键特性**

1. **无序性**
   元素存储顺序与插入顺序无关（与 `std::map`/`std::set` 的有序性不同）。

2. **自定义哈希函数**
   可通过模板参数指定自定义哈希函数和冲突解决策略：

   ```cpp
   struct MyHash {
       size_t operator()(const std::string& key) const {
           return std::hash<std::string>()(key) % 100; // 简单示例
       }
   };
   std::unordered_map<std::string, int, MyHash> customMap;
   ```

3. **性能调优**

   - `bucket_count()`：查看当前桶数量。
   - `load_factor()`：元素数量与桶数量的比值。
   - `rehash(n)`：预分配桶数量以减少冲突。

4. **时间复杂度**

   **查询平均是 O(1)**

   - **均匀分布假设**：在哈希函数设计良好且负载因子（`load_factor = 元素数量 / 桶数量`）较低时，**每个桶的链表长度非常短**（接近 1）。
   - 数学推导：
     - 假设哈希表有 `n` 个元素和 `m` 个桶，负载因子 `λ = n/m`。
     - 如果哈希函数均匀分布键，每个桶的期望元素数量为 `λ`。
     - 查询时需要遍历链表，平均时间复杂度为 **O(λ)**。
     - 当 `λ` 为常数（如 C++ 默认负载因子阈值为 0.75，扩容时保持 `λ ≤ 0.75`），则时间复杂度为 **O(1)**。

**与有序容器的对比**

| 特性           | `std::unordered_map` | `std::map` (基于红黑树) |
| -------------- | -------------------- | ----------------------- |
| **时间复杂度** | 平均 O(1)            | O(log n)                |
| **元素顺序**   | 无序                 | 按键排序                |
| **内存开销**   | 较高（需维护桶）     | 较低                    |
| **适用场景**   | 快速随机访问         | 需要有序遍历            |

**注意事项**

1. **哈希冲突**：设计良好的哈希函数可减少冲突。
2. **内存开销**：哈希表通常比数组或链表占用更多内存。
3. **不可哈希类型**：自定义类型需提供哈希函数或特化 `std::hash`。

#### 1.2.4 自定义哈希表

一、先明确 `unordered_map` 的构造函数默认参数

`unordered_map` 的构造函数有多个重载，其中最常用的形式为：

```cpp
unordered_map(
    size_t bucket_count = /* 实现定义的默认值 */,  // 初始桶数，默认通常是一个较小的数（如8）
    const Hash& hash = Hash(),                      // 哈希函数实例，默认用 Hash 类型的默认构造函数创建
    const KeyEqual& equal = KeyEqual()              // 相等比较器，默认用其默认构造函数创建
);
```

可见，**初始桶数和哈希函数实例都是有默认值的**。但这个默认值能否生效，取决于哈希函数的类型 `Hash` 是否支持 “默认构造”（即能否通过 `Hash()` 创建实例）。

二、为什么 “官方的哈希表” 不需要显式写？

这里的 “官方的哈希表” 指的是用标准类型（如 `int`、`string`）作为键，使用默认哈希函数 `std::hash` 的情况。例如：

```cpp
unordered_map<string, int> mp;  // 不需要显式传桶数和哈希函数
```

原因有两点：

1. **哈希函数类型 `std::hash<Key>` 有默认构造函数**：
   标准库为 `int`、`string` 等基本类型提供了 `std::hash` 的特化实现，且这些特化版本都有默认构造函数（可以通过 `std::hash<Key>()` 创建一个有效的哈希函数实例）。
   因此，构造函数中 `hash = Hash()` 这一默认参数可以正常生效，无需手动传入实例。
2. **初始桶数有默认值**：
   即使不指定初始桶数，编译器会使用一个默认值（如 8、16 等，不同编译器可能不同），后续会根据元素数量自动扩容，因此无需显式指定。

三、为什么 “自定义哈希函数的哈希表” 可能需要显式写？

以你的代码为例：

```cpp
// 自定义哈希函数（lambda表达式）
auto arrayHash = [fn = hash<int>{}] (const array<int, 26>& arr) -> size_t { ... };

// 声明哈希表
unordered_map<array<int, 26>, vector<string>, decltype(arrayHash)> mp(0, arrayHash);
```

需要显式传入 `(0, arrayHash)` 的核心原因是：**lambda 表达式的类型没有默认构造函数**。

具体来说：

- 这里的哈希函数类型是 `decltype(arrayHash)`（即 lambda 表达式的匿名类型）。C++ 标准规定，lambda 表达式的类型**没有默认构造函数**（无法通过 `decltype(arrayHash)()` 创建实例）。
- 因此，构造函数中 `hash = Hash()` 这一默认参数会失效（因为无法默认构造），必须**手动传入一个哈希函数的实例**（即 `arrayHash`）。

而 “初始桶数” 在这里显式写 `0` 是可选的：

- 如果你写成 `mp(arrayHash)`，编译器会自动用默认桶数（比如 8），但需要注意参数顺序（桶数在前，哈希函数在后）。
- 显式写 `0` 是一种习惯，让编译器自动管理初始桶数（后续动态扩容），避免默认值可能带来的微小性能差异。

**四、例外情况：如果自定义哈希函数有默认构造函数呢？**

如果你的自定义哈希函数不是 lambda，而是一个有默认构造函数的结构体 / 类，那么可以不用显式传入实例。例如：

```cpp
// 自定义哈希函数（结构体，有默认构造函数）
struct MyHash {
    size_t operator()(const array<int, 26>& arr) const {
        // 哈希逻辑
    }
};

// 声明哈希表时，无需显式传入哈希函数实例（因为MyHash有默认构造）
unordered_map<array<int, 26>, vector<string>, MyHash> mp;  // 合法
```

此时，构造函数会自动用 `MyHash()` 创建哈希函数实例，因此无需手动传入。

总结

- **是否需要显式传入哈希函数实例**：取决于哈希函数的类型是否有默认构造函数。
  - 标准库的 `std::hash` 或有默认构造的自定义哈希函数（如结构体）：不需要显式传入（用默认构造）。
  - 无默认构造的哈希函数（如 lambda 表达式）：必须显式传入实例。
- **是否需要显式传入初始桶数**：可选。
  - 不写则用默认值（如 8），写则指定初始大小（`0` 表示让编译器自动管理）。



------

## 1.2哈希函数

#### 1.2.1 lambda 表达式

**普通函数的写法：**

```
bool is_even(int x) {
    return x % 2 == 0;
}
```

**用 lambda 表达式写：**

```
auto is_even = [](int x) {
    return x % 2 == 0;
};
```

和普通函数作用一模一样，只不过是**匿名函数**，直接用变量保存。

**捕获列表**

它是让 lambda **可以访问外部变量** 的机制！

**不捕获变量（只能用参数）：**

```
auto f = [](int x) {
    return x + 1;
};
```

**捕获变量（让函数体能访问外部变量）：**

1. **全局变量**（无需捕获）
2. **局部变量**（必须显式捕获）

```
int base = 10;

auto f = [base](int x) { // 捕获base的值
    return x + base;
};

f(5); // 结果 15
```

`[]` 中的 `base` 表示：把外部的 base 变量值复制进来（值捕获）

**捕获方式详解**

| 写法      | 意义                               |
| --------- | ---------------------------------- |
| `[base]`  | 只捕获 base（值捕获，复制一份）    |
| `[&base]` | 捕获 base 的引用（改外部也会生效） |
| `[=]`     | 把用到的所有变量都值捕获（拷贝）   |
| `[&]`     | 把用到的所有变量都引用捕获         |

```
int a = 3, b = 4;

auto f1 = [=]() { return a + b; };     // 捕获 a 和 b 的值
auto f2 = [&]() { return a + b; };     // 捕获 a 和 b 的引用
auto f3 = [a, &b]() { return a + b; }; // 捕获 a 的值, b 的引用
```

#### 1.2.2 hash

```cpp
uint32_t compute_hash =std::hash<std:: string_view>{}(
  std::string_view(reinterpret_cast<const char *>(encoded.data()),encoded.size()-sizeof(uint32_t))
);
```

`std::hash` 是 C++ 标准库（`<functional>` 头文件）中用于生成哈希值的函数对象模板，广泛应用于哈希表（如 `unordered_map`、`unordered_set`）、哈希集合等数据结构中。它通过将输入数据映射为一个 `size_t` 类型的整数值（哈希值），实现高效的数据检索和存储。

==上述代码{} 是创建一个临时对象 后面的（）是传参，是operator()的参数==

 **为什么可以这样写？**

C++ 允许在构造临时对象后**立即调用其成员函数**，语法上等价于：

```cpp
临时对象.operator()(参数);
```

例如：

```cpp
struct Foo {
    void operator()(int x) { std::cout << x << std::endl; }
};
 
Foo{}(42);  // 构造临时对象并调用 operator()(42)
```

**核心特性**

1. **函数对象模板**
   `std::hash<T>` 是针对类型 `T` 定义的模板，不同类型需要特化以提供哈希计算逻辑。例如：
   - `std::hash<int>`：整数哈希
   - `std::hash<std::string>`：字符串哈希
   - `std::hash<double>`：浮点数哈希（需注意精度问题）
2. **返回值类型**
   始终返回 `size_t` 类型（无符号整数，通常与平台指针长度一致，如 64 位系统为 64 位）。
3. **哈希特性**
   - **确定性**：相同输入必定生成相同哈希值。
   - **高效性**：计算复杂度通常为 O (1) 或 O (n)（n 为数据长度，如字符串）。
   - **分布性**：理想情况下，不同输入的哈希值应均匀分布，减少哈希冲突。

**标准库预定义的特化**

C++ 标准库为常见类型提供了 `std::hash` 的特化实现：

| 类型                                               | 哈希计算方式                                                 |
| -------------------------------------------------- | ------------------------------------------------------------ |
| `bool`, `char`, `int`, `long` 等基础类型           | 直接将数值转换为 `size_t`（如 `int` 的值直接作为哈希值）。   |
| `float`, `double`                                  | 将二进制位 reinterpret 为 `uint32_t` 或 `uint64_t` 后计算哈希。 |
| `std::string`                                      | 对字符串每个字符的 ASCII 码进行迭代计算（通常使用类似 DJB2 或 FNV 算法）。 |
| `std::wstring`, `std::u16string`, `std::u32string` | 类似 `std::string`，按字符类型处理。                         |
| `std::shared_ptr<T>`, `std::unique_ptr<T>`         | 基于指针地址计算哈希。                                       |
| `std::pair<T1, T2>`, `std::tuple<T...>`            | 组合各个元素的哈希值（如 `hash(pair) = hash(first) ^ (hash(second) << 1)`）。 |

**使用示例**

1. **基础类型哈希**

   ```cpp
   std::hash<int> hash_int;
   std::cout << hash_int(42) << std::endl;  // 输出 42 对应的哈希值
   
   std::hash<std::string> hash_str;
   std::cout << hash_str("hello") << std::endl;  // 输出字符串的哈希值
   ```

2. **在容器中使用**

   ```cpp
   std::unordered_map<std::string, int> map;
   // 内部自动使用 std::hash<std::string> 计算键的哈希值
   map["key"] = 100;
   ```

3. **自定义类型的哈希**
   若要对自定义类型（如结构体）使用哈希，需特化 `std::hash`

   ```cpp
   struct Person {
       std::string name;
       int age;
   };
   
   // 为 Person 特化 std::hash
   namespace std {
       template<>
       struct hash<Person> {
           size_t operator()(const Person& p) const {
               return std::hash<std::string>{}(p.name) ^ (std::hash<int>{}(p.age) << 1);
           }
       };
   }
   
   // 使用示例
   std::unordered_set<Person> people;
   ```

- 上述代码是对 `std::hash` 的**全特化**（full specialization），用于计算 `Person` 对象的哈希值。
- `operator()` 接受一个 `const Person&` 参数，返回 `size_t` 类型的哈希值。
- 哈希值的计算方式：
  1. 计算 `p.name` 的哈希值（`std::hash<std::string>{}(p.name)`）。
  2. 计算 `p.age` 的哈希值（`std::hash<int>{}(p.age)`），并左移 1 位（`<< 1`）。
  3. 将两个结果进行**按位异或**（`^`），得到最终的哈希值。

```cpp
std::unordered_set<Person> people;
```

- 由于 `std::hash<Person>` 已经特化，`std::unordered_set` 可以直接使用 `Person` 作为键。

**注意事项**

1. **哈希冲突**
   不同输入可能生成相同哈希值（冲突），标准库容器会通过链表或开放寻址法处理冲突，但良好的哈希函数可减少冲突概率。
2. **浮点数哈希的不确定性**
   - `NaN` 的哈希值未定义，不同 `NaN` 可能生成不同哈希值。
   - 精度损失可能导致不同数值生成相同哈希值（如 `1.0` 和 `1.0000000001`）。
3. **自定义哈希的要求**
   自定义类型的哈希函数需满足：
   - 若 `a == b`，则 `hash(a) == hash(b)`（一致性）。
   - 尽量保证不同值的哈希值分布均匀 ==（<<1）==。
4. **C++17 新增特性**
   - `std::hash<std::string_view>`：高效处理字符串视图的哈希。
   - `std::hash` 支持更多标准类型（如 `std::optional`、`std::variant`）。

**与其他哈希函数的对比**

- **与 `boost::hash` 的区别**
  `boost::hash` 提供更强大的组合哈希功能（如直接支持元组），而 `std::hash` 是标准库的一部分，兼容性更好。
- **与具体哈希算法（如 MD5、SHA-1）的区别**
  `std::hash` 侧重于高效映射（速度优先），而 MD5 等加密哈希算法侧重抗碰撞性（安全性优先），两者应用场景不同。

#### 1.2.3 lambda+hash

利用lambda创建自定义hash函数：

```c++
auto arrayHash = [fn = hash<int>{}] (const array<int, 26>& arr) -> size_t {
    return accumulate(arr.begin(), arr.end(), 0u, [&](size_t acc, int num) {
        return (acc << 1) ^ fn(num);
    });
};
```

- 对于 lambda 表达式（匿名函数），如果编译器无法自动推断返回值类型（尤其是复杂逻辑时），需要用 `-> 返回类型` 的形式显式指定，这种语法称为**尾置返回类型（trailing return type）**。

- **`accumulate`的作用**

  函数体用了标准库的`accumulate`算法（需要`#include <numeric>`），它的功能是**遍历容器并累积计算一个结果**。这里的用法是：

  ```cpp
  accumulate(
      arr.begin(),  // 遍历起始位置
      arr.end(),    // 遍历结束位置
      0u,           // 初始值（0u表示无符号整数0）
      [&](size_t acc, int num) {  // 累加器（核心计算逻辑） 引用捕获捕获fn
          return (acc << 1) ^ fn(num);
      }
  )
  ```

  简单说：遍历`arr`中的 26 个整数（`num`），用累加器逐步计算出最终的哈希值，初始值是`0u`。

#### 1.2.4 accumulate

`accumulate` 是 C++ 标准库 `<numeric>` 头文件中的一个通用算法，用于**对容器中的元素进行累积计算**，可以实现求和、求积、拼接字符串等多种累积操作。它的核心功能是遍历容器中的元素，通过一个自定义的 “累加器函数” 将元素逐步合并成一个最终结果。

一、基本语法

`accumulate` 有两种常用重载形式，最基础的用法是：

```cpp
#include <numeric>  // 必须包含的头文件

// 形式1：默认累加（求和）
T accumulate(
    InputIt first,       // 容器起始迭代器
    InputIt last,        // 容器结束迭代器（不包含）
    T init               // 初始值
);

// 形式2：自定义累加规则
T accumulate(
    InputIt first,       // 容器起始迭代器
    InputIt last,        // 容器结束迭代器（不包含）
    T init,              // 初始值
    BinaryOp op          // 自定义累加函数（二元操作符）
);
```

二、参数说明

1. **`first` 和 `last`**：
   迭代器范围，表示要遍历的元素区间 `[first, last)`（包含 `first` 指向的元素，不包含 `last` 指向的元素）。
2. **`init`**：
   累积计算的初始值，其类型 `T` 决定了最终结果的类型。
3. **`op`**（可选）：
   自定义的二元函数（可以是函数指针、lambda 表达式、仿函数等），用于定义元素的累积规则。
   函数签名为 `T op(const T& acc, const Type& elem)`，其中：
   - `acc` 是当前累积的中间结果（类型为 `T`）。
   - `elem` 是当前遍历到的容器元素（类型为容器中元素的类型）。
   - 返回值是新的累积结果（类型为 `T`）。

三、工作原理

`accumulate` 的执行流程非常简单：

1. 初始化一个累积变量 `result`，值为 `init`。
2. 遍历`[first, last)`区间内的每个元素`elem`：
   - 若使用形式 1（无 `op`）：`result = result + elem`（默认做加法）。
   - 若使用形式 2（有 `op`）：`result = op(result, elem)`（按自定义规则计算）。
3. 遍历结束后，返回 `result`。

四、使用示例

示例 1：基础用法（求和）

用默认的累加规则计算容器中元素的总和：

```cpp
#include <iostream>
#include <vector>
#include <numeric>  // 包含accumulate

int main() {
    std::vector<int> nums = {1, 2, 3, 4, 5};
    
    // 计算总和，初始值为0
    int sum = std::accumulate(nums.begin(), nums.end(), 0);
    
    std::cout << "总和：" << sum << std::endl;  // 输出：15（0+1+2+3+4+5）
    return 0;
}
```

示例 2：自定义规则（求积）

通过自定义累加函数计算元素的乘积：

```cpp
#include <iostream>
#include <vector>
#include <numeric>

int main() {
    std::vector<int> nums = {1, 2, 3, 4};
    
    // 自定义乘法规则：acc * elem
    int product = std::accumulate(
        nums.begin(), 
        nums.end(), 
        1,  // 初始值为1（乘法的单位元）
        [](int acc, int elem) { return acc * elem; }  // lambda作为累加函数
    );
    
    std::cout << "乘积：" << product << std::endl;  // 输出：24（1*1*2*3*4）
    return 0;
}
```

示例 3：拼接字符串

累积字符串容器，拼接成一个完整字符串：

```cpp
#include <iostream>
#include <vector>
#include <string>
#include <numeric>

int main() {
    std::vector<std::string> words = {"Hello", " ", "World", "!"};
    
    // 自定义字符串拼接规则
    std::string sentence = std::accumulate(
        words.begin(), 
        words.end(), 
        std::string(""),  // 初始值为空字符串
        [](const std::string& acc, const std::string& elem) {
            return acc + elem;  // 拼接字符串
        }
    );
    
    std::cout << sentence << std::endl;  // 输出：Hello World!
    return 0;
}
```

示例 4：与哈希函数结合（回到你的代码）

在你提供的哈希函数中，`accumulate` 用于将 `array<int, 26>` 中的 26 个整数 “累积” 成一个哈希值：

```cpp
// 遍历array<int,26>中的每个元素，用自定义规则计算哈希值
return accumulate(arr.begin(), arr.end(), 0u, 
    [&](size_t acc, int num) {
        return (acc << 1) ^ fn(num);  // 自定义哈希累积规则
    }
);
```

这里的累积逻辑是：将每个整数的哈希值通过 “左移 + 异或” 逐步混入中间结果，最终得到整个数组的哈希值。

五、注意事项

1. **初始值的重要性**：
   初始值 `init` 的类型决定了结果类型。例如，若计算浮点型总和，初始值应设为 `0.0` 而非 `0`：

   ```cpp
   std::vector<int> nums = {1, 2, 3};
   double sum = std::accumulate(nums.begin(), nums.end(), 0.0);  // 结果为6.0（double类型）
   ```

2. **自定义函数的设计**：
   累加函数 `op` 需要保证线程安全性和无副作用，避免修改外部变量（除非有意为之）。

3. **适用范围**：
   `accumulate` 适用于所有支持迭代器的容器（`vector`、`list`、`array` 等），甚至可以遍历数组。

在使用 `unordered_map` 时，是否需要显式指定 “初始桶数” 和 “哈希函数实例”，取决于**哈希函数的类型是否有默认构造函数**以及是否使用了标准库的默认哈希函数。

