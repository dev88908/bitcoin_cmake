// 简单的兼容性测试程序
// 用于验证平台兼容性头文件是否正常工作

#include "src/compat.h"
#include <iostream>

int main()
{
    std::cout << "Bitcoin 兼容性测试" << std::endl;
    
    // 测试基本类型定义
    std::cout << "int64 size: " << sizeof(int64) << " bytes" << std::endl;
    std::cout << "uint64 size: " << sizeof(uint64) << " bytes" << std::endl;
    
    // 测试网络初始化
    char wsaData[256];
    int result = WSAStartup(0x0202, wsaData);
    std::cout << "WSAStartup result: " << result << std::endl;
    
    // 测试套接字创建
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock != INVALID_SOCKET) {
        std::cout << "Socket creation: SUCCESS" << std::endl;
        closesocket(sock);
    } else {
        std::cout << "Socket creation: FAILED" << std::endl;
    }
    
    // 测试临界区
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);
    EnterCriticalSection(&cs);
    std::cout << "Critical section: SUCCESS" << std::endl;
    LeaveCriticalSection(&cs);
    DeleteCriticalSection(&cs);
    
    // 清理网络
    WSACleanup();
    
    std::cout << "所有兼容性测试完成!" << std::endl;
    return 0;
}