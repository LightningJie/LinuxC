安装apt 需要的包

```
sudo apt-get install ca-certificates curl gnupg lsb-release apt-transport-https 
```

如果你不想使用清华源，想切换回 Docker 官方源，只需把上述命令中的清华源地址替换成 Docker 官方地址即可。具体操作如下：

#### 1. 导入 Docker 官方 GPG 密钥

```bash
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
```

#### 2. 添加 Docker 官方软件源

```bash
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
```

#### 3. 更新软件包索引

```bash
sudo apt update
```

#### 4. 安装 Docker

```bash
sudo apt install docker-ce docker-ce-cli containerd.io
```

#### 5. 给权限

```
sudo usermod -aG docker $USER

newgrp docker//立刻生效
```

#### 6.添加源

```
sudo vi /etc/docker/daemon.json
```

```
{
    "registry-mirrors": [
        "https://dockerproxy.com",
        "https://docker.m.daocloud.io",
        "https://cr.console.aliyun.com",
        "https://ccr.ccs.tencentyun.com",
        "https://hub-mirror.c.163.com",
        "https://mirror.baidubce.com",
        "https://docker.nju.edu.cn",
        "https://docker.mirrors.sjtug.sjtu.edu.cn",
        "https://github.com/ustclug/mirrorrequest",
        "https://registry.docker-cn.com"
    ]
}
```

#### 7.重载配置文件

```
sudo systemctl daemon-reload
sudo systemctl restart docker
```

