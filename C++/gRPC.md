# gPRC

## 一、安装

```bash
git clone -b v1.56.2 https://github.com/grpc/grpc.git
cd grpc
git submodule update --init --recursive
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build -j4
sudo cmake --install build
cd ..
```

## 二

## 1️⃣ 安装依赖

先装 Protobuf 和 gRPC（Ubuntu 22.04 例子）：

```
# 安装编译工具
sudo apt update
sudo apt install -y build-essential autoconf libtool pkg-config cmake git

# 安装 protobuf
git clone -b v3.21.12 https://github.com/protocolbuffers/protobuf.git
cd protobuf
git submodule update --init --recursive
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build -j4
sudo cmake --install build
cd ..

# 安装 gRPC
git clone -b v1.56.2 https://github.com/grpc/grpc.git
cd grpc
git submodule update --init --recursive
cmake -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build -j4
sudo cmake --install build
cd ..
```

确认安装成功：

```
protoc --version   # 应该输出 libprotoc 3.21.12
```

------

## 2️⃣ 建立项目结构

假设目录叫 `kwdb-grpc`：

```
kwdb-grpc/
 ├── CMakeLists.txt
 ├── proto/
 │    └── kwdb.proto
 └── src/
      ├── server.cpp
      └── client.cpp
```

------

## 3️⃣ 写 proto 文件（只写入）

```
proto/kwdb.proto
syntax = "proto3";

package kwdb;

service WriteService {
  rpc WriteStructured(StructuredWriteRequest) returns (WriteResponse);
}

message StructuredWriteRequest {
  string database = 1;
  string table = 2;
  repeated Row rows = 3;
}

message Row {
  map<string, string> tags = 1;       // 索引标签，适合 WHERE 条件过滤
  map<string, FieldValue> fields = 2; // 实际测量数据
  int64 timestamp = 3;                // 时间戳
}

message FieldValue {
  enum Type {
    INT32 = 0;
    INT64 = 1;
    DOUBLE = 2;
    STRING = 3;
    BOOL = 4;
    BYTES = 5;
  }
  Type type = 1;

  oneof value {
    int32 int32_value = 2;
    int64 int64_value = 3;
    double double_value = 4;
    string string_value = 5;
    bool bool_value = 6;
    bytes bytes_value = 7;
  }
}

message WriteResponse {
  bool success = 1;
  string error_message = 2;
  int64 inserted_points = 3;
}
```

------

## 4️⃣ 生成 C++ 代码

在 `kwdb-grpc/` 目录执行：

```
protoc -I=proto --grpc_out=src --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` proto/kwdb.proto
protoc -I=proto --cpp_out=src proto/kwdb.proto
```

生成的文件：

```
src/kwdb.pb.h
src/kwdb.pb.cc
src/kwdb.grpc.pb.h
src/kwdb.grpc.pb.cc
```

------

## 5️⃣ 写最小 Server

```
// src/server.cpp
#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "kwdb.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using kwdb::WriteService;
using kwdb::StructuredWriteRequest;
using kwdb::WriteResponse;
using kwdb::Row;
using kwdb::FieldValue;

class WriteServiceImpl final : public WriteService::Service {
  Status WriteStructured(ServerContext* context,
                         const StructuredWriteRequest* request,
                         WriteResponse* reply) override {
    std::cout << "Database: " << request->database()
              << " Table: " << request->table() << std::endl;

    for (const auto& row : request->rows()) {
      std::cout << "  Row timestamp: " << row.timestamp() << std::endl;

      // ✅ 打印 tags
      for (const auto& tag : row.tags()) {
        std::cout << "    TAG " << tag.first << " = " << tag.second << std::endl;
      }

      // 打印 fields
      for (const auto& kv : row.fields()) {
        std::cout << "    " << kv.first << " = ";
        const FieldValue& v = kv.second;
        switch (v.type()) {
          case FieldValue::INT32:
            std::cout << v.int32_value();
            break;
          case FieldValue::INT64:
            std::cout << v.int64_value();
            break;
          case FieldValue::DOUBLE:
            std::cout << v.double_value();
            break;
          case FieldValue::STRING:
            std::cout << v.string_value();
            break;
          case FieldValue::BOOL:
            std::cout << (v.bool_value() ? "true" : "false");
            break;
          case FieldValue::BYTES:
            std::cout << "<" << v.bytes_value().size() << " bytes>";
            break;
          default:
            std::cout << "Unknown";
        }
        std::cout << std::endl;
      }
    }

    reply->set_success(true);
    reply->set_inserted_points(request->rows_size());
    return Status::OK;
  }
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  WriteServiceImpl service;

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main() {
  RunServer();
  return 0;
}
```

------

## 6️⃣ 写最小 Client（测试）

```
// src/client.cpp
#include <iostream>
#include <grpcpp/grpcpp.h>
#include "kwdb.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using kwdb::WriteService;
using kwdb::StructuredWriteRequest;
using kwdb::WriteResponse;
using kwdb::Row;
using kwdb::FieldValue;

class WriteClient {
 public:
  WriteClient(std::shared_ptr<Channel> channel)
      : stub_(WriteService::NewStub(channel)) {}

