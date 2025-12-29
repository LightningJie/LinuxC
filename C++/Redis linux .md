# 一、Redis快速安装

### 1. 拉取 Redis 镜像

```bash
docker pull redis:latest  # 获取最新版Redis镜像
```

### 2. 启动 Redis 容器

```bash
# 启动一个基础的Redis容器（仅用于测试）
docker run --name my-redis -p 6379:6379 -d redis

# 更完整的启动方式（带持久化和配置）
docker run --name my-redis \
  -p 6379:6379 \
  -v /本地路径/redis-data:/data \  # 数据持久化到本地
  -v /本地路径/redis.conf:/usr/local/etc/redis/redis.conf \  # 挂载配置文件
  -d redis redis-server /usr/local/etc/redis/redis.conf
```

首先需要创建本地的 Redis 数据目录和配置文件：

```bash
# 在当前用户目录下创建redis相关文件夹
mkdir -p ~/redis/data ~/redis/conf

# 下载官方默认配置文件到本地（可选但推荐）
wget -O ~/redis/conf/redis.conf http://download.redis.io/redis-stable/redis.conf
```

然后使用完整路径启动容器：

```bash
docker run --name my-redis \
  -p 6379:6379 \
  -v /home/ub2204/redis/data:/data \  # 数据持久化到本地
  -v /home/ub2204/redis/conf/redis.conf:/usr/local/etc/redis/redis.conf \  # 挂载配置文件
  -d redis redis-server /usr/local/etc/redis/redis.conf
```

```bash
docker run --name my-redis \
  -p 6379:6379 \
  -v /home/ub2204/redis/data:/data \
  -v /home/ub2204/redis/conf/redis.conf:/usr/local/etc/redis/redis.conf \
  -d redis redis-server /usr/local/etc/redis/redis.conf
```



### 3. 进入 Redis 容器并使用客户端

```bash
# 进入容器内部
docker exec -it my-redis /bin/bash

# 在容器内启动Redis客户端
redis-cli

# 测试Redis命令
127.0.0.1:6379> set test "hello redis"
OK
127.0.0.1:6379> get test
"hello redis"
127.0.0.1:6379> keys *
1) "test"
```

### 4. 从宿主机直接连接 Redis

不需要进入容器，也可以直接从宿主机连接：

1. **修改本地的 redis.conf 配置文件**：

   ```bash
   vim ~/redis/conf/redis.conf
   ```

2. **找到并修改以下配置**：

   - 注释掉 `bind 127.0.0.1`（在行首加 `#`）
   - 将 `protected-mode yes` 改为 `protected-mode no`（开发环境临时关闭保护模式）

```bash
# 宿主机安装redis-cli（以Ubuntu为例）
sudo apt install redis-tools

# 连接容器中的Redis
redis-cli -h localhost -p 6379

#如果 Redis 容器设置了密码，使用redis-cli连接时需要通过-a参数来指定密码。具体命令格式如下：

redis-cli -h <主机地址> -p <端口号> -a <密码>
```

### 5. 容器管理常用命令

```bash
# 停止容器
docker stop my-redis

# 启动已停止的容器
docker start my-redis

# 重启容器
docker restart my-redis

# 查看容器状态
docker ps -a | grep redis

# 删除容器（需先停止）
docker rm my-redis

# 查看容器日志
docker logs my-redis
```

# 二、Redis五大数据结构

Redis支持的存储数据类型有很多：

常用：**string**（字符串），**hash**（哈希），**list**（列表），**set**（集合）及**zset**(sorted set：有序集合)

不常用：HyperLogLog，Bitmap(位图)，Bloom Filter(布隆过滤器)，Geospatial(地理位置) ，Module(模块)， Streams(流信息)

此处重点讲解常用的类型。

**命令格式**

类型命令    key       参数数据

   set           name    dafei

**操作建议**

Redis操作有点类似Java的Map集合，都是key-value形式存储数据，在学习过程中，可以进行类比。

另外Redis中的key大部分为String类型，value值根据缓存数据结构可以选用：string，hash，list，set，zset等类型。



**注意：下面讲的各种类型，表述的是缓存数据的value类型。**

1. **key 永远是字符串类型**
   无论存储什么数据类型（String、Hash、List 等），Redis 的键（key）本质上都是字符串，比如 "user:1"、"count" 等。
