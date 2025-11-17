#pragma once
// Disable deprecated API warnings
#ifndef CLIENT_H
#define CLIENT_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <utility> // 用于std::pair

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
#define RECEIVE_PORT 9846  // 接收响应的端口
#define BUFFER_SIZE 1024
#define BROADCAST_ADDRESS "255.255.255.255"

class RoomQuerySender {
private:
    WSADATA wsaData;
    SOCKET sendSocket;
    SOCKET receiveSocket;
    sockaddr_in broadcastAddr;
    bool isInitialized;
    bool isRunning;          // 线程运行标志
    bool hasFirstRoom;       // 是否已获取第一个房间
    std::pair<std::string, std::string> firstRoom; // 第一个房间：first=IP，second=端口
    std::mutex responseMutex; // 保护共享资源的互斥锁

public:
    RoomQuerySender() : isInitialized(false), isRunning(false), hasFirstRoom(false) {
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

        // 设置接收套接字的重用地址选项
        int reuseAddr = 1;
        if (setsockopt(receiveSocket, SOL_SOCKET, SO_REUSEADDR,
            (const char*)&reuseAddr, sizeof(reuseAddr)) != 0) {
            std::cerr << "设置重用地址失败: " << WSAGetLastError() << std::endl;
        }

        // 绑定接收套接字到指定端口
        sockaddr_in recvAddr;
        memset(&recvAddr, 0, sizeof(recvAddr));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(RECEIVE_PORT);
        recvAddr.sin_addr.s_addr = INADDR_ANY;

        if (bind(receiveSocket, (sockaddr*)&recvAddr, sizeof(recvAddr)) != 0) {
            std::cerr << "绑定接收套接字失败: " << WSAGetLastError() << std::endl;
            closesocket(sendSocket);
            closesocket(receiveSocket);
            WSACleanup();
            return;
        }

        isInitialized = true;
    }

    ~RoomQuerySender() {
        stopReceiver(); // 确保接收线程停止
        if (isInitialized) {
            closesocket(sendSocket);
            closesocket(receiveSocket);
            WSACleanup();
        }
    }

