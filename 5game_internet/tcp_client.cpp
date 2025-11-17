#include "tcp_client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 客户端初始化
int tcp_client_init() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSA初始化失败，错误码: %d\n", WSAGetLastError());
        return -1;
    }
#endif
    return 0;
}

// 连接服务器
socket_t tcp_connect_server(const char* server_ip, int port) {
    socket_t client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
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
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
#ifdef _WIN32
        printf("IP地址无效，错误码: %d\n", WSAGetLastError());
#else
        perror("IP地址无效");
#endif
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return -1;
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
#ifdef _WIN32
        printf("连接失败，错误码: %d\n", WSAGetLastError());
#else
        perror("连接失败");
#endif
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        return -1;
    }

    printf("成功连接到服务器 %s:%d\n", server_ip, port);
    return client_fd;
}

// 关闭客户端连接
void tcp_client_close(socket_t client_fd) {
    if (client_fd >= 0) {
#ifdef _WIN32
        closesocket(client_fd);
#else
        close(client_fd);
#endif
        printf("客户端连接已关闭\n");
    }
#ifdef _WIN32
    WSACleanup();
#endif
}