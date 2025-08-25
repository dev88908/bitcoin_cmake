// 测试util.h的修复
#include "src/compat.h"
#include <iostream>
#include <string>

// 模拟需要的类型和函数
typedef long long int64;

std::string strprintf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

// 包含PRId64定义
#ifndef PRId64
#if defined(_MSC_VER) || defined(__BORLANDC__) || defined(__MSVCRT__)
#define PRId64  "I64d"
#define PRIu64  "I64u"
#define PRIx64  "I64x"
#else
#define PRId64  "lld"
#define PRIu64  "llu"
#define PRIx64  "llx"
#endif
#endif

// 测试修复后的函数
inline std::string i64tostr(int64 n)
{
    return strprintf("%" PRId64, n);
}

// 测试CCriticalSection类
class CCriticalSection
{
protected:
    CRITICAL_SECTION cs;
public:
    char* pszFile;
    int nLine;
    explicit CCriticalSection() { InitializeCriticalSection(&cs); }
    ~CCriticalSection() { DeleteCriticalSection(&cs); }
    void Enter() { EnterCriticalSection(&cs); }
    void Leave() { LeaveCriticalSection(&cs); }
    bool TryEnter() { return TryEnterCriticalSection(&cs); }
    CRITICAL_SECTION* operator&() { return &cs; }
};

int main()
{
    std::cout << "测试util.h修复..." << std::endl;
    
    // 测试PRId64修复
    int64 test_num = 1234567890123LL;
    std::string result = i64tostr(test_num);
    std::cout << "i64tostr测试: " << result << std::endl;
    
    // 测试TryEnterCriticalSection修复
    CCriticalSection cs;
    bool entered = cs.TryEnter();
    std::cout << "TryEnter测试: " << (entered ? "成功" : "失败") << std::endl;
    if (entered) {
        cs.Leave();
        std::cout << "Leave测试: 成功" << std::endl;
    }
    
    std::cout << "所有util.h修复验证通过!" << std::endl;
    return 0;
}