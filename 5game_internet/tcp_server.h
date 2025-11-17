#pragma once
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "tcp_comm.h"

// 服务端上下文
typedef struct {
    socket_t server_fd;         // 监听套接字
    socket_t client_fd;         // 客户端连接套接字
    struct sockaddr_in client_addr;
    socklen_t addr_len;
} TCPServer;

// 初始化服务端（绑定8000端口）
int tcp_server_init(TCPServer* server, int backlog);

// 接受客户端连接
int tcp_server_accept(TCPServer* server);

// 关闭客户端连接
void tcp_server_close_client(TCPServer* server);

// 清理服务端资源
void tcp_server_cleanup(TCPServer* server);

#endif // TCP_SERVER_H