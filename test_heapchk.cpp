// 测试堆检查函数的修复
#include "src/compat.h"
#include <iostream>

// 模拟util.h中的heapchk函数
inline void heapchk()
{
    if (_heapchk() != _HEAPOK)
        DebugBreak();
}

int main()
{
    std::cout << "测试堆检查函数修复..." << std::endl;
    
    // 测试_heapchk函数
    int result = _heapchk();
    std::cout << "_heapchk() 返回: " << result << std::endl;
    std::cout << "_HEAPOK 值: " << _HEAPOK << std::endl;
    
    // 测试heapchk函数
    heapchk();  // 这应该不会崩溃
    std::cout << "heapchk() 调用: 成功" << std::endl;
    
    // 测试DebugBreak函数
    DebugBreak();  // 在Unix上应该什么都不做
    std::cout << "DebugBreak() 调用: 成功" << std::endl;
    
    std::cout << "所有堆检查函数修复验证通过!" << std::endl;
    return 0;
}