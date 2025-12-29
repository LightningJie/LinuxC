# 一、普通克隆项目



# 二、fork项目提交PR

### 1. 克隆你 fork 的仓库到本地

```bash
# 克隆你fork的仓库（替换为你的仓库地址）
git clone https://github.com/你的GitHub用户名/你fork的仓库名称.git

# 进入克隆下来的项目目录
cd 仓库目录名称
```

### 2. 添加原仓库作为远程仓库（upstream）

```bash
# 添加原仓库地址（替换为原仓库的地址）
git remote add upstream https://github.com/原仓库作者/原仓库名称.git

# 验证远程仓库配置是否正确
git remote -v
```

执行后会显示两个远程仓库：`origin`（你的 fork 仓库）和`upstream`（原仓库）

### 3. 同步原仓库最新代码到本地

```bash
# 拉取原仓库的最新代码
git fetch upstream

# 切换到主分支（通常是main或master）
git checkout main

# 合并原仓库的最新代码到本地主分支
git merge upstream/main

# 解决可能的冲突后，推送到你的fork仓库
git push origin main
```

## 提交到zi'ji

如果你想将本地修改提交到自己的仓库（而非原仓库），可以按照以下步骤操作：

### 1. 在自己的代码托管平台（如 GitHub/GitLab）创建一个新仓库

- 登录你的代码托管平台（如 GitHub）
- 点击 "New repository" 创建一个新仓库，记下仓库地址（如`https://github.com/你的用户名/你的仓库名.git`）

### 2. 将本地仓库与你的远程仓库关联

如果是全新克隆的项目，需要先移除原远程仓库关联，再关联你的仓库：

```bash
# 查看当前远程仓库
git remote -v

# 移除原远程仓库（通常是origin）
git remote remove origin

# 关联你的新仓库
git remote add origin https://github.com/你的用户名/你的仓库名.git
```

### 3. 推送本地修改到你的远程仓库

```bash
# 推送本地主分支到你的远程仓库，并设置上游跟踪
git push -u origin main
```

- `-u` 参数用于设置 upstream，之后再推送时只需用 `git push` 即可

### 后续修改与推送流程

```bash
# 本地修改后提交
git add .
git commit -m "描述修改内容"

# 推送到你的远程仓库
git push
```

