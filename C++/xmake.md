# xmake快速入门

## 1. xmake 是什么？

- **xmake** = 构建系统 + 包管理器。
- 和 **CMake** 一样，都是写配置文件生成编译规则。
- 不同的是：
  - **CMake** 只负责生成工程（Makefile / Ninja / VS 工程），然后还得自己调用 `make`、找包。
  - **xmake** 是 **一站式**，写一个 `xmake.lua` 就能直接 `xmake` 编译。
  - 自带 **包管理器**（有点像 CMake + vcpkg/conan 的组合体，但集成在一起）。

------

## 2. 最简单的项目

假设你有一个 `main.cpp`：

```
#include <iostream>
int main() {
    std::cout << "Hello, xmake!" << std::endl;
    return 0;
}
```

写一个 `xmake.lua`：

```
add_rules("mode.debug", "mode.release") -- 支持debug/release

target("hello")       -- 定义一个目标叫 hello
    set_kind("binary")-- 目标类型：可执行程序
    add_files("main.cpp") -- 源文件
```

然后直接：

```
xmake        # 默认编译 release
xmake run    # 运行程序
xmake -m debug # 编译debug版
xmake run -m debug
```

------

## 3. 自动下载依赖（xmake 的强大之处 🚀）

比如你要用 `fmt` 库（格式化字符串），在 CMake 里你得：

1. 下载 fmt
2. `find_package(fmt REQUIRED)`
3. 处理 cmake 的各种变量

在 **xmake** 里，只需要一行：

```
add_requires("fmt")          -- 声明依赖
target("demo")
    set_kind("binary")
    add_files("main.cpp")
    add_packages("fmt")      -- 链接依赖
```

如果本地没有 `fmt`，xmake 会自动去拉包、编译、缓存，然后直接用。
 下次用就走缓存，不需要重复下载。

支持的包仓库很多（xrepo），常见的 Boost、OpenCV、glog、spdlog、protobuf、openssl、sqlite3……都能直接拉。
 👉 类似于 **Node.js 的 npm** 或者 **Rust 的 cargo**。

------

## 4. 常用命令

- `xmake f -m debug` : 切换到 debug
- `xmake f -m release`: 切换到 release
- `xmake` : 编译
- `xmake run` : 运行目标程序
- `xmake package` : 打包产物
- `xmake install` : 安装到系统路径
- `xmake require --info fmt` : 查看某个包的信息

------

## 5. 为什么比 CMake 省心？

- **CMake**：写 `CMakeLists.txt`，再 `cmake .. && make`，还得处理包依赖（apt/yum/conan/vcpkg）。
- **xmake**：写 `xmake.lua`，一条 `xmake` 就搞定，包管理内置。

等于是：

- **CMake = 构建系统 + 第三方包管理器**
- **xmake = 构建系统 + 内置包管理器 + 运行器**

------

⚡总结：

- 写项目配置更简洁（Lua 语法，比 CMake 的 DSL 好用）。
- 自动解决依赖，适合快速搭项目。
- 切换模式和运行都更方便。

------

# xmake大型项目示例——LSM