2. **类型划分确实是针对 value 的**
   Redis 对数据类型的划分（String、Hash、List 等），都是针对「值（value）」而言的。
3. **为什么 String 类型的 value 能存数字？**
   Redis 的 String 类型是一种「二进制安全的字符串」，它的底层存储形式是字节序列，并不严格区分 “字符串” 和 “数字”：
   - 当你存入 "123" 时，它可以被当作字符串处理（比如拼接操作 `append count "4"` 会变成 "1234"）
   - 同时 Redis 也提供了专门的数字操作（如 `incr count`），会自动将字符串形式的数字转为整数计算后再存回去

## String类型

String类型包含多种类型的特殊类型，并且是二进制安全的，其值可以是数值，可以是字符串，也可以是二进制数据。

类似Java中：**Map<String,  String/Number>  map**

在Redis内容数据存储结构：

![image-20220829195054925](images/image-20220829195054925.png)

### **常用的命令**

| 命令格式                | 功能                                               | 案例                |
| ----------------------- | -------------------------------------------------- | ------------------- |
| set key value           | 将key-value缓存redis中                             | set name dafei      |
| get key                 | 从redis中获取key对应value值                        | get name            |
| incr key                | 将key对应value值 + 1                               | incr age            |
| decr key                | 将key对应value值-1                                 | decr age            |
| setex key seconds value | 将key-value缓存到redis中，seconds 秒后失效         | setex  sex  10  man |
| ttl key                 | 查看key存活时间                                    | ttl sex             |
| del  key                | 从redis中删除key                                   | del name            |
| setnx key value         | 如果key已经存，不做任何操作，如果key不存，直接添加 | setnx  name xiaofei |



### **非常用命令**

| 命令格式                    | 功能                                                      | 案例                   |
| --------------------------- | --------------------------------------------------------- | ---------------------- |
| incrby key increment        | 给key对应值加increment                                    | incrby age 10          |
| mset k1 v1 k2 v2....        | 批量添加k1v1 k2v2 key value对                             | mset name dafei age 18 |
| mget k1  k2....             | 批量获取k1, k2的值                                        | mget name  age         |
| append key  value           | 在key对应的value值中拼+value                              | append name yes        |
| setrange key  offset  value | 修改key对应的value值,替换为指定value,冲offset索引位置开始 | setrange name 2   xx   |



## Hash类型

Hash类型是String类型的field和value的映射表，或者说是一个String集合。它特别适合存储对象，相比较而言，将一个对象存储在Hash类型里要比存储在String类型里占用更少的内存空间。

类似Java中：**Map<String, Map<String, ?>> map**

![image-20220831190853799](images/image-20220831190853799.png)*

### **常用的命令**

| 命令格式                       | 功能                                                | 案例                 |
| ------------------------------ | --------------------------------------------------- | -------------------- |
| hset key field  value          | 将field  value对缓存到redis中hash中，键值为key      | hset user name dafei |
| hget key field                 | 从key对应hash列表中获取field字段                    | hget user  name      |
| hexists key  field             | 判断key对应的hash列表是否存在 field字段             | hexists user age     |
| hdel key  field                | 删除key对应hash列表中field字段                      | hdel user age        |
| hincrby  key  field  increment | 给key对应hash列表中field字段 + increment            | hincrby user  age 10 |
| hlen key                       | 查看key对应的hash列表field的数量                    | hlen user            |
| hkeys  key                     | 获取key对应的hash列表所有的field值                  | hkeys  user          |
| hvals  key                     | 获取key对应的hash列表所有的field对应的value值       | kvals  user          |
| hgetall key                    | 获取key对应的hash列表中所有的field及其对应的value值 | hgetall user         |

## List类型

Redis中的List类似Java中的Queue,也可以当做List来用.

List类型是一个链表结构的集合，其主要功能有push、pop、获取元素等，更详细的说,List类型是一个双端链表的结构，我们可以通过相关操作进行集合的头部或者尾部添加删除元素，List的设计非常简单精巧，即可以作为栈,又可以作为队列，满足绝大多数需求.

类似Java中：**Map<String, List>  map**

![image-20220831194916914](images/image-20220831194916914.png)*