    // 发送单个查询广播
    bool sendSingleQuery(const std::string& queryMessage = "是否存在游戏房间") {
        if (!isInitialized) {
            std::cerr << "发送器未初始化" << std::endl;
            return false;
        }

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

    // 发送多次查询广播
    void sendMultipleQueries(int count = 3, int intervalMs = 1000) {
        for (int i = 0; i < count; i++) {
            sendSingleQuery();
            if (i < count - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            }
        }
    }

    // 接收单个响应（只处理第一个有效响应）
    std::string receiveResponse(int timeoutMs = 2000) {
        if (!isInitialized || hasFirstRoom) { // 已获取第一个房间则不再处理
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
            if (WSAGetLastError() != WSAETIMEDOUT) {
                std::cerr << "接收数据失败: " << WSAGetLastError() << std::endl;
            }
            return "";
        }

        buffer[bytesRead] = '\0'; // 添加字符串结束符
        char* senderIP = inet_ntoa(senderAddr.sin_addr); // 提取IP地址
        int senderPort = ntohs(senderAddr.sin_port);

        std::string rawResponse(buffer);
        std::string tcpPort = "8000"; // 默认TCP端口

        // 解析响应内容，提取TCP端口号（格式假设为"房间信息:端口号"）
        size_t colonPos = rawResponse.find(':');
        if (colonPos != std::string::npos && colonPos + 1 < rawResponse.length()) {
            tcpPort = rawResponse.substr(colonPos + 1);
        }

        // 构建详细的房间信息
        std::string detailedInfo = "==========================================\n";
        detailedInfo += "接收到第一个游戏房间响应！\n";
        detailedInfo += "发送者IP地址: " + std::string(senderIP) + "\n";
        detailedInfo += "发送者端口号: " + std::to_string(senderPort) + "\n";
        detailedInfo += "原始响应内容: " + rawResponse + "\n";
        detailedInfo += "解析的TCP端口: " + tcpPort + "\n";
        detailedInfo += "TCP连接目标: " + std::string(senderIP) + ":" + tcpPort + "\n";
        detailedInfo += "==========================================";

        // 输出详细信息到控制台
        std::cout << "\n" << detailedInfo << "\n" << std::endl;

        // 存储第一个房间的IP和端口
        std::lock_guard<std::mutex> lock(responseMutex);
        if (!hasFirstRoom) { // 双重检查，确保只存储第一个
            firstRoom = std::make_pair(senderIP, tcpPort);
            hasFirstRoom = true;
            stopReceiver(); // 找到第一个后停止接收线程
        }

        return detailedInfo;
    }

    // 启动接收线程
    void startReceiver() {
        if (isRunning) return;
        isRunning = true;
        std::thread(&RoomQuerySender::receiverLoop, this).detach();
    }

    // 停止接收线程
    void stopReceiver() {
        isRunning = false;
    }

    // 接收循环（找到第一个房间后自动停止）
    void receiverLoop() {
        std::cout << "响应接收线程已启动，正在监听第一个房间响应...\n";
        while (isRunning && !hasFirstRoom) { // 未找到第一个房间时持续监听
            receiveResponse(2000);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        std::cout << "响应接收线程已停止（已找到第一个房间或手动终止）\n";
    }

    // 获取第一个房间的IP和端口（返回pair）
    std::pair<std::string, std::string> getFirstRoom() {
        std::lock_guard<std::mutex> lock(responseMutex);
        return firstRoom;
    }

    // 检查是否已找到第一个房间
    bool hasFoundFirstRoom() {
        std::lock_guard<std::mutex> lock(responseMutex);
        return hasFirstRoom;
    }

    bool isReady() const {
        return isInitialized;
    }
};

// 返回类型为pair<string, string>，仅返回第一个房间的IP和端口（空表示未找到）
std::pair<std::string, std::string> findOutRoom() {
    std::cout << "==========================================\n";
    std::cout << "        游戏房间查询客户端启动           \n";
    std::cout << "        （仅返回第一个发现的房间）       \n";
    std::cout << "==========================================\n";

    RoomQuerySender sender;
    std::pair<std::string, std::string> firstRoom;

    if (sender.isReady()) {
        std::cout << "客户端初始化成功！\n";
        std::cout << "正在启动响应接收线程...\n";

        // 启动接收线程
        sender.startReceiver();

        // 短暂延迟确保接收线程已启动
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "\n开始发送游戏房间查询广播...\n";
        // 发送多次广播查询
        sender.sendMultipleQueries(3, 1000);

        std::cout << "\n==========================================\n";
        std::cout << "查询广播已发送完成！\n";
        std::cout << "正在等待第一个房间响应...\n";
        std::cout << "按Enter键强制结束监听并退出...\n";
        std::cout << "==========================================\n";

        // 等待用户输入或直到找到第一个房间
        while (!sender.hasFoundFirstRoom()) {
            if (std::cin.peek() != EOF) { // 检测到用户输入
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // 停止接收线程
        sender.stopReceiver();
        // 等待线程清理
        std::this_thread::sleep_for(std::chrono::milliseconds(300));

        // 获取第一个房间的信息
        firstRoom = sender.getFirstRoom();

        std::cout << "\n==========================================\n";
        std::cout << "              程序结束总结               \n";
        std::cout << "==========================================\n";

        if (!firstRoom.first.empty() && !firstRoom.second.empty()) {
            std::cout << "已找到第一个游戏房间：\n";
            std::cout << "IP地址: " << firstRoom.first << "\n";
            std::cout << "TCP端口: " << firstRoom.second << "\n";
        }
        else {
            std::cout << "未找到任何游戏房间\n";
            std::cout << "请检查以下事项:\n";
            std::cout << "1. 游戏房间服务是否正在运行\n";
            std::cout << "2. 防火墙是否阻止了UDP广播\n";
            std::cout << "3. 是否在同一个局域网内\n";
        }
        std::cout << "==========================================\n";
    }
    else {
        std::cerr << "客户端初始化失败！请检查网络环境\n";
    }

    std::cout << "按Enter键退出程序...";
    //std::cin.get();

    return firstRoom;
}

#endif