```lua
-- 定义项目
set_project("toni-lsm")
set_version("0.0.1")
set_languages("c++20")

add_rules("mode.debug", "mode.release")

add_repositories("local-repo build")

add_requires("gtest")
add_requires("muduo")
add_requires("pybind11")
add_requires("spdlog", { system = false })
add_requires("toml11", { system = false })

if is_mode("debug") then
    add_defines("LSM_DEBUG")
end

target("logger")
    set_kind("static")  -- 生成静态库
    add_files("src/logger/*.cpp")
    add_packages("spdlog")
    add_includedirs("include", {public = true})
    
target("config")
    set_kind("static")  -- 生成静态库
    add_files("src/config/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("utils")
    set_kind("static")  -- 生成静态库
    add_files("src/utils/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("iterator")
    set_kind("static")  -- 生成静态库
    add_files("src/iterator/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("skiplist")
    set_kind("static")  -- 生成静态库
    add_files("src/skiplist/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("memtable")
    set_kind("static")  -- 生成静态库
    add_deps("skiplist","iterator", "config")
    add_deps("sst")
    add_packages("toml11", "spdlog")
    add_files("src/memtable/*.cpp")
    add_includedirs("include", {public = true})

target("block")
    set_kind("static")  -- 生成静态库
    add_deps("config")
    add_files("src/block/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("sst")
    set_kind("static")  -- 生成静态库
    add_deps("block", "utils", "iterator")
    add_files("src/sst/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("wal")
    set_kind("static")  -- 生成静态库
    add_deps("sst", "memtable")
    add_files("src/wal/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("lsm")
    set_kind("static")  -- 生成静态库
    add_deps("sst", "memtable", "wal", "logger")
    add_files("src/lsm/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

target("redis")
    set_kind("static")  -- 生成静态库
    add_deps("lsm")
    add_files("src/redis_wrapper/*.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})

-- 定义动态链接库目标
target("lsm_shared")
    set_kind("shared")
    add_files("src/**.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})
    set_targetdir("$(builddir)/lib")

    -- 安装头文件和动态链接库
    on_install(function (target)
        os.cp("include", path.join(target:installdir(), "include/toni-lsm"))
        os.cp(target:targetfile(), path.join(target:installdir(), "lib"))
    end)

-- 定义测试
target("test_config")
    set_kind("binary")  -- 生成可执行文件
    set_group("tests")
    add_files("test/test_config.cpp")
    add_deps("logger", "config")  -- 依赖skiplist库
    add_packages("gtest")  -- 添加gtest包
    add_packages("toml11", "spdlog")

target("test_skiplist")
    set_kind("binary")  -- 生成可执行文件
    set_group("tests")
    add_files("test/test_skiplist.cpp")
    add_deps("logger", "skiplist")  -- 依赖skiplist库
    add_packages("gtest")  -- 添加gtest包
    add_packages("toml11", "spdlog")

target("test_memtable")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_memtable.cpp")
    add_deps("logger", "memtable")  -- 如果memtable是独立的target，这里需要添加对应的依赖
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_block")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_block.cpp")
    add_deps("logger", "block")  -- 如果memtable是独立的target，这里需要添加对应的依赖
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_blockmeta")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_blockmeta.cpp")
    add_deps("logger", "block")  -- 如果memtable是独立的target，这里需要添加对应的依赖
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_utils")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_utils.cpp")
    add_deps("logger", "utils")
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_sst")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_sst.cpp")
    add_deps("logger", "sst")
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_lsm")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_lsm.cpp")
    add_deps("logger", "lsm", "memtable", "iterator")  -- Added memtable and iterator dependencies
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_block_cache")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_block_cache.cpp")
    add_deps("logger", "block")
    add_includedirs("include")
    add_packages("gtest")
    add_packages("toml11", "spdlog")

target("test_compact")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_compact.cpp")
    add_deps("logger", "lsm", "memtable", "iterator")  -- Added memtable and iterator dependencies
    add_packages("gtest")
    add_packages("toml11", "spdlog")
    add_includedirs("include")

target("test_redis")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_redis.cpp")
    add_deps("logger", "redis", "memtable", "iterator")  -- Added memtable and iterator dependencies
    add_includedirs("include")
    add_packages("gtest")
    add_packages("toml11", "spdlog")

target("test_wal")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_wal.cpp")
    add_deps("logger", "wal")  -- Added memtable and iterator dependencies
    add_includedirs("include")
    add_packages("gtest")
    add_packages("toml11", "spdlog")

-- 定义 示例
target("example")
    set_kind("binary")
    add_files("example/main.cpp")
    add_deps("lsm_shared")
    add_includedirs("include", {public = true})
    -- set_targetdir("$(builddir)/bin")

-- 定义 debug 示例
target("debug")
    set_kind("binary")
    add_files("example/debug.cpp")
    add_deps("lsm_shared")
    add_includedirs("include", {public = true})
    set_targetdir("$(builddir)/bin")

-- 定义server
target("server")
    set_kind("binary")
    add_files("server/src/*.cpp")
    add_deps("redis")
    add_includedirs("include", {public = true})
    add_packages("muduo")
    set_targetdir("$(builddir)/bin")

target("lsm_pybind")
    set_kind("shared")
    add_files("sdk/lsm_pybind.cpp")
    add_packages("pybind11")
    add_deps("lsm_shared")
    add_includedirs("include", {public = true})
    set_targetdir("$(builddir)/lib")
    set_filename("lsm_pybind.so")  -- 确保生成的文件名为 lsm_pybind.so
    add_ldflags("-Wl,-rpath,$ORIGIN")
    add_defines("TONILSM_EXPORT=__attribute__((visibility(\"default\")))")
    add_cxxflags("-fvisibility=hidden")  -- 隐藏非导出符号

task("run-all-tests")
    set_category("plugin")
    set_menu {
        usage = "xmake run-all-tests",
        description = "Build and run all test binaries (targets starting with 'test_')"
    }

    on_run(function ()
        import("core.project.project")

        local targets = project.targets()
        local test_targets = {}

        for name, _ in pairs(targets) do
            if name:startswith("test_") then
                table.insert(test_targets, name)
            end
        end

        table.sort(test_targets)

        if #test_targets == 0 then
            print("\27[33m[Warning] No test targets found.\27[0m")
            return
        end

        for _, name in ipairs(test_targets) do
            -- print("\27[36m>> Building\27[0m " .. name)
            -- os.execv("xmake", {"build", name})

            print("\27[32m>> Running\27[0m " .. name)
            os.execv("xmake", {"run", name})
            print("")
        end

        print("\27[32mAll tests finished.\27[0m")
    end)
```