### **常用的命令**

| 命令格式              | 功能                                                     | 案例              |
| --------------------- | -------------------------------------------------------- | ----------------- |
| rpush  key  value     | 从右边往key集合中添加value值                             | rpush hobby java  |
| lrange key start stop | 输出从左边开始列表key集合，从start位置开始，stop位置结束 | lrange hobby 0 -1 |
| lpush key value       | 从左边往key集合中添加value值                             | lpush hobby c++   |
| lpop key              | 弹出key集合中最左边的数据                                | lpop hobby        |
| rpop key              | 弹出key集合中最右边的数据                                | rpop hobby        |
| llen key              | 获取列表长度                                             | llen hooby        |



### **非常用命令**

| 命令格式                        | 功能                                    | 案例                          |
| ------------------------------- | --------------------------------------- | ----------------------------- |
| linsert key before pivot value  | 操作key集合，在pivot值之前插入value     | linsert hobby before java  c# |
| linsert key  after  pivot value | 操作key集合，在pivot值之后插入value     | linsert hobby after  java  c# |
| lset key  index  value          | 操作key集合，更新索引index位置值为value | lset hobby 1  go              |
| lrem key count  value           | 操作key集合，删除 count个 value值       | lrem hobby 3   go             |
| ltrim   key  start stop         | 操作key集合，从start到stop截取自列表    | ltrim  hobby 2   4            |
| lindex  key  index              | 操作key集合，获取索引为index位置的数据  | lindex  hobby 1               |

## Set类型

Set集合是String类型的无序集合,set是通过HashTable实现的，对集合我们可以取**交集、并集、差集。**

类似Java中：**Map<String, Set>  map**

![image-20220901191547262](images/image-20220901191547262.png)*



### **常用的命令**

| 命令格式                 | 功能                           | 案例               |
| ------------------------ | ------------------------------ | ------------------ |
| sadd key  members [....] | 往key 集合中添加member元素     | sadd myset a  b  c |
| smembers key             | 遍历key集合中所有的元素        | smembers myset     |
| srem  key members [....] | 删除key集合中members元素       | srem myset a       |
| spop key count           | 从key集合中随机弹出count个元素 | spop myset 1       |



### **非常用命令**

| 命令格式                        | 功能                                                 | 案例                         |
| ------------------------------- | ---------------------------------------------------- | ---------------------------- |
| **sdiff key1   key2**           | **返回key1中特有的元素(差集)**                       | **sdiff key1 key2**          |
| sidiffstore  dest  key1 key2    | 返回key1中特有的元素，并将返回值缓存到dest集合中     | sidiffstore  dest  key1 key2 |
| **sinter key1 key2**            | **返回key1跟key2集合的交集**                         | **sinter key1 key2**         |
| sinterstore  dest key1 key2     | 返回key1跟key2集合的交集，并将返回值缓存到dest集合中 | sinterstore  dest key1 key2  |
| **sunion key1  key2**           | **返回key1跟key2集合的并集**                         | **sunion key1  key2**        |
| sunionstore dest key1  key2     | 返回key1跟key2集合的并集，并将返回值缓存到dest集合中 | sunionstore dest key1  key2  |
| smove source destination member | 将source集合中member元素移动到destination集合中      | smove key1  key2 aa          |
| sismember key member            | 判断member元素是否在key集合中                        | sismember key1   aa          |
| srandmember  key  count         | 随机获取set集合中count 个元素                        | srandmember key1 1           |

## Sorted set 类型

Sorted set 也称Zset类型，是一种具有排序效果的set集合。它跟set集合一样也是 string 类型元素的集合，且不允许重复的成员。并且要求每个元素都会关联一个double 类型的分数。后续可以通过分数来为集合中的成员进行从小到大的排序。

Sorted set集合是通过哈希表实现的，所以添加，删除，查找的复杂度都是 O(1)。 集合中最大的成员数为 232 - 1 (4294967295, 每个集合可存储40多亿个成员)。

数据缓存结构：

![image-20220901200221440](images/image-20220901200221440.png)*



### **常用的命令**