  void InsertTest() {
    StructuredWriteRequest request;
    request.set_database("iot_db");
    request.set_table("sensor");

    // 添加第一行数据
    Row* row = request.add_rows();
    row->set_timestamp(1694862000);

    // ✅ 设置 tags
    auto& tags = *row->mutable_tags();
    tags["device_id"] = "sensor_001";
    tags["location"] = "room_101";
    tags["model"] = "temp-sensor-v2";

    // 设置 fields
    auto& fields = *row->mutable_fields();

    FieldValue temp;
    temp.set_type(FieldValue::DOUBLE);
    temp.set_double_value(36.5);
    fields["temperature"] = temp;

    FieldValue active;
    active.set_type(FieldValue::BOOL);
    active.set_bool_value(true);
    fields["active"] = active;

    FieldValue version;
    version.set_type(FieldValue::INT32);
    version.set_int32_value(2);
    fields["version"] = version;

    // 可选：添加第二行数据（演示多行）
    Row* row2 = request.add_rows();
    row2->set_timestamp(1694862060);

    auto& tags2 = *row2->mutable_tags();
    tags2["device_id"] = "sensor_002";
    tags2["location"] = "hall";

    auto& fields2 = *row2->mutable_fields();
    FieldValue temp2;
    temp2.set_type(FieldValue::DOUBLE);
    temp2.set_double_value(24.1);
    fields2["temperature"] = temp2;

    // 发送请求
    WriteResponse reply;
    ClientContext context;
    Status status_rpc = stub_->WriteStructured(&context, request, &reply);

    if (status_rpc.ok()) {
      std::cout << "Write success=" << reply.success()
                << " inserted_points=" << reply.inserted_points()
                << " error_message=" << reply.error_message() << std::endl;
    } else {
      std::cout << "RPC failed: " << status_rpc.error_message() << std::endl;
    }
  }

 private:
  std::unique_ptr<WriteService::Stub> stub_;
};

int main() {
  WriteClient client(grpc::CreateChannel(
      "localhost:50051", grpc::InsecureChannelCredentials()));
  client.InsertTest();
  return 0;
}
```

------

## 7️⃣ CMakeLists.txt

放在 `kwdb-grpc/`：

```
cmake_minimum_required(VERSION 3.15)
project(kwdb_grpc CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 👉 关键：告诉 CMake 去 /usr/local 找手动安装的库
list(APPEND CMAKE_PREFIX_PATH /usr/local)

# 查找 Protobuf 和 gRPC（使用 Config 模式）
find_package(Protobuf REQUIRED CONFIG)
find_package(gRPC REQUIRED CONFIG)

# 包含必要的头文件目录
include_directories(
    src                            # 生成的 .pb.h 文件在这里
    ${Protobuf_INCLUDE_DIRS}
)

# 你已经生成了这些文件，直接引用
set(PROTO_SRCS
    src/kwdb.pb.cc
    src/kwdb.grpc.pb.cc
)

# 编译 server
add_executable(server src/server.cpp ${PROTO_SRCS})
target_link_libraries(server
    gRPC::grpc++                   # 链接 gRPC++
    ${Protobuf_LIBRARIES}          # 链接 Protobuf
)

# 编译 client
add_executable(client src/client.cpp ${PROTO_SRCS})
target_link_libraries(client
    gRPC::grpc++
    ${Protobuf_LIBRARIES}
)
```

------

## 8️⃣ 编译 & 运行

```
mkdir build && cd build
cmake ..
make -j4
```

运行：

```
./server   # 先启动服务端
./client   # 再运行客户端
```

输出示例：

Server 端：

```
Server listening on 0.0.0.0:50051
Database: iot_db Table: sensor
  Row timestamp: 1694862000
    TAG model = temp-sensor-v2
    TAG location = room_101
    TAG device_id = sensor_001
    temperature = 36.5
    version = 2
    active = true
  Row timestamp: 1694862060
    TAG location = hall
    TAG device_id = sensor_002
    temperature = 24.1
```

Client 端：

```
Write success=1 inserted_points=1
```