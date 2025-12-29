# qt基础知识

## 1.**信号（Signal）**

### 一、先搞懂：信号到底是什么？

信号本质是 **Qt 对象发出的 “通知 / 事件”** —— 当某个动作发生时（比如按钮被点击、登录页要切注册页、数据加载完成），对象会发出一个信号，告诉外界 “我这里发生了这件事”。

举个生活例子：

- 你（主窗口）点了外卖（登录对话框），外卖小哥（登录对话框对象）做好饭后，不会直接闯进你家，而是发个 “外卖到了” 的短信（信号）；

- 你收到短信（信号被监听），才下楼取餐（执行槽函数）。

  

  信号就是这个 “短信”，只负责 “通知”，不负责 “做事”；做事的是槽函数。

### 二、信号的核心特性（零基础必记）

1. **信号是 “声明” 而非 “实现”**：只需要告诉 Qt “我有这个信号”，不用写函数体（Qt 会通过 `moc` 工具自动生成实现）；
2. **信号无返回值**：不管有没有参数，信号都不能有返回值（比如 `void` 类型）；
3. **信号属于 QObject 子类**：只有继承 `QObject` 的类（比如 QWidget、QDialog、自定义窗口）才能定义信号；
4. **信号通过 `emit` 触发**：想要发出信号，必须用 `emit` 关键字（Qt 专属）调用信号；
5. **信号可带参数**：能传递数据（比如登录失败时，信号带 “用户名不存在” 的提示文本）；
6. **信号不依赖接收者**：发出信号时，不管有没有人 “听”（连接槽），都不会报错（没人听就相当于 “短信发了但没人看”）。

### 三、信号的基础用法（从 “定义→触发→使用” 三步走）

#### 第一步：准备前提（必须满足，否则信号失效）

所有要定义信号的类，必须满足两个条件：

1. 继承 `QObject`（或 `QObject` 的子类，比如 QDialog、QMainWindow、QWidget）；
2. 类声明开头加 `Q_OBJECT` 宏（Qt 用来处理信号槽的元数据，少了必失效）。

示例（自定义登录对话框类）：

```cpp
// LoginDialog.h（头文件）
#include <QDialog> // QDialog 继承自 QObject，满足条件1

// 自定义登录对话框类
class LoginDialog : public QDialog {
    Q_OBJECT // 条件2：必须加这个宏，否则信号槽全失效
public:
    explicit LoginDialog(QWidget *parent = nullptr); // 构造函数
};
```

#### 第二步：定义信号（用 `signals:` 关键字）

在类的声明中，用 `signals:` 关键字标记信号（类似 `public/private` 访问控制），格式：

```cpp
signals:
    // 格式：void 信号名(可选参数列表);
    void 信号名();                // 无参信号
    void 信号名(参数类型1, 参数类型2); // 带参信号
```

实战示例（给 LoginDialog 定义 2 个常用信号）：

```cpp
// LoginDialog.h
#include <QDialog>
#include <QString> // 带参信号要用字符串，需包含头文件

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = nullptr);

// 定义信号区（signals: 无需加 public/private，默认是 public）
signals:
    // 信号1：用户点击“切换到注册页”按钮（无参）
    void switchToRegister(); 
    // 信号2：登录失败（带参，传递失败原因）
    void loginFailed(QString errorMsg); 
    // 信号3：登录成功（带参，传递用户名）
    void loginSuccess(QString username);
};
```

关键注意：

- `signals:` 是 Qt 关键字，不是 C++ 原生的；
- 信号不能加 `static`/`const` 等修饰；
- 信号的参数可以是任意 Qt 支持的类型（int、QString、自定义类 <需注册>）。

#### 第三步：触发信号（用 `emit` 关键字）

信号定义后不会自动发出，需要在 “动作发生时” 用 `emit` 触发。

比如：在 LoginDialog 中，用户点击 “注册” 按钮时触发 `switchToRegister()` 信号，点击 “登录” 按钮验证失败时触发 `loginFailed()` 信号。

