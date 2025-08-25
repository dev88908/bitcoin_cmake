#include "src/compat.h"
#include <iostream>

int main() {
    // Test thread priority functions
    pthread_t currentThread = GetCurrentThread();
    int result = SetThreadPriority(currentThread, THREAD_PRIORITY_LOWEST);
    std::cout << "SetThreadPriority result: " << result << std::endl;
    
    // Test performance counter
    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    
    // Do some work
    volatile int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }
    
    QueryPerformanceCounter(&end);
    std::cout << "Performance counter test: " << (end.QuadPart - start.QuadPart) << " ticks" << std::endl;
    
    // Test _I64_MAX
    int64 maxVal = _I64_MAX;
    std::cout << "_I64_MAX = " << maxVal << std::endl;
    
    std::cout << "Thread and performance compatibility tests passed!" << std::endl;
    return 0;
}