| 命令格式                                | 功能                                       | 案例                               |
| --------------------------------------- | ------------------------------------------ | ---------------------------------- |
| zadd key score member                   | 往key集合中添加member元素，分数为score     | zadd players 100  a                |
| zincrby  key increment  member          | 将key集合中的member元素 分数 + increment   | zadd players 100  a                |
| zrange  key  start  stop [withscores]   | 将key集合中的元素按分数升序排列 [显式分数] | zrange players 0 -1  withscores    |
| zrevrange key  start  stop [withscores] | 将key集合中的元素按分数降序排列 [显式分数] | zrevrange players 0 -1  withscores |
| zrank  key  member                      | 返回member元素在key结合中的正序排名        | zrank players  a                   |
| zrevrank key  member                    | 返回member元素在key结合中的倒序排名        | zrevrank players  a                |
| zcard  key                              | 返回key集合元素个数                        | zcard  players                     |



### **非常用命令**

| 命令格式                                     | 功能                                          | 案例                                           |
| -------------------------------------------- | --------------------------------------------- | ---------------------------------------------- |
| zrangebyscore  key  min  max  [withscores]   | 按[min, max) 分数范围返回key集合中元素(正序)  | zrangebyscore players  200 300  withscores     |
| zrevrangebyscore key  min  max  [withscores] | 按[min, max) 分数范围返回key集合中元素(倒序)  | zrevrangebyscore players  200 300  withscores  |
| zrem key member                              | 删除key集合中member元素与分数                 | zrem players  a                                |
| zremrangebyscore  key min max  withscores    | 按[min, max) 分数范围删除key集合中元素        | zremrangebyscore  players  200 300  withscores |
| zremrangebyrank  key start  stop             | 删除key集合正序排名落在[start, stop) 范围元素 | zremrangebyrank  players  10  20               |
| zcount key min max                           | 按照分数范围[min, max]统计key集合中元素个数   | zcount  players  100 300                       |

## 类型总结

一个问题，Redis在项目中如何使用？

思考点：

1>项目是否需要使用到缓存？使用

2>使用缓存是否选用Redis？选用

3>使用Redis那该怎么设计Key-Value值？

这里重点讨论Redis的KV对的设计。



### Value设计

先说value值的设计其实就是value类型选用： String， Hash， List， Set， Sort Set

一般考虑：

- 是否需要排序？要使用Sort Set
- 缓存的数据是多个值还是单个值，
- 多个值：允许重复选List  不允许重复选择Set
- 单个值：简单值选择String， 对象值选择Hash



一种取巧的方式：

- 是否需要排序？要使用Sort Set
- 剩下使用String



操作方式：

所有value之后都转换成json格式字符串，然后缓存到Redis，原因：Java操作方便，减少泛型操作麻烦

比如：

```java
List<String>list = ...
Set<String> set = ....
Map<String, Object> map = ....


List<Object>  list = redis对象.getList
Set<Object> set =redis对象.getSet   
Map<Object, Object> map  =  redis对象.getMap
```

不管存放啥数据类型，从reds中获取出来都是Object类型，后续对象强制转换麻烦，干脆直接使用字符串。



### Key设计

Redis 的key 设计讲究4个性：

#### **唯一性**

Redis 类似Map集合，key必须保证唯一，缓存同一个key时，后者会覆盖前者，所有必须要求唯一，那如何保证唯一呢？

最常用的方式：**使用缓存数据的主键作为key**。

比如：缓存员工信息

key                             value

\----------------------------------------------------------------

1           					 员工1

2            					员工2

其中的1， 2 是员工的id



#### **可读性**

可读性是保证Redis的key能做到见名知意，上面的员工id1， 员工id2 虽说能保证key唯一，但可读性非常差，维护key时，无法从， 1， 2中快速判断该key对应value值。所以一一般在保证key唯一的前提下，给key加上前缀：

key                                       value

\----------------------------------------------------------------

employee_info:id1            员工1

employee_info:id2            员工2



employee_info:id1     employee_info:id2  这样子设计key，可读性就好多了。



可读性前缀的设计规范千奇百怪，我个人比较推崇的：

- 普通单值

  key                                       value

  \----------------------------------------------------------------

  employe_info:id1              员工对象信息

  

- 类似关系型数据库设计

  **表名:主键名:主键值:列名**

  key                                       value

  \----------------------------------------------------------------

  employee : id : 1:info        员工对象信息

  

