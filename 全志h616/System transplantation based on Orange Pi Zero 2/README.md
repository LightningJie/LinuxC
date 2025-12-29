# 基于orangepizero2的系统移植

**System transplantation based on Orange Pi Zero 2**

## 一、环境配置

要求虚拟机Ubuntu22.04以上版本

### 1.1 配置LinuxSDK

确保Ubuntu系统是22.04 而且确保已经配置了镜像源

```
sudo vim /etc/apt/sources.list
```

https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu/

```
sudo apt update
```

克隆Linux SDK

```
git clone https://github.com/orangepi-xunlong/orangepi-build.git -b next
```

因为OrangePi官方的 乌班图镜像3.1.0版本会导致开发板无线网卡和i2c不能使用，orange-build默认编译出来的版本就是3.1.0 ，编译出来的镜像烧写到sd卡中会出bug，因此要回退Linux内核版本，回退到`5.16.17` 也就是`OrangePi3.0.6`版本

修改 `orangepi-build-next/external/config/sources/families/sun50iw9.conf` 文件中的next代码 将6.0两行代码注释，

```
 vim ./external/config/sources/families/sun50iw9.conf
 
 KERNELBRANCH="branch:orange-pi-5.16-sunxi64"
 LINUXCONFIG="linux-5.16-sun50iw9-current"
```

### 1.2 编译Linux镜像

```bash
sudo ./build
```

选择orangepi zero2

```
Full os image for flashing
orangepizero2 Allwinner H616 quad core 512MB/1GB RAM WiFL/BT GBE SPI
jammy Ubuntu jammy 22.04 LTS
Image with desktop environment
Xfce desktop environment
```

就可以了。

### 1.3 百度云压缩包配置LinuxSDK

```
tar -zxvf orangepi-build-ok.tar.gz
```

