# VSCode 调试C++ 单文件

### 1. 配置构建任务（tasks.json）

点击**Terminal->Configure Default Build Task ->c++**  生成tasks.json 或者按照下面操作生成。

这个文件用于定义如何编译你的代码。

1. 在 VS Code 中，按 `Ctrl+Shift+P` 打开命令面板。
2. 输入并选择 **Tasks: Configure Task**。
3. 选择 **Create tasks.json file from template**，然后选择 **Others**。
4. 编辑生成的 `tasks.json` 文件，内容如下（以 g++ 为例）：

```json
{
	"version": "2.0.0",
	"tasks": [
		{
			"type": "cppbuild",
			"label": "C/C++: g++ 生成活动文件",
			"command": "/usr/bin/g++",
			"args": [
				"-fdiagnostics-color=always",
				"-g",
				"${file}",
				"-o",
				"${fileDirname}/${fileBasenameNoExtension}"
			],
			"options": {
				"cwd": "${fileDirname}"
			},
			"problemMatcher": [
				"$gcc"
			],
			"group": {
				"kind": "build",
				"isDefault": true
			},
			"detail": "编译器: /usr/bin/g++"
		}
	]
}
```

> 注意：`-g` 参数是必须的，它会在可执行文件中包含调试信息。

------

### 2. 配置调试器（launch.json）

点击**Run->Add Configuration** 生成launch.json  或者按照如下生成

这个文件用于启动调试会话。

1. 在 VS Code 中，点击左侧的“运行和调试”图标（或按 `Ctrl+Shift+D`）。
2. 点击 **“创建一个 launch.json 文件”**。
3. 选择 **C++ (GDB/LLDB)** 或 **C++ (Windows)**，取决于你的平台。
4. 编辑 `launch.json`，内容如下：

```json
{
    // 使用 IntelliSense 了解相关属性。 
    // 悬停以查看现有属性的描述。
    // 欲了解更多信息，请访问: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": []
}
```

------

### 3. 开始调试

1. 打开你的 `main.cpp` 文件。
2. 在你想暂停的代码行左侧点击，设置一个**断点**（红点）。
3. 按 `F5` 或点击左侧“运行和调试”面板中的“运行”按钮。
4. VS Code 会自动：
   - 执行 `tasks.json` 中的构建任务（编译代码）。
   - 启动 `launch.json` 配置的调试器。
   - 在断点处暂停，你可以查看变量、调用栈、单步执行等。

------

# VSCode 调试C++ 多文件

多个 `.cpp` 文件的项目，不能直接使用默认配置，需要修改！

默认的 `tasks.json` 是这样的：

```
"args": [
    "-g",
    "${file}",                  // ← 只编译当前打开的文件！
    "-o",
    "${fileDirname}/${fileBasenameNoExtension}"
]
```

👉 它只编译 **当前正在编辑的文件**（`${file}`），比如你打开的是 `utils.cpp`，它就只编译 `utils.cpp`，不会链接 `main.cpp`，导致：

- 编译失败（找不到 `main`）
- 或者生成的可执行文件不完整

------

小到中型项目 —— 手动在 `tasks.json` 中列出所有 `.cpp` 文件（简单直接）

### 1. 修改 `tasks.json`

```
{
    "version": "2.0.0",
    "tasks": [
        {
            "type": "cppbuild",
            "label": "C/C++: g++ build all files",
            "command": "/usr/bin/g++",
            "args": [
                "-fdiagnostics-color=always",
                "-g",                             // 调试信息
                "-std=c++17",                     // C++ 标准
                "main.cpp",                       // 所有源文件列在这里
                "utils.cpp",
                "math_utils.cpp",
                "-o",
                "myapp"                           // 输出可执行文件名
            ],
            "options": {
                "cwd": "${workspaceFolder}"      // 在项目根目录运行
            },
            "problemMatcher": ["$gcc"],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "detail": "Building multi-file project"
        }
    ]
}
```

> 💡 注意：
>
> - 把所有 `.cpp` 文件都列在 `args` 中
> - `"cwd": "${workspaceFolder}"` 确保路径正确
> - 输出文件名可以自定义（如 `myapp`）

------

### 2. `launch.json` 配置

（只需改 `program` 路径）

```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug Multi-File C++",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/myapp",   // 必须和 tasks 中 -o 指定的一致
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "C/C++: g++ build all files"  // 必须和 tasks 的 label 一致！
        }
    ]
}
```

------

### 3. 开始调试

1. 打开任意一个 `.cpp` 文件（如 `main.cpp`）
2. 按 `Ctrl+Shift+B`   构建任务→ 选择 `C/C++: g++ build all files` 构建
3. 按 `F5` 启动调试，自动编译 + 调试

------



# VSCode 调试C++ make

核心思路

1. `Makefile` 负责**编译**（必须包含 `-g` 调试信息）。
2. VS Code 的 `tasks.json` 调用 `make` 命令来构建项目。
3. VS Code 的 `launch.json` 启动 GDB 调试器，加载可执行文件并运行。
4. 按 `F5` 实现：自动编译 + 启动调试。