- 通用玩法

  **业务模块名:业务逻辑含义:其他:value类型**

  key                                                        value

  \-----------------------------------------------------------------------

  employee :base.info:id1:hash         员工对象信息

  **业务模块名**：表示该key属于哪个功能模块

  **业务逻辑含义段**：这里可以使用  **.**  分开， 具体业务逻辑表示

  ​	比如：缓存员工权限

  ​    employee:auth.permission:id1:set     员工权限集合

  **其他**：一般设置唯一标识，比如主键

  **value类型**：key对应value类型值，提高可读性。

  

#### **灵活性**--项目二重点讲

这个难介绍，一般key保证唯一时，可以使用主键，有的使用，一个主键不能表达出全部意思，可以使用联合主键。

比如：

id为1的朋友圈下id为A的评论。

key                                                        value

\-----------------------------------------------------------------------

post:1:reply:A                                    评论内容

post:1:reply:B                                    评论内容



#### **时效性**

Redis key一定要设置过期时间。要跟自己的业务场景，需要对key设置合理的过期时间。可以在写入key时，就要追加过期时间；也可以在按照需要动态设置。

这里要注意：

- 不设置过期时间，这种key为永久key，会一直占用内存不释放，时间久了，数量一多，就容易达到服务器的内存上限，导致宕机，开发时一般配合Key过期策略使用哦。
- key的时效性设置，必须根据业务场景进行评估，设置合理有效期；

# Redis全局命令

全局命令针对的是所有的key，大部分用来做运维，做管理的

常用的全局key

| 命令格式            | 功能                                       | 案例                 |
| ------------------- | ------------------------------------------ | -------------------- |
| keys  pattern       | 按照pattern 匹配规则，列表redis中所有的key | keys xxx:*           |
| exists  key         | 判断key是否存在                            | exists name          |
| expire key  seconds | 给key设置过期时间，超时：seconds           | expire name 10       |
| persist key         | 取消key过期时间                            | persist  name        |
| select  index       | 切换数据库，默认是第0个，共有【0,15】个    | select 0             |
| move key   db       | 从当前数据库将key移动到指定db库            | move name 1          |
| randomkey           | 随机返回一个key                            | randomkey            |
| rename key newkey   | 将key改名为newkey                          | rename name  newname |
| echo message        | 打印message信息                            | echo  message        |
| dbsize              | 查看key个数                                | dbsize               |
| info                | 查看redis数据库信息                        | info                 |
| config get  *       | 查看所有redis配置信息                      | config get *         |
| flushdb             | 清空当前数据库                             | flushdb              |
| flushall            | 清空所有数据库                             | flushall             |

# Redis事务[拓展]

一个事务从开始到执行会经历以下三个阶段：

- 开始事务。
- 命令入队。
- 执行事务。

借用：菜鸟教程中例子

它先以 **MULTI** 开始一个事务， 然后将多个命令入队到事务中， 最后由 **EXEC** 命令触发事务， 一并执行事务中的所有命令：

```bash
redis 127.0.0.1:6379> MULTI
OK

redis 127.0.0.1:6379> SET book-name "Mastering C++ in 21 days"
QUEUED

redis 127.0.0.1:6379> GET book-name
QUEUED

redis 127.0.0.1:6379> SADD tag "C++" "Programming" "Mastering Series"
QUEUED

redis 127.0.0.1:6379> SMEMBERS tag
QUEUED

redis 127.0.0.1:6379> EXEC
1) OK
2) "Mastering C++ in 21 days"
3) (integer) 3
4) 1) "Mastering Series"
   2) "C++"
   3) "Programming"
```

单个 Redis 命令的执行是原子性的，但 Redis 没有在事务上增加任何维持原子性的机制，所以 Redis 事务的执行并不是原子性的。

Redis事务可以理解为一个**打包的批量执行脚本**，但批量指令并非原子化的操作，中间某条指令的失败不会导致前面已做指令的回滚，也不会造成后续的指令不做。

