#include <stdio.h>
#include "src/compat.h"

int main() {
    // Test if the compatibility functions are available
    pthread_t thread = GetCurrentThread();
    SetThreadPriority(thread, THREAD_PRIORITY_NORMAL);
    
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    
    printf("Compatibility test passed\n");
    return 0;
}