------

示例项目结构

```
my_project/
├── Makefile
├── src/
│   └── main.cpp
└── build/
    └── myapp   # 编译后的可执行文件
```

------

## 1.确保 Makefile 支持 Debug 模式

 `Makefile` 必须生成带有调试符号的可执行文件（即使用 `-g` 编译选项），否则无法查看变量、断点可能失效。

推荐的 Makefile 示例：

```
# 编译器
CXX = g++
# 编译选项
CXXFLAGS = -std=c++17 -g -O0   # -g: 生成调试信息, -O0: 关闭优化
# 目标文件输出目录
BUILD_DIR = build
# 可执行文件名
TARGET = $(BUILD_DIR)/myapp

# 源文件
SRCS = src/main.cpp
# 生成对应的目标文件（.o）
OBJS = $(SRCS:.cpp=.o)
# 确保 build 目录存在
$(shell mkdir -p $(BUILD_DIR))

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $@ 

# 编译 .cpp 文件为 .o 文件，放入 build/
$(BUILD_DIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(BUILD_DIR)/*.o $(TARGET)

# 伪目标
.PHONY: all clean
```

> 🔥 关键点：
>
> - `-g`：必须加，否则没有调试符号！
> - `-O0`：关闭优化，防止变量被优化掉或断点跳转异常
> - 输出路径明确（如 `build/myapp`），方便 `launch.json` 引用

------

## 第二步：配置 VS Code 的 tasks.json（用于 make 构建）

作用：按 `Ctrl+Shift+B` 或调试前自动调用 `make` 编译项目。

1. 打开命令面板：`Ctrl+Shift+P`
2. 输入：**Tasks: Configure Task**
3. 选择：**Create tasks.json file from template** → **Others**

编辑 `.vscode/tasks.json`：

Json

编辑





```
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "make-build",
            "type": "shell",
            "command": "make",
            "args": [],
            "group": {
                "kind": "build",
                "isDefault": true
            },
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "shared"
            },
            "options": {
                "cwd": "${workspaceFolder}"
            },
            "problemMatcher": {
                "owner": "cpp",
                "fileLocation": ["relative", "${workspaceFolder}"],
                "pattern": {
                    "regexp": "^(.*):(\\d+):(\\d+):\\s+(warning|error):\\s+(.*)$",
                    "file": 1,
                    "line": 2,
                    "column": 3,
                    "severity": 4,
                    "message": 5
                }
            }
        }
    ]
}
```

> 💡 这样配置后：
>
> - `Ctrl+Shift+B` 会触发 `make`
> - 错误会高亮显示在“问题”面板中

------

## 第三步：配置 launch.json（启动调试）

1. 打开“运行和调试”视图（左侧图标或 `Ctrl+Shift+D`）
2. 点击“创建 launch.json”
3. 选择 **C/C++: (gdb) Launch**

修改 `.vscode/launch.json`：

Json

编辑





```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug with Makefile",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/myapp",  // 必须与 Makefile 输出一致
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing for gdb",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "make-build",  // 调试前自动执行 make
            "miDebuggerPath": "/usr/bin/gdb"
        }
    ]
}
```

> 🔥 关键字段说明：
>
> - `"program"`：必须指向 `Makefile` 编译出的可执行文件路径
> - `"preLaunchTask"`：必须与 `tasks.json` 中的 `label` 一致，确保先编译再调试

------

## 第四步：开始调试

1. 在 `main.cpp` 中设置一个断点（点击行号左侧）
2. 按 `F5` 或点击 ▶️ “调试”
3. VS Code 会自动：
   - 运行 `make` 编译代码
   - 启动 GDB 加载 `build/myapp`
   - 在断点处暂停
4. 你可以：
   - 查看变量值
   - F10 单步跳过
   - F11 单步进入函数
   - 查看调用栈

------

## 🛠️ 常见问题与解决

| 问题                           | 原因                       | 解决方案                                    |
| ------------------------------ | -------------------------- | ------------------------------------------- |
| 断点显示为空心？               | 可执行文件无调试信息       | 检查 `Makefile` 是否有 `-g`                 |
| 提示 "Unable to start program" | `program` 路径错误         | 检查 `launch.json` 中路径是否正确           |
| 变量显示 `<optimized out>`     | 编译时开了优化（如 `-O2`） | 使用 `-O0` 关闭优化                         |
| `make` 找不到                  | 终端环境问题               | 确保 `make` 已安装：`sudo apt install make` |
| 多文件编译失败                 | `Makefile` 规则不完整      | 确保 `.o` 文件生成规则正确                  |

------

## ✅ 最佳实践建议

1. **始终使用 `-g -O0` 进行调试构建**
2. **分离构建目录**（如 `build/`），避免污染源码
3. **可以扩展 Makefile 支持多模式**：

Makefile

编辑





```
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -O0
else
    CXXFLAGS += -O2
endif
```

然后调试时用：`make DEBUG=1`

------

## 总结