```bash
127.0.0.1:6379> multi
OK
127.0.0.1:6379(TX)> set name dafei
QUEUED
127.0.0.1:6379(TX)> set age 18
QUEUED
127.0.0.1:6379(TX)> incr age 
QUEUED
127.0.0.1:6379(TX)> incr name
QUEUED
127.0.0.1:6379(TX)> get age
QUEUED
127.0.0.1:6379(TX)> get name
QUEUED
127.0.0.1:6379(TX)> exec
1) OK
2) OK
3) (integer) 19
4) (error) ERR value is not an integer or out of range
5) "19"
6) "dafei"
127.0.0.1:6379> 
```



Redis 事务可以一次执行多个命令， 并且带有以下三个重要的保证：

- 批量操作在发送 EXEC 命令前被放入队列缓存。
- 收到 EXEC 命令后进入事务执行，事务中任意命令执行失败，其余的命令依然被执行。
- 在事务执行过程，其他客户端提交的命令请求不会插入到事务执行命令序列中。



# Redis持久化机制[拓展]

## 引言

先来一个小实验，大家可以一起实现一下

步骤1：在Redis中添加2个key-value对

```bash
127.0.0.1:6379> set aa aa
OK
127.0.0.1:6379> set bb bb
OK
127.0.0.1:6379> keys *
```

步骤2：重启Redis 服务器，在执行keys * 观察数据

步骤3：分析结果

会出现一下几种结果：

- 之前的key在，aa  bb 都在（最理想的结果）
- 之前的key在，aa也在，bb不见了
- 之前的key在，aa， bb 不在
- 之前的key， aa， bb 都不在了（最坏的结果）

思考：

为啥会这样？以我们对内存的操作理解，按道理重启之后数据应该全丢失了，为啥Redis 可能丢失，也可能不丢失，为何？

这里就涉及到Redis的持久化机制了。

![image-20220902113153504](images/image-20220902113153504.png)*



Redis持久化机制目前以后3种，分别为：

1>**快照方式**（RDB, Redis DataBase）

2>**文件追加方式**（AOF, Append Only File）

3>**混合持久化方式**（Redis4版本之后）



## RDB方式

Snapshotting(快照)默认方式，将内存数据中以快照的方式写入到二进制文件中，默认为dump.rdb。触发RDB持久化过程分手动触发与自动触发。



### 触发机制

**手动触发**

使用save命令：会阻塞当前Redis服务器，知道RDB过程完成为主，如果内存数据较多，会造成长时间阻塞，影响其他命令的使用，不建议轻易使用

使用bgsave命令：Redis进程执行fork指令创建子进程，由子进程实现RDB持久化，有需要时建议使用bgsave命令。

**自动触发**

使用save相关配置，格式： save m n      表示m秒内数据集存在n次修改时会自动触发bgsave命令。

```java
save 900 1  #900秒内如果超过1个Key被修改则发起快照保存
save 300 10 #300秒内如果超过10个key被修改,则发起快照保存
save 60 10000
```

![image-20220902151151422](images/image-20220902151151422.png)*

### 优点

- RDB快照文件是一个紧凑压缩的二进制文件，非常使用用于备份，全量复制等场景。开发中可以按照每6小时执行一次bgsave备份，用于容灾备份。

- Redis加载RDB恢复数据远远快于AOF方式

### 缺点

- RDB无法做到实时持久化/秒级持久化，每次bgsave时都需要fork子进程，频繁执行有时间成本。
- RDB快照文件不同版本格式不一样，容易引起兼容问题。



## AOF方式

AOF与RDB不一样，它是一独立日志的方式记录每次写命令，重启时再重新执行AOF文件中命令达到恢复数据的目的。解决了数据持久化的实时性的问题。

Redis默认是不开启的，需要使用时，需要配置： **appendonly yes**

AOF 有3种文件同步策略

| 策略                 | 解释                                                   |
| -------------------- | ------------------------------------------------------ |
| appendfsync always   | 收到命令就立即写到磁盘,效率最慢.但是能保证完全的持久化 |
| appendfsync everysec | 每秒写入磁盘一次,在性能和持久化方面做了很好的折中      |
| appendfsync no       | 完全以依赖os，一般同步周期是30秒                       |

![image-20220902152951385](images/image-20220902152951385.png)*

### 优点

- AOF方式数据安全性更高，配置得当，最多损失1秒的数据量
- 在不小心执行flushall命令，也可以通过AOF方式恢复(删除最后一个命令即可)