## 1. 项目基础配置

```
set_project("toni-lsm")   -- 定义项目名
set_version("0.0.1")      -- 项目版本
set_languages("c++20")    -- 指定编译标准

add_rules("mode.debug", "mode.release")  -- 支持 Debug / Release 两种模式
```

- `set_project`：就是你的项目名。
- `set_version`：纯粹标识用，不影响编译。
- `set_languages`：这里就是 `-std=c++20`。
- `add_rules("mode.debug", "mode.release")`：启用两种构建模式，切换用 `xmake f -m debug` 或 `xmake f -m release`。

------

## 2. 包管理配置

```
add_repositories("local-repo build")

add_requires("gtest")
add_requires("muduo")
add_requires("pybind11")
add_requires("spdlog", { system = false })
add_requires("toml11", { system = false })
```

- `add_repositories("local-repo build")`：添加一个本地包仓库，可以自定义包描述。

- `add_requires("xxx")`：声明依赖，xmake 会自动下载编译。

- `system = false`：表示 **不要用系统里已有的库**，强制 xmake 自己拉源码编译。

  > 这点比 CMake 简单很多，CMake 你得 `find_package`，而 xmake 一行搞定。

------

## 3. Debug 特定设置

```
if is_mode("debug") then
    add_defines("LSM_DEBUG")
end
```

- 只有在 Debug 模式下才会定义宏 `LSM_DEBUG`。
- 等价于编译时自动加 `-DLSM_DEBUG`。
- 你可以用来打开调试日志、断言等。

------

##  4. 定义库（核心模块）

你有一堆 `target("xxx")`，比如：

```
target("logger")
    set_kind("static")  -- 静态库
    add_files("src/logger/*.cpp")
    add_packages("spdlog")
    add_includedirs("include", {public = true})
```

通用规律：

- `target("名字")`：定义一个构建目标。
- `set_kind("static")`：生成静态库（`.a`/`.lib`）。
- `set_kind("shared")`：生成动态库（`.so`/`.dll`）。
- `set_kind("binary")`：生成可执行程序。
- `add_files("xxx")`：指定源码。
- `add_includedirs("include", {public = true})`：头文件路径，`public` 表示会传播给依赖它的 target。
- `add_packages("xxx")`：链接外部依赖。
- `add_deps("xxx")`：声明依赖关系（自动处理 include 和 link）。

