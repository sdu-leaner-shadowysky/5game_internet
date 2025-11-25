#include "tcp_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 实现公共接口：发送两个整数（处理字节序）
//int tcp_send_two_ints(socket_t fd, int a, int b) {
//    if (fd < 0) return -2;  // 无效套接字
//
//    // 转换为主机字节序→网络字节序（32位整数用htonl）
//    int net_a = htonl(a);
//    int net_b = htonl(b);
//
//    // 连续发送两个整数（共8字节）
//    ssize_t send_len = send(fd, (const char*)&net_a, sizeof(net_a), 0);
//    if (send_len != sizeof(net_a)) return -1;
//
//    //send_len = send(fd, &net_b, sizeof(net_b), 0);
//    send_len = send(fd, (const char*)&net_b, sizeof(net_b), 0);
//    if (send_len != sizeof(net_b)) return -1;
//
//    return 0;
//}

// 实现公共接口：接收两个整数（处理字节序）
//int tcp_recv_two_ints(socket_t fd, int* a, int* b) {
//    if (fd < 0 || !a || !b) return -2;  // 无效参数
//
//    int net_a, net_b;
//
//    // 连续接收两个整数（共8字节）
//    //ssize_t recv_len = recv(fd, &net_a, sizeof(net_a), 0);
//    ssize_t recv_len = recv(fd, (char*)& net_a, sizeof(net_a), 0);
//    if (recv_len <= 0) {
//        return (recv_len == 0) ? -2 : -1;  // -2连接关闭，-1接收失败
//    }
//    if (recv_len != sizeof(net_a)) return -3;  // 数据不完整
//
//    //recv_len = recv(fd, &net_b, sizeof(net_b), 0);
//    recv_len = recv(fd, (char*) & net_b, sizeof(net_b), 0);
//    if (recv_len <= 0) {
//        return (recv_len == 0) ? -2 : -1;
//    }
//    if (recv_len != sizeof(net_b)) return -3;
//
//    // 转换为网络字节序→主机字节序
//    *a = ntohl(net_a);
//    *b = ntohl(net_b);
//    return 0;
//}

// 服务端初始化
int tcp_server_init(TCPServer* server, int backlog, int port) {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSA初始化失败，错误码: %d\n", WSAGetLastError());
        return -1;
    }
#endif

    server->server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server->server_fd < 0) {
#ifdef _WIN32
        printf("创建套接字失败，错误码: %d\n", WSAGetLastError());
#else
        perror("创建套接字失败");
#endif
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);  // 绑定8000端口port

    if (bind(server->server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
#ifdef _WIN32
        printf("绑定端口失败，错误码: %d\n", WSAGetLastError());
#else
        perror("绑定端口失败");
#endif
#ifdef _WIN32
        closesocket(server->server_fd);
#else
        close(server->server_fd);
#endif
        return -1;
    }

    if (listen(server->server_fd, backlog) < 0) {
#ifdef _WIN32
        printf("监听失败，错误码: %d\n", WSAGetLastError());
#else
        perror("监听失败");
#endif
#ifdef _WIN32
        closesocket(server->server_fd);
#else
        close(server->server_fd);
#endif
        return -1;
    }

    server->addr_len = sizeof(server->client_addr);
    server->client_fd = -1;
    printf("服务端初始化成功，监听%d端口...\n", port);
    return 0;
}

// 接受客户端连接
int tcp_server_accept(TCPServer* server) {
    printf("开始尝试连接\n");
    server->client_fd = accept(server->server_fd,
        (struct sockaddr*)&server->client_addr,
        &server->addr_len);
    if (server->client_fd < 0) {
#ifdef _WIN32
        printf("接受连接失败，错误码: %d\n", WSAGetLastError());
#else
        perror("接受连接失败");
#endif
        return -1;
    }

    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &server->client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("客户端 %s:%d 已连接\n", client_ip, ntohs(server->client_addr.sin_port));
    return 0;
}

// 关闭客户端连接
void tcp_server_close_client(TCPServer* server) {
    if (server->client_fd >= 0) {
#ifdef _WIN32
        closesocket(server->client_fd);
#else
        close(server->client_fd);
#endif
        printf("客户端连接已关闭\n");
        server->client_fd = -1;
    }
}

// 清理服务端资源
void tcp_server_cleanup(TCPServer* server) {
    tcp_server_close_client(server);
    if (server->server_fd >= 0) {
#ifdef _WIN32
        closesocket(server->server_fd);
#else
        close(server->server_fd);
#endif
        printf("服务端已停止\n");
    }
#ifdef _WIN32
    WSACleanup();
#endif
}