即使你没有使用 CMake 或 XMake，只要你的 `Makefile` 正确生成了带调试信息的可执行文件，VS Code 就能完美调试：

🔧 **三步走**：

1. `Makefile` 加 `-g -O0`
2. `tasks.json` 配 `make` 构建任务
3. `launch.json` 配 `gdb` 调试，指定正确 `program` 路径 + `preLaunchTask`

✅ 然后按 `F5`，享受现代 IDE 级别的调试体验！

如果你愿意，可以把你的 `Makefile` 内容贴出来，我可以帮你检查是否适合调试，并生成完整的 `launch.json` 和 `tasks.json`。

# VSCode 调试C++ cmake

**使用 CMake + CMake Tools 扩展调试**

### 1. 项目结构示例

```
my_project/
├── CMakeLists.txt
├── src/
│   └── main.cpp
```

`CMakeLists.txt` 示例：

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)  # 关键：必须是 Debug 模式
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")  # 确保带调试信息

add_executable(myapp src/main.cpp)
```

> ⚠️ 注意：`-g` 是生成调试符号的关键，`-O0` 关闭优化，避免变量被优化掉导致无法查看。

------

### 2. 安装并配置 "CMake Tools" 扩展

- 在 VS Code 扩展市场中搜索并安装 **CMake Tools**。
- 打开你的项目文件夹后，VS Code 会自动检测到 `CMakeLists.txt`。

------

### 3. 配置和构建项目

#### 步骤：

1. 按 `Ctrl+Shift+P` 打开命令面板。

2. 输入并选择：

   CMake: Select a Kit

   - 选择编译器，如 `GCC 11.4.0 x86_64-linux-gnu`

3. 再次输入：

   CMake: Select Variant

   - 选择 `Debug`（这会设置 `CMAKE_BUILD_TYPE=Debug`）

4. 输入：

   CMake: Configure

   - 生成 `build/` 目录和 Makefile

5. 输入：

   CMake: Build

   - 编译生成可执行文件（如 `myapp`）

默认会在 `build/` 目录下生成可执行文件。

------

### 4. 配置调试启动（launch.json）

按 `F5` 或点击“运行和调试” → “创建 launch.json”，选择 **C++ (GDB) Launcher**

修改 `launch.json` 如下：

Json

编辑





```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug myapp",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/myapp",  // 指向构建出的可执行文件
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "cmake-build"  // 可选：自动构建
        }
    ]
}
```

> 💡 提示：你可以通过 CMake Tools 自动生成 `.vscode/settings.json` 来指定构建目录。

------

### 5. 开始调试

- 设置断点
- 按 `F5` 启动调试
- 程序会在断点处暂停，支持：
  - 查看变量值
  - 单步执行（F10/F11）
  - 调用栈查看
  - 表达式求值

------

# VSCode 调试C++ xmake

## 🧩 方法二：使用 XMake 调试

XMake 是一个现代化的 C/C++ 构建系统，对 VS Code 支持良好。

### 1. 安装 XMake

Bash

编辑





```
curl -fsSL https://xmake.io/shget.text | bash
```

验证安装：

Bash

编辑





```
xmake --version
```

------

### 2. 创建或已有 XMake 项目

Bash

编辑





```
xmake create -l c++ my_xmake_proj
cd my_xmake_proj
```

`xmake.lua` 示例：

Lua

编辑





```
add_rules("mode.debug", "mode.release")

target("myapp")
    set_kind("binary")
    set_languages("c++17")
    add_files("src/*.cpp")
    if is_mode("debug") then
        set_symbols("debug")      -- 生成调试符号
        set_optimize("none")      -- 不优化
    end
```

------

### 3. 在 VS Code 中配置调试

#### 方式一：手动配置（推荐初学者）

1. 使用 XMake 构建 Debug 版本：

Bash

编辑





```
xmake f -m debug   # 配置为 debug 模式
xmake              # 构建项目
```

1. 生成 `launch.json`，内容如下：

Json

编辑





```
{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "Debug with XMake",
            "type": "cppdbg",
            "request": "launch",
            "program": "${workspaceFolder}/build/linux/x86_64/debug/myapp",
            "args": [],
            "stopAtEntry": false,
            "cwd": "${workspaceFolder}",
            "environment": [],
            "externalConsole": false,
            "MIMode": "gdb",
            "setupCommands": [
                {
                    "description": "Enable pretty-printing",
                    "text": "-enable-pretty-printing",
                    "ignoreFailures": true
                }
            ],
            "preLaunchTask": "xmake-build"
        }
    ]
}
```

1. （可选）添加 `tasks.json` 自动构建：

Json

编辑





```
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "xmake-build",
            "type": "shell",
            "command": "xmake",
            "args": [],
            "group": "build",
            "presentation": {
                "echo": true,
                "reveal": "always"
            },
            "problemMatcher": []
        }
    ]
}
```

#### 方式二：使用 XMake 官方插件（高级）

- 安装 VS Code 插件：**XMake**（由 tboox 提供）
- 支持直接在状态栏切换模式、构建、运行、调试
- 更深度集成，适合长期使用 XMake 的项目