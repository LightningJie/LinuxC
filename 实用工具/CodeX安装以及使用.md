# CodeX安装以及使用

## 1) 本地安装

### Windows（CMD / PowerShell）

1. 安装 **Node.js（建议 18+）**
2. 全局安装 Codex CLI：

```
npm i -g @openai/codex
```

3. 验证：

```
codex --help
```

> 备注：官方说明 Windows 支持是“实验性”，体验更稳通常是在 WSL 工作区里用。 

### macOS

- Homebrew（cask）：

```
brew install --cask codex
```

### 升级

```
npm i -g @openai/codex@latest
```

------

## 2) 登录与鉴权

Codex 首次运行 `codex` 时通常会提示登录，也可以手动登录：  

1. **浏览器 OAuth 登录（默认）**

```
codex login
```

会打开浏览器走 OAuth。 

### 查看登录状态 / 退出

```
codex login status
codex logout
```



------

## 3) 常用命令速查（命令行）

官方 CLI 命令概览里，最常用的是这些： 

### A. 交互式使用（最常用）

```
cd <你的项目目录>
codex
```

启动 TUI，能读代码、改文件、跑命令。 

你也可以带一句初始任务：

```
codex "帮我梳理这个仓库结构并指出风险点"
```

### B. 脚本/CI 非交互（强烈建议你记住）

```
codex exec "生成 release notes（基于 git log），输出为 markdown"
```

`codex exec`（别名 `codex e`）用于“跑完就退出”的自动化场景，可输出纯文本或 JSON 事件流。 

常见搭配：

```
codex exec --json "总结本仓库 open bugs"
codex exec -o out.txt "把最终答案写入文件"
codex exec resume --last "基于上一次 exec 的上下文继续做下一步"
```

这些参数与 resume 行为在官方 reference 里有完整定义。 

### C. 安全相关的关键参数（很重要）

- `--sandbox`：控制模型生成的 shell 命令权限（read-only / workspace-write / danger-full-access） 
- `--ask-for-approval`：控制何时需要你确认后才执行命令 
- `--full-auto`：更“放手”的预设（仍有一定保护） 
- `--yolo`：完全绕过审批和沙盒（官方明确标注危险） 

------

## 4) 交互会话里最常用的“/斜杠命令”

在 `codex` 的交互界面里输入 `/` 会弹出命令列表，常用这些：

- `/model` 切模型
- `/approvals` 调整审批强度
- `/diff` 看改动（含未追踪文件）
- `/review` 让 Codex 做一次 code review
- `/status` 看当前会话配置与 token 状态
- `/mention` 把指定文件/路径“点名”给它
- `/init` 生成 `AGENTS.md` 指令脚手架
- `/compact` 压缩总结当前对话上下文
- `/new` 新开线程、`/resume` 续接历史、`/logout` 退出登录

------

## 5) 建议的使用流程（高成功率）

1. `cd` 到你的仓库根目录
2. 先用保守模式跑一轮（只读 + 解释/定位问题）
3. 再允许它改工作区（workspace-write）并要求它“先给计划，再改代码，再跑测试”
4. 每次改动后用 `/diff` 或 `git diff` 快速审查，再提交

Codex 的设计就是在“你启动命令的目录/允许的目录范围内”读写与运行命令，因此工作目录与权限设置很关键。 

------

