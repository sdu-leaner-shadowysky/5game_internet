// Disable deprecated API warnings
#ifndef SERVER_H
#define SERVER_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>

// 为不同编译器提供兼容的库链接方式
#ifdef _WIN32
    // Visual Studio 使用 pragma comment
#ifdef _MSC_VER
#pragma comment(lib, "ws2_32.lib")
#endif
// MinGW 在编译命令中使用 -lws2_32
#endif

// 定义端口号和缓冲区大小
#define BROADCAST_PORT 9845
#define RESPONSE_PORT 9846  // 新增：发送响应的目标端口
#define TCP_PORT 8000  // 游戏房间使用的TCP端口
#define BUFFER_SIZE 1024
#define BROADCAST_ADDRESS "255.255.255.255"


// 声明全局线程控制变量（在.cpp中定义）
//extern std::thread g_roomThread;
//extern class RoomListener* g_roomListener;

class RoomListener {
private:
    WSADATA wsaData;
    SOCKET listenSocket;
    bool isRunning;

public:
    RoomListener() : isRunning(false) {
        // 初始化Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup 失败: " << WSAGetLastError() << std::endl;
            return;
        }

        // 创建UDP套接字
        listenSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (listenSocket == INVALID_SOCKET) {
            std::cerr << "创建套接字失败: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        // 设置套接字的重用地址选项
        int reuseAddr = 1;
        if (setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&reuseAddr, sizeof(reuseAddr)) != 0) {
            std::cerr << "设置重用地址失败: " << WSAGetLastError() << std::endl;
        }

        // 绑定套接字到特定端口
        sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(BROADCAST_PORT);
        addr.sin_addr.s_addr = INADDR_ANY;  // 监听所有网络接口

        if (bind(listenSocket, (sockaddr*)&addr, sizeof(addr)) != 0) {
            std::cerr << "绑定套接字失败: " << WSAGetLastError() << std::endl;
            closesocket(listenSocket);
            WSACleanup();
            return;
        }
    }

    ~RoomListener() {
        stopListening();
        if (listenSocket != INVALID_SOCKET) {
            closesocket(listenSocket);
        }
        WSACleanup();
    }

    void startListening() {
        isRunning = true;
        std::cout << "游戏房间监听器已启动，等待广播查询..." << std::endl;
        std::cout << "TCP端口: " << TCP_PORT << std::endl;

        while (!isRunning) {
            char buffer[BUFFER_SIZE];
            sockaddr_in senderAddr;
            int senderAddrSize = sizeof(senderAddr);

            // 接收广播查询
            int bytesRead = recvfrom(listenSocket, buffer, BUFFER_SIZE - 1, 0,
                (sockaddr*)&senderAddr, &senderAddrSize);


            if (bytesRead == SOCKET_ERROR) {
                int err = WSAGetLastError();
                // 套接字已关闭或操作被中断（主动终止信号）
                if (err == WSAENOTSOCK || err == WSAEINTR) {
                    std::cout << "recvfrom被中断，退出监听循环" << std::endl;
                    break;  // 退出循环
                }
                // 其他错误（如意外错误），继续循环（但isRunning可能已为false）
                std::cerr << "接收数据失败: " << err << std::endl;
                continue;
            }

            buffer[bytesRead] = '\0'; // 添加字符串结束符
            char* senderIP = inet_ntoa(senderAddr.sin_addr);
            std::string message(buffer);

            // 检查是否是游戏房间查询消息
            //std::string message(buffer);
            if (message.find("是否存在游戏房间") != std::string::npos ||
                message.find("ARE_YOU_GAME_ROOM") != std::string::npos) {

                std::cout << "接收到来自 " << senderIP << " 的查询: " << message << std::endl;

                // 构造响应消息，包含TCP端口号
                std::string response = "ROOM_FOUND:" + std::to_string(TCP_PORT);

                // 将响应发送回查询方
                // 将响应发送到9846端口
                sockaddr_in responseAddr = senderAddr; // 复制发送者地址
                responseAddr.sin_port = htons(RESPONSE_PORT); // 修改端口为9846

                int bytesSent = sendto(listenSocket, response.c_str(), response.length(), 0,
                    (sockaddr*)&responseAddr, sizeof(responseAddr));


                if (bytesSent == SOCKET_ERROR) {
                    std::cerr << "发送响应失败: " << WSAGetLastError() << std::endl;
                }
                else {
                    std::cout << "已向 " << senderIP << " 发送响应: " << response << std::endl;
                    std::cout << "房间IP: " << getLocalIP() << ", TCP端口: " << TCP_PORT << std::endl;
                }
            }
        }

        std::cout << "监听循环已退出" << std::endl;
    }

    void stopListening() {
        isRunning = false;
        if (listenSocket != INVALID_SOCKET) {
            closesocket(listenSocket);  // 新增：关闭套接字，强制中断阻塞的recvfrom
            listenSocket = INVALID_SOCKET;
        }
        std::cout << "游戏房间监听器已停止" << std::endl;
    }

private:
    // 获取本地IP地址的辅助方法
    std::string getLocalIP() {
        char hostname[256];
        if (gethostname(hostname, sizeof(hostname)) == SOCKET_ERROR) {
            return "127.0.0.1";
        }

        struct hostent* hostinfo = gethostbyname(hostname);
        if (hostinfo == nullptr) {
            return "127.0.0.1";
        }

        // 返回第一个IPv4地址
        struct in_addr* addr = (struct in_addr*)hostinfo->h_addr_list[0];
        return inet_ntoa(*addr);
    }
};

inline int openMyRoom() {
    RoomListener listener;
    listener.startListening();
    return 0;
}
#endif