实战示例（LoginDialog 的实现文件）：

```cpp
// LoginDialog.cpp
#include "LoginDialog.h"
#include <QPushButton> // 按钮类

// 构造函数：初始化按钮并绑定点击事件
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    // 1. 创建“注册”按钮
    QPushButton *btnReg = new QPushButton("切换到注册", this);
    // 按钮点击时，触发 switchToRegister 信号
    connect(btnReg, &QPushButton::clicked, this, [=]() {
        emit switchToRegister(); // 触发无参信号
    });

    // 2. 创建“登录”按钮
    QPushButton *btnLogin = new QPushButton("登录", this);
    connect(btnLogin, &QPushButton::clicked, this, [=]() {
        // 模拟登录验证（项目中替换为真实逻辑）
        QString username = "test"; // 假设从输入框获取
        QString pwd = "123";       // 假设从输入框获取
        if (pwd != "123456") {
            // 验证失败，触发带参信号：传递失败原因
            emit loginFailed("密码错误！");
        } else {
            // 验证成功，触发带参信号：传递用户名
            emit loginSuccess(username);
        }
    });
}
```

⚠️ 关键注意：

- `emit` 是 Qt 关键字，本质是空宏（编译时会被忽略），作用是 “标记这是触发信号”；
- 触发信号的本质是 “调用信号函数”，所以格式和调用普通函数一样（带参就传参）；
- 信号可以在任意地方触发（按钮点击、定时器超时、自定义逻辑里都可以）。

#### 第四步：使用信号（和槽连接，核心！）

信号本身只是 “通知”，必须通过 `connect` 函数绑定到「槽函数」，才能产生实际效果（否则信号发了也白发）。

实战示例（主窗口中监听 LoginDialog 的信号）：

```cpp
// MainWindow.h
#include <QMainWindow>
#include "LoginDialog.h" // 引入登录对话框头文件

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

// 定义槽函数（处理信号的逻辑）
private slots:
    // 处理“切换到注册页”信号
    void onSwitchToRegister();
    // 处理“登录失败”信号（参数和信号匹配）
    void onLoginFailed(QString errorMsg);
    // 处理“登录成功”信号（参数和信号匹配）
    void onLoginSuccess(QString username);

private:
    LoginDialog *_login_dlg; // 登录对话框对象
};
```

```cpp
// MainWindow.cpp
#include "MainWindow.h"
#include <QMessageBox> // 弹提示框用

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. 创建登录对话框对象
    _login_dlg = new LoginDialog(this); // 设置父对象，Qt自动回收

    // 2. 连接信号和槽（核心！）
    // 信号1：切换注册页 → 槽函数：切换StackedWidget页面
    connect(_login_dlg, &LoginDialog::switchToRegister, 
            this, &MainWindow::onSwitchToRegister);
    // 信号2：登录失败 → 槽函数：弹提示框
    connect(_login_dlg, &LoginDialog::loginFailed, 
            this, &MainWindow::onLoginFailed);
    // 信号3：登录成功 → 槽函数：提示+切换主界面
    connect(_login_dlg, &LoginDialog::loginSuccess, 
            this, &MainWindow::onLoginSuccess);

    // 显示登录对话框
    _login_dlg->show();
}

// 实现槽函数1：切换到注册页
void MainWindow::onSwitchToRegister() {
    // 假设ui->stackedWidget是页面容器，索引1是注册页
    ui->stackedWidget->setCurrentIndex(1);
}

// 实现槽函数2：登录失败提示
void MainWindow::onLoginFailed(QString errorMsg) {
    QMessageBox::warning(this, "登录失败", errorMsg);
}

// 实现槽函数3：登录成功
void MainWindow::onLoginSuccess(QString username) {
    QMessageBox::information(this, "登录成功", "欢迎 " + username);
    ui->stackedWidget->setCurrentIndex(2); // 索引2是主界面
}

MainWindow::~MainWindow() {
    // _login_dlg有父对象，无需手动delete
}
```

