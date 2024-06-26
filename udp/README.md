# UDP
类比为邮件
套接字就像是**邮筒**，只需要有一个邮筒就可像任意主机传输数据
## UDP 中服务器端与客户端没有连接
### UDP 服务器端和客户端均只需要 1 个套接字
![UDP套接字通信模型](./figure/img1.png)
### UDP 数据报
![UDP数据报](./figure/image.png)
因此，发送几次udp数据报(sendto)，就要接受几次(recv_from)
### UDP 的数据 I/O函数
#### 发送函数
```c
#include <sys/socket.h>

// sock 套接字
// buff 待传输数据的缓冲地址
// nbytes   待传输数据长度，单位为字节
// flags    可选参数，没有传 0
// to       目标地址
// addrlen  目标地址 to的长度 
ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
```

#### 接受函数
```c
#include <sys/socket.h>

// from 和 addrlen用于存读取到的 udp 报文的 sockaddr 和 addrlen
ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t *addrlen);
```

#### connect
sendto 流程:
1. 向 UDP套接字注册目标IP和端口号
2. 传输数据
3. 删除 UDP套接字中注册的目标IP和端口

如果是多次重复向同一个 IP:PROT 发送 udp报文，可以先调用 connect，注册后，再调用 read write，节省上述的 第 1 步的开销
## 实现了一个 upd 版本的 echo_server
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
### clean
```shell
make clean_build
```