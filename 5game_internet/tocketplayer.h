// Disable deprecated API warnings
#ifndef LAN_BROADCAST_H
#define LAN_BROADCAST_H
#define _WINSOCK_DEPRECATED_NO_WARNINGS



#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>

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
#define BUFFER_SIZE 1024
#define BROADCAST_ADDRESS "255.255.255.255"

class LANBroadcast {
private:
    WSADATA wsaData;
    SOCKET sendSocket;      // 用于发送广播的套接字
    SOCKET receiveSocket;   // 用于接收响应的套接字
    sockaddr_in broadcastAddr;
    bool isInitialized;

public:
    // 构造函数
    LANBroadcast() : isInitialized(false) {
        // 初始化Winsock
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup 失败: " << WSAGetLastError() << std::endl;
            return;
        }

        // 创建发送广播的UDP套接字
        sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sendSocket == INVALID_SOCKET) {
            std::cerr << "创建发送套接字失败: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }

        // 设置发送套接字为广播模式
        BOOL broadcastEnable = TRUE;
        if (setsockopt(sendSocket, SOL_SOCKET, SO_BROADCAST,
            (const char*)&broadcastEnable, sizeof(broadcastEnable)) != 0) {
            std::cerr << "设置广播模式失败: " << WSAGetLastError() << std::endl;
            closesocket(sendSocket);
            WSACleanup();
            return;
        }

        // 设置广播地址和端口
        memset(&broadcastAddr, 0, sizeof(broadcastAddr));
        broadcastAddr.sin_family = AF_INET;
        broadcastAddr.sin_port = htons(BROADCAST_PORT);
        broadcastAddr.sin_addr.s_addr = inet_addr(BROADCAST_ADDRESS);

        // 创建接收响应的UDP套接字
        receiveSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (receiveSocket == INVALID_SOCKET) {
            std::cerr << "创建接收套接字失败: " << WSAGetLastError() << std::endl;
            closesocket(sendSocket);
            WSACleanup();
            return;
        }

        // 设置接收套接字的重用地址选项，避免端口占用问题
        int reuseAddr = 1;
        if (setsockopt(receiveSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&reuseAddr, sizeof(reuseAddr)) != 0) {
            std::cerr << "设置重用地址失败: " << WSAGetLastError() << std::endl;
        }

        // 绑定接收套接字到特定端口，以便接收响应
        sockaddr_in recvAddr;
        memset(&recvAddr, 0, sizeof(recvAddr));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(BROADCAST_PORT); // 绑定到与广播相同的端口
        recvAddr.sin_addr.s_addr = INADDR_ANY;     // 监听所有网络接口

        if (bind(receiveSocket, (sockaddr*)&recvAddr, sizeof(recvAddr)) != 0) {
            std::cerr << "绑定接收套接字失败: " << WSAGetLastError() << std::endl;
            closesocket(sendSocket);
            closesocket(receiveSocket);
            WSACleanup();
            return;
        }

        isInitialized = true;
    }

    // 析构函数
    ~LANBroadcast() {
        if (isInitialized) {
            closesocket(sendSocket);
            closesocket(receiveSocket);
            WSACleanup();
        }
    }

    // 发送游戏房间查询广播
    bool sendRoomQuery(const std::string& queryMessage = "ARE_YOU_GAME_ROOM") {
        if (!isInitialized) {
            std::cerr << "广播器未初始化" << std::endl;
            return false;
        }

        // 发送广播消息
        int bytesSent = sendto(sendSocket, queryMessage.c_str(), queryMessage.length(), 0,
            (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));

        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "发送广播失败: " << WSAGetLastError() << std::endl;
            return false;
        }

        std::cout << "已发送游戏房间查询广播，消息: \"" << queryMessage
            << "\"，端口: " << BROADCAST_PORT << std::endl;
        return true;
    }

    // 接收响应 - 修改为返回包含房间IP和TCP端口的信息
    std::string receiveResponse(int timeoutMs = 2000) {
        if (!isInitialized) {
            std::cerr << "广播器未初始化" << std::endl;
            return "";
        }

        // 设置接收超时
        struct timeval timeout;
        timeout.tv_sec = timeoutMs / 1000;
        timeout.tv_usec = (timeoutMs % 1000) * 1000;

        if (setsockopt(receiveSocket, SOL_SOCKET, SO_RCVTIMEO,
            (const char*)&timeout, sizeof(timeout)) != 0) {
            std::cerr << "设置接收超时失败: " << WSAGetLastError() << std::endl;
        }

        char buffer[BUFFER_SIZE];
        sockaddr_in senderAddr;
        int senderAddrSize = sizeof(senderAddr);

        int bytesRead = recvfrom(receiveSocket, buffer, BUFFER_SIZE - 1, 0,
            (sockaddr*)&senderAddr, &senderAddrSize);

        if (bytesRead == SOCKET_ERROR) {
            if (WSAGetLastError() == WSAETIMEDOUT) {
                // 不输出超时信息，避免在循环中产生过多日志
            }
            else {
                std::cerr << "接收数据失败: " << WSAGetLastError() << std::endl;
            }
            return "";
        }

        buffer[bytesRead] = '\0'; // 添加字符串结束符

        // 获取发送者IP地址
        char* senderIP = inet_ntoa(senderAddr.sin_addr);

        // 解析响应内容，提取TCP端口号（假设响应格式为"ROOM_FOUND:端口号"）
        std::string response(buffer);
        std::string tcpPort = "8000"; // 默认TCP端口

        // 查找冒号分隔符，提取端口号
        size_t colonPos = response.find(':');
        if (colonPos != std::string::npos && colonPos + 1 < response.length()) {
            tcpPort = response.substr(colonPos + 1);
        }

        // 构建包含房间IP和TCP端口的返回信息
        std::string roomInfo = "房间IP: " + std::string(senderIP) + ", TCP端口: " + tcpPort;
        std::cout << "接收到来自 " << senderIP << " 的响应: " << response << std::endl;
        std::cout << "游戏房间信息: " << roomInfo << std::endl;

        // 返回包含房间IP、TCP端口和原始响应内容的完整信息
        return roomInfo + ", 响应内容: " + response;
    }

    // 检查是否初始化成功
    bool isReady() const {
        return isInitialized;
    }
};

#endif // LAN_BROADCAST_H