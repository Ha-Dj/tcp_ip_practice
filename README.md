# tcp_ip_practice
这个项目用于记录《TCP/IP网络编程》阅读过程中的一些Lab实践和相关笔记。
## echo_server
### 功能
客户端向服务端写入数据后，服务端将相同数据写回
### 编译
```shell
mkdir build
cd build
cmake ..
make
```
### 运行
```shell
# 在一个 shell中启动服务端
bin/server

# 在另一个 shell中启动客户端
bin/client
```