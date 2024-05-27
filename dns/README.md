# DNS
## 利用域名获取IP
```c
#include <netdb.h>

struct hostent *gethostbyname(const char * hostname);


struct hostent
{
    char * h_name;      // official name
    char ** h_aliases   // alias list
    int h_addrtype;     // host address type. IPV4: AF_INET
    int h_length;       // address length. IPV4: 4, IPV6: 16
    char ** h_addr_list // address list. 最重要的成员，保存域名对应的一组 IP. 一组指向 struct in_addr指针 的指针，所以使用时需要把它强转成 (in_addr *)，再利用 ntoa()转化
};

```

## 利用 IP 获取域名

```c
// addr 传入 in_addr结构体指针
// len 传入地址信息字节数，IPV4 为 4， IPV6为16
// family AF_*
struct hostent * gethostbyaddr(const char * addr, socklen_t len, int family);
```