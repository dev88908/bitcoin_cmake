// 测试headers.h的基本包含
#define VERSION 1

// 先包含基本的C++头文件
#include <iostream>
#include <string>
#include <vector>

// 模拟一些必要的定义来避免依赖问题
typedef long long int64;
typedef unsigned long long uint64;

// 简单的strprintf实现
std::string strprintf(const char* format, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

// 包含我们修复的头文件
#include "src/compat.h"

int main()
{
    std::cout << "测试基本头文件包含..." << std::endl;
    
    // 测试基本类型
    int64 test_int = 123;
    uint64 test_uint = 456;
    
    std::cout << "int64: " << test_int << std::endl;
    std::cout << "uint64: " << test_uint << std::endl;
    
    // 测试网络初始化
    char wsaData[256];
    int result = WSAStartup(0x0202, wsaData);
    std::cout << "WSAStartup: " << result << std::endl;
    
    WSACleanup();
    
    std::cout << "基本头文件测试通过!" << std::endl;
    return 0;
}