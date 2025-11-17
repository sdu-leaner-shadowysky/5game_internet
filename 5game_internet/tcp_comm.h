#pragma once
#ifndef TCP_COMM_H
#define TCP_COMM_H

#include <stddef.h>

// 跨平台类型定义
#ifdef _WIN32
#include <Winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
typedef int ssize_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int socket_t;
#endif

/**
 * 发送两个整数到对端
 * @param fd 连接套接字
 * @param a 第一个整数
 * @param b 第二个整数
 * @return 0成功，非0失败（-1发送失败，-2参数无效）
 */
int tcp_send_two_ints(socket_t fd, int a, int b);

/**
 * 从对端接收两个整数
 * @param fd 连接套接字
 * @param a 输出参数：第一个整数
 * @param b 输出参数：第二个整数
 * @return 0成功，非0失败（-1接收失败，-2连接关闭，-3数据不完整）
 */
int tcp_recv_two_ints(socket_t fd, int* a, int* b);

#endif // TCP_COMM_H