### 四、信号的进阶知识点（项目中常用）

#### 1. 信号的参数匹配规则（必懂，否则连接失败）

信号和槽的参数必须 “兼容”，规则：

- 槽的参数个数 ≤ 信号的参数个数；
- 槽的参数类型必须和信号的对应位置参数类型**完全一致**（或可隐式转换）。

示例：

| 信号声明                 | 合法槽声明                | 非法槽声明                |
| ------------------------ | ------------------------- | ------------------------- |
| `void sig(int, QString)` | `void slot(int)`          | `void slot(QString, int)` |
| `void sig(int, QString)` | `void slot(int, QString)` | `void slot(int, int)`     |
| `void sig()`             | `void slot()`             | `void slot(int)`          |

#### 2. 内置信号（不用自己定义，直接用）

Qt 自带的控件（按钮、输入框、定时器等）已经定义了大量信号，项目中可以直接用，不用自己写：

| 控件           | 常用内置信号           | 含义                     |
| -------------- | ---------------------- | ------------------------ |
| QPushButton    | `clicked()`            | 按钮被点击               |
| QLineEdit      | `textChanged(QString)` | 输入框文本变化           |
| QTimer         | `timeout()`            | 定时器超时               |
| QStackedWidget | `currentChanged(int)`  | 页面切换（参数是新索引） |

示例（直接用按钮的内置信号）：

```cpp
// 按钮点击后弹提示框，无需自己定义信号
QPushButton *btn = new QPushButton("点我", this);
connect(btn, &QPushButton::clicked, this, []() {
    QMessageBox::information(this, "提示", "按钮被点击了");
});
```

#### 3. 信号可以连接信号（信号转发）

一个信号可以触发另一个信号，适合复杂界面的 “信号传递”。

示例：

```cpp
// 登录对话框的switchToRegister信号 → 主窗口的forwardSignal信号
connect(_login_dlg, &LoginDialog::switchToRegister, 
        this, &MainWindow::forwardSignal);
// 主窗口的forwardSignal信号 → 注册页的initPage槽函数
connect(this, &MainWindow::forwardSignal, 
        ui->registerPage, &RegisterPage::initPage);
```

#### 4. 信号的连接类型（解决多线程 / 卡顿问题）

`connect` 有第 5 个可选参数，决定信号触发槽的方式，项目中常用 2 种：

- `Qt::AutoConnection`（默认）：自动判断，同线程直接调用，不同线程排队调用（推荐）；
- `Qt::QueuedConnection`：槽函数在接收者的线程执行（多线程场景必用，比如主线程监听子线程的 “数据加载完成” 信号）。

示例：

```cpp
// 多线程场景，强制排队调用槽函数，避免卡顿
connect(worker, &Worker::dataLoaded, 
        this, &MainWindow::showData, 
        Qt::QueuedConnection);
```

### 五、零基础避坑（项目中最容易踩的错）

1. **忘记加 `Q_OBJECT` 宏**：信号槽直接失效，编译可能无报错，但 `connect` 不生效（最常见！）；
2. **信号写了函数体**：信号只能声明，不能写 `{}`，否则编译报错；
3. **触发信号时漏写 `emit`**：虽然语法不报错，但信号不会被发出（相当于普通函数调用，Qt 不识别）；
4. **对象是空指针**：`connect` 的发送者 / 接收者是 `nullptr`，连接失败，无效果；
5. **参数类型不匹配**：比如信号传 `int`，槽接 `QString`，Qt 会在运行时输出警告（控制台能看到），槽不执行。

### 六、总结（零基础核心记忆点）

