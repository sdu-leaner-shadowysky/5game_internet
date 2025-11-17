// 定义全局变量（仅在此处定义一次，避免重定义）
#include "server1.h"

std::thread g_roomThread;          // 运行openMyRoom的线程
RoomListener* g_roomListener = nullptr;  // RoomListener实例指针