你这里把项目拆成了模块化的库（`logger`, `config`, `skiplist`, `sst`, `wal`, `lsm`, `redis`），然后再组合。
 👉 这跟 CMake 的 `add_library` / `target_link_libraries` 很像，但写法更短。

------

## 5. 动态库 & 安装规则

```
target("lsm_shared")
    set_kind("shared")
    add_files("src/**.cpp")
    add_packages("toml11", "spdlog")
    add_includedirs("include", {public = true})
    set_targetdir("$(builddir)/lib")

    on_install(function (target)
        os.cp("include", path.join(target:installdir(), "include/toni-lsm"))
        os.cp(target:targetfile(), path.join(target:installdir(), "lib"))
    end)
```

- `set_targetdir`：输出目录。
- `on_install`：定义安装规则（头文件复制、库复制）。
   执行 `xmake install -o /usr/local` 就能自动安装。

------

##  6. 测试（gtest）

```
target("test_skiplist")
    set_kind("binary")
    set_group("tests")
    add_files("test/test_skiplist.cpp")
    add_deps("logger", "skiplist")
    add_packages("gtest", "toml11", "spdlog")
    add_includedirs("include")
```

- 每个测试用例都建一个 target，生成一个 `test_xxx` 可执行文件。
- `set_group("tests")`：逻辑分组，方便管理。
- `add_deps`：让测试直接复用已有模块。

这就相当于 CMake 的 `add_executable` + `target_link_libraries`。

------

## 7. 示例程序

```
target("example")
    set_kind("binary")
    add_files("example/main.cpp")
    add_deps("lsm_shared")
    add_includedirs("include", {public = true})
```

这是你的 demo，可执行文件。

------

## 8. 服务器

```
target("server")
    set_kind("binary")
    add_files("server/src/*.cpp")
    add_deps("redis")
    add_includedirs("include", {public = true})
    add_packages("muduo")
    set_targetdir("$(builddir)/bin")
```

- 用了 `muduo` 网络库。
- 生成一个 `server` 可执行程序。

------

## 9. Python 绑定（pybind11）

```
target("lsm_pybind")
    set_kind("shared")
    add_files("sdk/lsm_pybind.cpp")
    add_packages("pybind11")
    add_deps("lsm_shared")
    add_includedirs("include", {public = true})
    set_targetdir("$(builddir)/lib")
    set_filename("lsm_pybind.so")
    add_ldflags("-Wl,-rpath,$ORIGIN")
    add_defines("TONILSM_EXPORT=__attribute__((visibility(\"default\")))")
    add_cxxflags("-fvisibility=hidden")
```

- 生成 `lsm_pybind.so`，Python 可直接 `import lsm_pybind`。
- 控制符号可见性，减少导出污染。

------

## 10. 自定义任务

```
task("run-all-tests")
    set_category("plugin")
    set_menu {
        usage = "xmake run-all-tests",
        description = "Build and run all test binaries (targets starting with 'test_')"
    }

    on_run(function ()
        import("core.project.project")
        local targets = project.targets()
        local test_targets = {}
        for name, _ in pairs(targets) do
            if name:startswith("test_") then
                table.insert(test_targets, name)
            end
        end
        table.sort(test_targets)

        for _, name in ipairs(test_targets) do
            print("\27[32m>> Running\27[0m " .. name)
            os.execv("xmake", {"run", name})
            print("")
        end
        print("\27[32mAll tests finished.\27[0m")
    end)
```

- 定义了一个任务 `xmake run-all-tests`。
- 自动找到所有 `test_xxx` target 并运行。
- 等价于一个迷你 test runner，类似于 CTest。

------

## 你能学到的重点

1. **模块化 target**：项目拆分成库，再组合。
2. **add_requires 自动拉依赖**：比 CMake 简单很多。
3. **多模式支持**：Debug/Release 切换方便。
4. **测试组织**：直接把 gtest 可执行程序注册为 target。
5. **跨语言绑定**：`lsm_pybind` 用 pybind11 直接生成 Python 模块。
6. **自定义任务**：可以写 Lua 脚本自动化管理。