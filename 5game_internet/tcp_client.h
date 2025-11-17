#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "tcp_comm.h"

// 初始化客户端网络环境
int tcp_client_init();

// 连接服务器
socket_t tcp_connect_server(const char* server_ip, int port);

// 关闭客户端连接
void tcp_client_close(socket_t client_fd);

#endif // TCP_CLIENT_H