1. 信号是 `QObject` 子类的 “通知函数”，只声明、不实现、无返回值；
2. 信号用 `signals:` 定义，用 `emit` 触发；
3. 信号必须和槽通过 `connect` 绑定，才能产生效果；
4. 项目中优先用 Qt 内置信号，自定义信号只用于 “自己的业务事件”（比如登录失败、切换页面）；
5. 核心原则：信号只负责 “通知发生了什么”，槽负责 “处理这个事件”，两者解耦（不用互相包含头文件 / 调用函数）。

## 2.异常退出问题

你的代码在切换到注册页面后关闭窗口出现 “异常退出”，核心原因是 **`_login_dlg` 被重复销毁（Qt 父子机制自动回收 + 析构函数手动 `delete`），导致 “双重释放” 崩溃**。下面详细分析原因并给出修复方案：

### 一、崩溃的核心原因：`_login_dlg` 被重复销毁

当你调用 `setCentralWidget(_login_dlg)` 时，`MainWindow` 会自动将 `_login_dlg` 的父对象设置为自己（`MainWindow` 是 `_login_dlg` 的父）。

而 Qt 的 **父子对象机制** 规则是：**父对象销毁时，会自动递归销毁所有子对象**。

你的代码中：

1. `_login_dlg` 是 `MainWindow` 的子对象（因为 `setCentralWidget` 隐式设置了父）；
2. `MainWindow` 析构时，会 **自动销毁 `_login_dlg`**；
3. 你又在 `MainWindow` 的析构函数中手动 `delete _login_dlg`；

→ 这就导致 `_login_dlg` 被 **销毁两次**（双重释放），触发内存错误，程序异常退出。

### 二、额外隐患：`_reg_dlg` 未设置父对象（可能内存泄漏）

`_reg_dlg` 是你手动 `new` 的，但没有设置父对象（也没被 `setCentralWidget` 管理），如果切换到注册页面后，`_reg_dlg` 会成为 “无父对象的孤儿”：

- 若未手动 `delete`，会内存泄漏；
- 若手动 `delete`，又可能和其他逻辑冲突。

### 三、修复方案：遵循 Qt 父子机制，避免手动重复销毁

#### 方案 1：移除析构函数中对 `_login_dlg` 的手动 `delete`

因为 `_login_dlg` 是 `MainWindow` 的子对象，`MainWindow` 析构时会自动销毁它，无需手动 `delete`。

同时，给 `_reg_dlg` 设置父对象（让 Qt 自动回收）：

cpp



运行









```cpp
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog();
    setCentralWidget(_login_dlg); // _login_dlg的父自动设为MainWindow
    
    connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::SlotSwitchReg);
    // 给_reg_dlg设置父对象（MainWindow），让MainWindow析构时自动回收
    _reg_dlg = new RegisterDialog(this); 
}

MainWindow::~MainWindow()
{
    // 移除对_login_dlg的手动delete（父会自动销毁）
    // 移除对_reg_dlg的手动delete（父会自动销毁）
    delete ui;
}
```

#### 方案 2：若坚持手动管理，需解除父子关系

如果必须在析构函数中手动 `delete`，需要先解除 `_login_dlg` 与 `MainWindow` 的父子关系：

cpp



运行









```cpp
MainWindow::~MainWindow()
{
    // 先解除_login_dlg的父关系，避免MainWindow自动销毁它
    _login_dlg->setParent(nullptr);
    if(_login_dlg){
        delete _login_dlg;
        _login_dlg=nullptr;
    }
    if(_reg_dlg){
        delete  _reg_dlg;
        _reg_dlg=nullptr;
    }
    delete ui;
}
```

但 **方案 1 更符合 Qt 规范**，推荐优先使用。

### 四、切换注册页面的逻辑优化（避免 `_reg_dlg` 成为孤儿）

当前代码中 `SlotSwitchReg` 函数（切换注册页面）可能是直接替换中央部件，导致 `_login_dlg` 被销毁，建议改用 `QStackedWidget` 管理页面（页面隐藏不销毁）：

cpp



运行









