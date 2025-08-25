// Copyright (c) 2009 Satoshi Nakamoto
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_COMPAT_H
#define BITCOIN_COMPAT_H

#ifdef WIN32
    // Windows 特定包含
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
    #include <io.h>
    #include <process.h>
    #include <malloc.h>
    
    // Windows 类型定义
    typedef int socklen_t;
    
#else
    // Unix/Linux/macOS 包含
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
    #include <fcntl.h>
    #include <signal.h>
    #include <pthread.h>
    #include <cctype>  // for tolower
    #include <sys/ioctl.h>  // for ioctl
    
    #ifdef __APPLE__
        #include <mach/mach_time.h>
        #include <sys/sysctl.h>
    #else
        #include <time.h>
    #endif
    #include <climits>  // for LLONG_MAX
    
    // Unix 兼容性定义
    #define closesocket(s) close(s)
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define SOCKET int
    #define WSAGetLastError() errno
    #define WSAEWOULDBLOCK EWOULDBLOCK
    #define WSAEMSGSIZE EMSGSIZE
    #define WSAEINTR EINTR
    #define WSAEINPROGRESS EINPROGRESS
    #define WSAEADDRINUSE EADDRINUSE
    #define WSAENOTSOCK EBADF
    #define WSAECONNREFUSED ECONNREFUSED
    #define WSAENOTCONN ENOTCONN
    #define WSAECONNRESET ECONNRESET
    #define WSAECONNABORTED ECONNABORTED
    #define WSAENETDOWN ENETDOWN
    #define WSAENETUNREACH ENETUNREACH
    #define WSAEHOSTUNREACH EHOSTUNREACH
    #define WSAESHUTDOWN ESHUTDOWN
    #define WSAETIMEDOUT ETIMEDOUT
    
    // 睡眠函数
    #define Sleep(n) usleep((n)*1000)
    
    // 线程函数
    #define _beginthreadex(a,b,c,d,e,f) pthread_create(f,NULL,c,d)
    
    // 临界区 (互斥锁)
    typedef pthread_mutex_t CRITICAL_SECTION;
    #define InitializeCriticalSection(cs) pthread_mutex_init(cs, NULL)
    #define DeleteCriticalSection(cs) pthread_mutex_destroy(cs)
    #define EnterCriticalSection(cs) pthread_mutex_lock(cs)
    #define LeaveCriticalSection(cs) pthread_mutex_unlock(cs)
    
    // TryEnterCriticalSection 的Unix实现
    inline bool TryEnterCriticalSection(CRITICAL_SECTION* cs)
    {
        return pthread_mutex_trylock(cs) == 0;
    }
    
    // 路径和字符串函数
    #define MAX_PATH 1024
    #define _vsnprintf vsnprintf
    #define _snprintf snprintf
    #define strlwr(psz) to_lower(psz)
    #define _strlwr(psz) to_lower(psz)
    #define _strdup strdup
    
    // 字符串转换函数
    inline char* to_lower(char* psz)
    {
        char* pszRet = psz;
        while (*psz)
        {
            *psz = tolower(*psz);
            psz++;
        }
        return pszRet;
    }
    
    // 文件操作
    #define _open open
    #define _close close
    #define _read read
    #define _write write
    #define _lseek lseek
    
    // 目录操作
    #include <sys/stat.h>
    #define _mkdir(path) mkdir(path, 0755)
    
    // 堆检查函数 (Windows特定)
    #define _HEAPOK 0
    inline int _heapchk() { return _HEAPOK; }  // 在Unix上总是返回OK
    inline void DebugBreak() { /* 在Unix上什么都不做 */ }
    
    // 线程优先级函数 (Windows特定)
    #define THREAD_PRIORITY_LOWEST -2
    #define THREAD_PRIORITY_BELOW_NORMAL -1
    #define THREAD_PRIORITY_NORMAL 0
    #define THREAD_PRIORITY_ABOVE_NORMAL 1
    #define THREAD_PRIORITY_HIGHEST 2
    
    inline pthread_t GetCurrentThread() { return pthread_self(); }
    inline int SetThreadPriority(pthread_t thread, int priority) {
        // 在Unix上，线程优先级设置比较复杂，这里简化为无操作
        // 实际应用中可以使用pthread_setschedparam
        return 0;
    }
    
    // Windows 互斥锁和同步函数
    #define INFINITE 0xFFFFFFFF
    
    // 确保HANDLE、DWORD和WORD类型已定义
    #ifndef HANDLE
    typedef void* HANDLE;
    #endif
    #ifndef DWORD
    typedef unsigned int DWORD;
    #endif
    #ifndef WORD
    typedef unsigned short WORD;
    #endif
    
    inline HANDLE CreateMutex(void* lpMutexAttributes, bool bInitialOwner, const char* lpName) {
        pthread_mutex_t* mutex = new pthread_mutex_t;
        pthread_mutex_init(mutex, NULL);
        return (HANDLE)mutex;
    }
    
    inline int WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) {
        pthread_mutex_t* mutex = (pthread_mutex_t*)hHandle;
        return pthread_mutex_lock(mutex);
    }
    
    inline int ReleaseMutex(HANDLE hMutex) {
        pthread_mutex_t* mutex = (pthread_mutex_t*)hMutex;
        return pthread_mutex_unlock(mutex);
    }
    
    inline int CloseHandle(HANDLE hObject) {
        pthread_mutex_t* mutex = (pthread_mutex_t*)hObject;
        pthread_mutex_destroy(mutex);
        delete mutex;
        return 1;
    }
    
    // 性能计数器函数 (Windows特定)
    typedef struct {
        long long QuadPart;
    } LARGE_INTEGER;
    
    inline int QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount) {
        #ifdef __APPLE__
            // macOS使用mach_absolute_time
            lpPerformanceCount->QuadPart = mach_absolute_time();
        #else
            // Linux使用clock_gettime
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            lpPerformanceCount->QuadPart = ts.tv_sec * 1000000000LL + ts.tv_nsec;
        #endif
        return 1;
    }
    
    // 常量定义
    #define _I64_MAX LLONG_MAX
    #define _UI64_MAX ULLONG_MAX
    
    // Windows 注册表函数 (简化实现)
    #define HKEY_PERFORMANCE_DATA ((void*)0x80000004)
    #define ERROR_SUCCESS 0
    
    inline long RegQueryValueEx(void* hKey, const char* lpValueName, void* lpReserved, 
                               void* lpType, unsigned char* lpData, unsigned long* lpcbData) {
        // 在Unix上返回失败，因为没有注册表
        return -1;
    }
    
    inline long RegCloseKey(void* hKey) {
        return 0;
    }
    
    // Windows 模块函数
    inline DWORD GetModuleFileName(void* hModule, char* lpFilename, DWORD nSize) {
        // 在Unix上返回程序路径的简化版本
        strncpy(lpFilename, "bitcoin", nSize);
        return strlen(lpFilename);
    }
    
    // OpenSSL Windows特定函数
    inline void RAND_screen() {
        // 在Unix上什么都不做，OpenSSL会使用其他熵源
    }
    
    // Windows GUI函数
    typedef void* HWND;
    #define SW_RESTORE 9
    
    inline HWND FindWindow(const char* lpClassName, const char* lpWindowName) {
        // 在Unix上返回NULL，因为没有Windows窗口管理
        return NULL;
    }
    
    inline int ShowWindow(HWND hWnd, int nCmdShow) {
        // 在Unix上什么都不做
        return 0;
    }
    
    inline void ExitProcess(unsigned int uExitCode) {
        exit(uExitCode);
    }
    
    // 字符串比较函数
    #define stricmp strcasecmp
    
    // 格式化字符串兼容性 - Windows使用%I64d，Unix使用%lld
    #define PRId64 "lld"
    #define PRIu64 "llu"
    #define PRIx64 "llx"
    
    // Windows 网络函数
    #define FIONBIO 0x8004667e
    #define NO_ERROR 0
    #define MAKEWORD(a,b) ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
    
    // Windows 线程函数
    inline int _beginthread(void (*start_address)(void*), unsigned stack_size, void* arglist) {
        pthread_t thread;
        return pthread_create(&thread, NULL, (void*(*)(void*))start_address, arglist);
    }
    
    inline void _endthread() {
        pthread_exit(NULL);
    }
    
    // Windows 网络类型
    typedef struct {
        WORD wVersion;
        WORD wHighVersion;
        char szDescription[257];
        char szSystemStatus[129];
        unsigned short iMaxSockets;
        unsigned short iMaxUdpDg;
        char* lpVendorInfo;
    } WSADATA;
    
    // ioctlsocket 兼容性
    inline int ioctlsocket(int fd, unsigned long cmd, unsigned long* argp) {
        return ioctl(fd, cmd, argp);
    }
    
    // 其他兼容性定义
    #ifndef SOCKET_EINVAL
    #define SOCKET_EINVAL EINVAL
    #endif
    
#endif // WIN32

// 跨平台类型定义
#ifndef WIN32
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef void* HANDLE;
#define WINAPI
#endif

// 64位整数类型
#ifdef WIN32
    typedef __int64 int64;
    typedef unsigned __int64 uint64;
#else
    typedef long long int64;
    typedef unsigned long long uint64;
#endif

// 网络初始化函数
inline int WSAStartup(WORD wVersionRequested, void* lpWSAData)
{
#ifdef WIN32
    return ::WSAStartup(wVersionRequested, (WSADATA*)lpWSAData);
#else
    return 0; // Unix系统不需要初始化
#endif
}

inline int WSACleanup()
{
#ifdef WIN32
    return ::WSACleanup();
#else
    return 0; // Unix系统不需要清理
#endif
}

#endif // BITCOIN_COMPAT_H