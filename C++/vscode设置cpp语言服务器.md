# vscode配置语言服务器

1. 安装语言服务器

```
sudo apt install clangd
```

2. 安装gcc和g++

```
sudo apt install gcc g++
```

一般来说简单的程序只安装语言编译器和语言服务器就可以了

3. 创建.vscode/settings.json   或者用户settings.json

```json
{
  "C_Cpp.intelliSenseEngine": "Disabled",
  "clangd.path": "/usr/bin/clangd",
}
```

4. 创建.clangd (不是必须)

```
CompileFlags:
  Add: 
    - "-std=c++17"
    - "-I/usr/include"
    - "-I/usr/local/include"
    - "-I${workspaceFolder}"
```

5. cmake  make编译的时候生成 `compile_commands.json` clangd语言服务器会自动识别

6. 告诉 clangd 该文件的路径有两种方式（选其一即可）：

   **方式 1：创建 .clangd 配置文件**（推荐，项目级配置）
   在项目根目录创建 `.clangd` 文件，内容：

   ```yaml
   CompileFlags:
     CompilationDatabase: build  # 指向 compile_commands.json 所在目录（如 build）
   ```

   **方式 2：在 VS Code settings.json 中配置**（全局或工作区）
   打开 `settings.json`（方法：`Ctrl+,` → 点击 `{}` 图标），添加：

   ```json
   {
       "clangd.arguments": ["--compile-commands-dir=build"]  # 路径与构建目录一致
   }
   或者
   {
       "clangd.arguments": ["--compile-commands-dir=${workspaceFolder}/build"]  
   }
   ```

7. 禁用冲突插件（确保 clangd 独占语言服务）避免 Microsoft C/C++ 扩展与 clangd 冲突，需禁用其语言服务：

   打开 VS Code `settings.json`，添加以下配置：

   ```json
   {
       // 禁用 Microsoft C/C++ 扩展的语言服务
       "C_Cpp.intelliSenseEngine": "disabled",
       "C_Cpp.enable": false,
       // 可选：关闭 clangd 的冲突检测提示（避免弹窗）
       "clangd.detectExtensionConflicts": false
   }
   ```

8. 验证配置是否生效

**检查 clangd 运行状态**
打开 VS Code 输出面板（`Ctrl+Shift+U`），选择 **`clangd`**，日志中应显示：

```plaintext
I[12:34:56] Loaded compilation database from .../build/compile_commands.json
```

表示成功加载编译数据库。

**测试语言服务功能**
在 C++ 源文件中：

- 输入代码（如 `std::vector<int> v; v.`），应有 clangd 提供的补全提示
- 右键点击函数 / 类名，选择「转到定义」，应能正确跳转
- 若有语法错误，会实时显示红色波浪线（clangd 语法检查）



# vscode &cmake调试

要调试c++程序，必须设置Debug编译模式，即显式设置构建类型

- 通过命令行：

  ```bash
  cmake -DCMAKE_BUILD_TYPE=Release ..  # 设置为 Release
  cmake -DCMAKE_BUILD_TYPE=Debug ..    # 设置为 Debug
  ```

- 在 `CMakeLists.txt` 中强制设置（不推荐，但可行）：

  ```cmake
  set(CMAKE_BUILD_TYPE Debug)  # 强制使用 Debug 模式
  ```

.vscode下配置launch.json和task.json () (本来这是自动生成的，可是不知道为啥，我不能 后续看看)

launch：

```json
{
    "version": "2.0.0",
    "configurations": [
        {
            "name": "CMake Debug",
            "type": "cppdbg",
            "request": "launch",
            // 替换为你实际的可执行文件路径（build 目录下的输出）
            "program": "${workspaceFolder}/build/app",  
            "args": [],
            "cwd": "${workspaceFolder}",
            "console": "integratedTerminal",
            // 调试前先执行构建（关联 CMake 构建）
            "preLaunchTask": "build",  
            "miDebuggerPath": "/usr/bin/gdb"  // 或 lldb，根据调试器
        }
    ]
}
```

tasks：

```json
{
    "tasks": [
        {
            "label": "build",  
            "type": "shell",
            "command": "cmake --build build --config Debug",  
            "group": {
                "kind": "build",
                "isDefault": true
            }
        }
    ],
    "version": "2.0.0"
}
```