```cpp
// 1. 在MainWindow的UI中添加QStackedWidget（命名为stackedWidget）
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _login_dlg = new LoginDialog();
    _reg_dlg = new RegisterDialog();
    
    // 把登录/注册页面添加到StackedWidget
    ui->stackedWidget->addWidget(_login_dlg);
    ui->stackedWidget->addWidget(_reg_dlg);
    
    // 设置StackedWidget为主窗口中央部件
    setCentralWidget(ui->stackedWidget);
    // 默认显示登录页
    ui->stackedWidget->setCurrentIndex(0);

    connect(_login_dlg,&LoginDialog::switchRegister,this,[=](){
        // 切换到注册页（页面隐藏不销毁）
        ui->stackedWidget->setCurrentIndex(1);
    });
}

// 析构函数无需手动delete（StackedWidget是父，会自动回收_login_dlg/_reg_dlg）
MainWindow::~MainWindow()
{
    delete ui;
}
```

### 五、总结

崩溃的直接原因是 **`_login_dlg` 被父子机制自动回收 + 析构函数手动 `delete`，导致双重释放**。修复的核心是：**遵循 Qt 父子机制，让父对象自动管理子对象的销毁，避免手动重复 `delete`**。

按方案 1 修复后，程序关闭时不会再出现异常退出。要不要我帮你整理一份 **Qt 内存管理的避坑清单**？

## 3.奇异递归模板

CRTP 是 **Curiously Recurring Template Pattern** 的缩写，中文常译作「奇异递归模板模式」，是 C++ 中一种非常经典的**模板元编程技巧**—— 核心特征是：**一个类继承自以自身为模板参数的模板类**（看似 “递归”，实则是编译期的静态多态设计）。

你之前问的单例模板类（`Singleton<T>`）其实就是 CRTP 的典型应用（`class MyClass : public Singleton<MyClass>`），下面从「定义、原理、用途、示例」四个维度讲清楚 CRTP。

### 一、CRTP 核心定义

先看最基础的 CRTP 结构，一眼就能看懂它的 “奇异递归”：

```cpp
// 模板类（基类）
template <typename Derived>
class Base {
public:
    // 基类可以调用派生类的方法/访问派生类的成员
    void interface() {
        // 静态转换：将基类this指针转为派生类指针（编译期完成）
        static_cast<Derived*>(this)->implementation();
    }
};

// 派生类：继承以自身为模板参数的基类
class Derived : public Base<Derived> {
public:
    // 派生类实现具体逻辑
    void implementation() {
        std::cout << "Derived 的具体实现" << std::endl;
    }
};
```

关键特征：

- 基类是模板类，模板参数 `Derived` 是**最终的派生类**；
- 派生类继承基类时，把自己作为模板参数传给基类（`Base<Derived>`）；
- 基类通过 `static_cast<Derived*>(this)` 调用派生类的方法，实现「静态多态」。

### 二、CRTP 核心原理：编译期静态多态

C++ 传统的多态（虚函数）是**运行时多态**：通过虚函数表（vtable）在运行时找到具体的函数实现，有一定运行时开销；

CRTP 实现的是**编译期静态多态**：编译器在编译时就确定要调用的派生类方法，没有运行时开销，是 “零成本抽象”。

举个对比例子：

#### 1. 传统虚函数（运行时多态）

```cpp
class Base {
public:
    virtual void func() = 0; // 虚函数
    void call() { func(); }  // 运行时才知道调用哪个子类的func
};

class Derived : public Base {
public:
    void func() override { std::cout << "Derived" << std::endl; }
};
```

#### 2. CRTP（编译期多态）

```cpp
template <typename D>
class Base {
public:
    void call() {
        // 编译期就确定调用 D::func()，无运行时开销
        static_cast<D*>(this)->func();
    }
};

class Derived : public Base<Derived> {
public:
    void func() { std::cout << "Derived" << std::endl; }
};
```

### 三、CRTP 的典型用途

CRTP 是 C++ 模板元编程的基础技巧，常见场景有：