- AOF 日志是一个增量日志文件，不会存在断电时出现损坏问题。即使出现问题，redis-check-aof 工具也能够轻松修复它。
- 当 AOF 变得太大时，Redis 能够在后台自动重写 AOF



### 缺点

- 相同数据量来说，AOF文件体积通常大于RDB文件
- 数据持久化性能上来说，AOF 比 RDB 慢



## RDB-AOF混合方式

混合持久化是结合了 RDB 和 AOF 的优点，在写入的时候，先把当前的数据以 RDB 的形式写入文件的开头，再将后续的操作命令以 AOF 的格式存入文件。即以 RDB 作为全量备份，AOF 作为增量备份，来提高备份效率。这样既能保证 Redis 重启时的速度，又能防止数据丢失的风险， 这就是 Redis 4.0 之后推出的 **RDB-AOF 混合持久化模式，其作为默认配置来使用**。



## 持久化机制选择

- 如果对数据安全性有非常高的要求，建议 RDB 和 AOF 同时启用。

- 如果对数据安全性要求不是很高，能够容忍数据的丢失，建议单独使用 RDB。

- 不推荐单独使用 AOF，因为对于进行数据库备份、更快重启以及 AOF 引擎中出现错误的情况来说，RDB 是更好的选择。

- 如果没特殊要求，Redis又是4.x版本以上，可以选择RDB-AOF混合方式。

  

如果不是混合模式，而是普通的RDB与AOF一起启动时，Redis加载数据执行流程

![image-20220902153031549](images/image-20220902153031549.png)*





# Redis内存淘汰机制[拓展]

## 引言

Redis 启动会加载一个配置：

```bash
maxmemory <byte>   //内存上限
```

默认值为 0 (window版的限制为100M)，表示默认设置Redis内存上限。但是真实开发还是需要提前评估key的体量，提前设置好内容上限。

此时思考一个问题，开发中，在设置完内存上限之后，如果Redis key达到上限了，该怎么办？这就设置到Redis的内存淘汰机制了。



## 内存淘汰算法

Redis内存淘汰机制也可以称之为key内卷机制，当资源不足时，该如何选择？

常见的内存淘汰机制分为四大类：

- **LRU：**LRU是Least recently used，最近最少使用的意思，简单的理解就是从数据库中删除最近最少访问的数据，该算法认为，你长期不用的数据，那么被再次访问的概率也就很小了，淘汰的数据为最长时间没有被使用，仅与时间相关。

![image-20220902161050652](images/image-20220902161050652.png)

- **LFU：**LFU是Least Frequently Used，最不经常使用的意思，简单的理解就是淘汰一段时间内，使用次数最少的数据，这个与频次和时间相关。

  ![image-20220902161057116](images/image-20220902161057116.png)

- **TTL：**Redis中，有的数据是设置了过期时间的，而设置了过期时间的这部分数据，就是该算法要解决的对象。如果你快过期了，不好意思，我内存现在不够了，反正你也要退休了，提前送你一程，把你干掉吧。

- 随机淘汰：生死有命，富贵在天，是否被干掉，全凭天意了。



## Redis淘汰策略

Redis 通过配置

```bash
maxmemroy-policy  
```

来配置指定具体的淘汰机制，可供选择的值有：

　通过maxmemroy-policy可以配置具体的淘汰机制，看了网上很多文章说只有6种，其实有8种，可以看Redis5.0的配置文件，上面有说明：

- volatile-lru -> 找出已经设置过期时间的数据集，将最近最少使用（被访问到）的数据干掉。
- volatile-ttl -> 找出已经设置过期时间的数据集，将即将过期的数据干掉。
- volatile-random -> 找出已经设置过期时间的数据集，进行无差别攻击，随机干掉数据。
- volatile-lfu -> 找出已经设置过期时间的数据集，将一段时间内，使用次数最少的数据干掉。



- allkeys-lru ->与第1个差不多，数据集从设置过期时间数据变为全体数据。
- allkeys-lfu -> 与第4个差不多，数据集从设置过期时间数据变为全体数据。
- allkeys-random -> 与第3个差不多，数据集从设置过期时间数据变为全体数据。



- no-enviction -> 什么都不干，报错，告诉你内存不足，这样的好处是可以保证数据不丢失

**系统默认选择： noenviction** 

