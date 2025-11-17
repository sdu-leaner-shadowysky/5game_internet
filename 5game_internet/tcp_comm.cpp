#include "tcp_comm.h"
#include <stdio.h>

// 发送两个整数（处理字节序和类型转换）
int tcp_send_two_ints(socket_t fd, int a, int b) {
    if (fd < 0) return -2;

    int net_a = htonl(a);
    int net_b = htonl(b);

    // 发送时将 int* 强制转换为 const char*，匹配 send 函数参数
    ssize_t send_len = send(fd, (const char*)&net_a, sizeof(net_a), 0);
    if (send_len != sizeof(net_a)) return -1;

    send_len = send(fd, (const char*)&net_b, sizeof(net_b), 0);
    if (send_len != sizeof(net_b)) return -1;

    return 0;
}

// 接收两个整数（处理字节序和类型转换）
int tcp_recv_two_ints(socket_t fd, int* a, int* b) {
    if (fd < 0 || !a || !b) return -2;

    int net_a, net_b;

    // 接收时将 int* 强制转换为 char*，匹配 recv 函数参数
    ssize_t recv_len = recv(fd, (char*)&net_a, sizeof(net_a), 0);
    if (recv_len <= 0) {
        return (recv_len == 0) ? -2 : -1;
    }
    if (recv_len != sizeof(net_a)) return -3;

    recv_len = recv(fd, (char*)&net_b, sizeof(net_b), 0);
    if (recv_len <= 0) {
        return (recv_len == 0) ? -2 : -1;
    }
    if (recv_len != sizeof(net_b)) return -3;

    *a = ntohl(net_a);
    *b = ntohl(net_b);
    return 0;
}