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
#include <vector>
#include <mutex>

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
#define RECEIVE_PORT 9846  // 新增：接收响应的端口
#define BUFFER_SIZE 1024
#define BROADCAST_ADDRESS "255.255.255.255"

class RoomQuerySender {
private:
    WSADATA wsaData;
    SOCKET sendSocket;
    SOCKET receiveSocket;
    sockaddr_in broadcastAddr;
    bool isInitialized;
    std::vector<std::string> receivedResponses; // 存储接收到的所有响应
    std::mutex responseMutex; // 用于保护共享资源

public:
    RoomQuerySender() : isInitialized(false) {
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

        // 绑定接收套接字到临时端口
        sockaddr_in recvAddr;
        memset(&recvAddr, 0, sizeof(recvAddr));
        recvAddr.sin_family = AF_INET;
        recvAddr.sin_port = htons(RECEIVE_PORT); // 使用9846端口
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
        if (isInitialized) {
            closesocket(sendSocket);
            closesocket(receiveSocket);
            WSACleanup();
        }
    }

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

    void sendMultipleQueries(int count = 3, int intervalMs = 1000) {
        for (int i = 0; i < count; i++) {
            sendSingleQuery();
            if (i < count - 1) {
                std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
            }
        }
    }

    std::string receiveResponse(int timeoutMs = 2000, bool connectToRoom = false) {
        if (!isInitialized) {
            std::cerr << "发送器未初始化" << std::endl;
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
                // 不输出超时信息
            }
            else {
                std::cerr << "接收数据失败: " << WSAGetLastError() << std::endl;
            }
            return "";
        }

        buffer[bytesRead] = '\0'; // 添加字符串结束符
        char* senderIP = inet_ntoa(senderAddr.sin_addr);
        int senderPort = ntohs(senderAddr.sin_port); // 获取发送者端口号

        std::string rawResponse(buffer);
        std::string tcpPort = "8000"; // 默认TCP端口

        // 解析响应内容，提取TCP端口号
        size_t colonPos = rawResponse.find(':');
        if (colonPos != std::string::npos && colonPos + 1 < rawResponse.length()) {
            tcpPort = rawResponse.substr(colonPos + 1);
        }

        // 如果需要自动连接，尝试建立TCP连接
        // if (connectToRoom) {
        //     SOCKET tcpSocket = connectToGameRoom(senderIP, tcpPort);
        //     if (tcpSocket != INVALID_SOCKET) {
        //         std::cout << "TCP连接建立成功，可以开始游戏通信了！" << std::endl;
        //         // 这里可以添加TCP通信逻辑
        //         closesocket(tcpSocket); // 测试连接后关闭，实际应用中应该保留连接
        //     }
        // }

        // 构建详细的房间信息，包括所有接收到的内容
        std::string detailedInfo = "==========================================\n";
        detailedInfo += "接收到游戏房间响应！\n";
        detailedInfo += "发送者IP地址: " + std::string(senderIP) + "\n";
        detailedInfo += "发送者端口号: " + std::to_string(senderPort) + "\n";
        detailedInfo += "原始响应内容: " + rawResponse + "\n";
        detailedInfo += "解析的TCP端口: " + tcpPort + "\n";
        detailedInfo += "TCP连接目标: " + std::string(senderIP) + ":" + tcpPort + "\n";
        detailedInfo += "==========================================";

        // 立即输出接收到的详细信息到控制台
        std::cout << "\n" << detailedInfo << "\n" << std::endl;

        // 存储响应供后续使用
        std::lock_guard<std::mutex> lock(responseMutex);
        receivedResponses.push_back(detailedInfo);

        return detailedInfo;
    }

    // 获取所有接收到的响应
    std::vector<std::string> getAllResponses() {
        std::lock_guard<std::mutex> lock(responseMutex);
        return receivedResponses;
    }

    // 获取响应数量
    int getResponseCount() {
        std::lock_guard<std::mutex> lock(responseMutex);
        return receivedResponses.size();
    }

    bool isReady() const {
        return isInitialized;
    }
};

int findOutRoom() {
    std::cout << "==========================================\n";
    std::cout << "        游戏房间查询客户端启动           \n";
    std::cout << "==========================================\n";

    RoomQuerySender sender;

    if (sender.isReady()) {
        std::cout << "客户端初始化成功！\n";
        std::cout << "正在启动响应接收线程...\n";

        // 创建接收线程
        std::thread receiverThread([&sender]() {
            std::cout << "响应接收线程已启动，正在监听局域网响应...\n";
            while (true) {
                std::string response = sender.receiveResponse(2000, true); // 启用自动连接
                if (!response.empty()) {
                    // 响应内容已经在receiveResponse方法中详细输出
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            });

        // 短暂延迟确保接收线程已启动
        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        std::cout << "\n开始发送游戏房间查询广播...\n";
        // 发送多次广播查询
        sender.sendMultipleQueries(3, 1000);

        std::cout << "\n==========================================\n";
        std::cout << "查询广播已发送完成！\n";
        std::cout << "继续监听响应中...\n";
        std::cout << "按Enter键查看统计信息并退出...\n";
        std::cout << "==========================================\n";

        // 等待用户输入
        std::cin.get();

        // 显示所有接收到的响应统计
        int responseCount = sender.getResponseCount();
        auto allResponses = sender.getAllResponses();

        std::cout << "\n==========================================\n";
        std::cout << "              程序结束总结               \n";
        std::cout << "==========================================\n";
        std::cout << "总共接收到 " << responseCount << " 个游戏房间响应\n";

        if (responseCount > 0) {
            std::cout << "\n所有接收到的响应详情:\n";
            for (size_t i = 0; i < allResponses.size(); i++) {
                std::cout << "\n[响应 " << (i + 1) << "/" << responseCount << "]\n";
                std::cout << allResponses[i] << "\n";
            }
        }
        else {
            std::cout << "\n未接收到任何游戏房间响应\n";
            std::cout << "请检查以下事项:\n";
            std::cout << "1. 游戏房间服务是否正在运行\n";
            std::cout << "2. 防火墙是否阻止了UDP广播\n";
            std::cout << "3. 是否在同一个局域网内\n";
        }
        std::cout << "==========================================\n";

        // 分离线程，让它在程序退出时自动结束
        receiverThread.detach();
    }
    else {
        std::cerr << "客户端初始化失败！请检查网络环境\n";
    }

    std::cout << "按Enter键退出程序...";
    std::cin.get();

    return 0;
}

#endif