#### 1. 实现通用的单例模板（你之前的代码）

这是最常见的 CRTP 应用之一：

cpp



运行









```cpp
template <typename T>
class Singleton {
public:
    static T& GetInstance() {
        static T instance; // 懒汉式，编译期保证线程安全（C++11后）
        return instance;
    }
protected:
    Singleton() = default;
    ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

// 派生类继承 Singleton，自身作为模板参数
class MyManager : public Singleton<MyManager> {
    // 让 Singleton 能访问构造函数
    friend class Singleton<MyManager>;
private:
    MyManager() = default; // 私有构造，禁止外部创建
public:
    void doWork() { std::cout << "MyManager 工作中" << std::endl; }
};

// 使用：MyManager::GetInstance().doWork();
```

#### 2. 实现 “混入（Mixin）” 功能

给不同类批量添加通用功能（比如计数、打印、序列化），无需重复写代码：

cpp



运行









```cpp
// 通用的计数 Mixin：统计对象创建数量
template <typename Derived>
class Counter {
public:
    static int count() { return _count; }
protected:
    Counter() { _count++; }
    ~Counter() { _count--; }
private:
    static int _count;
};
// 静态成员初始化
template <typename D>
int Counter<D>::_count = 0;

// 给 User 类添加计数功能
class User : public Counter<User> {};
// 给 Product 类添加计数功能
class Product : public Counter<Product> {};

// 使用
int main() {
    User u1, u2;
    Product p1;
    std::cout << User::count() << std::endl;   // 输出 2
    std::cout << Product::count() << std::endl;// 输出 1
    return 0;
}
```

#### 3. 静态多态接口（替代虚函数）

适合对性能要求高的场景（比如游戏、嵌入式），避免虚函数的运行时开销：

cpp



运行









```cpp
// 通用的图形基类（CRTP）
template <typename Shape>
class Graphic {
public:
    void draw() {
        // 编译期绑定到具体 Shape 的 drawImpl
        static_cast<Shape*>(this)->drawImpl();
    }
};

// 圆形：实现具体的绘制逻辑
class Circle : public Graphic<Circle> {
public:
    void drawImpl() { std::cout << "画圆形" << std::endl; }
};

// 矩形：实现具体的绘制逻辑
class Rectangle : public Graphic<Rectangle> {
public:
    void drawImpl() { std::cout << "画矩形" << std::endl; }
};

// 使用
template <typename Shape>
void render(Graphic<Shape>& shape) {
    shape.draw(); // 编译期确定调用哪个 drawImpl
}

int main() {
    Circle c;
    Rectangle r;
    render(c); // 输出 画圆形
    render(r); // 输出 画矩形
    return 0;
}
```

### 四、CRTP 的关键注意事项

1. **编译期确定类型**：CRTP 的所有逻辑都在编译期完成，无法像虚函数那样 “运行时动态绑定”；

2. **避免派生类未实现方法**：如果基类调用了派生类的 `xxxImpl`，但派生类没实现，编译时会直接报错（而非运行时）；

3. **模板参数必须是最终派生类**：不能中间层继承（比如 `A->B< A >->C< B >` 会出错），必须是 `C->B< C >`；

4. 与虚函数的选择

   ：

   - 追求性能、类型固定 → 用 CRTP；
   - 需要运行时动态切换类型（比如基类指针指向不同子类）→ 用虚函数。

### 总结

CRTP 本质是「利用模板的编译期类型推导，实现静态多态和代码复用」—— 核心就是 “派生类继承以自身为参数的模板基类”，看似递归，实则是 C++ 模板元编程的巧妙技巧，尤其适合写通用模板（比如你之前的单例）、高性能接口、批量添加通用功能的场景。

你之前问的单例模板就是 CRTP 的典型应用：`MyClass` 继承 `Singleton<MyClass>`，`Singleton` 模板通过模板参数 `T` 操作 `MyClass` 的实例，实现通用的单例逻辑。