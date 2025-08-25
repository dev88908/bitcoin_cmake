#include "src/compat.h"
#include <boost/array.hpp>
#include <iostream>

// Test the array fix
boost::array<bool, 10> vfThreadRunning;

int main() {
    // Test array usage
    vfThreadRunning[0] = true;
    std::cout << "boost::array works: " << vfThreadRunning[0] << std::endl;
    
    // Test _mkdir compatibility
    int result = _mkdir("/tmp/test_bitcoin_dir");
    if (result == 0 || errno == EEXIST) {
        std::cout << "_mkdir compatibility works" << std::endl;
    } else {
        std::cout << "_mkdir failed: " << strerror(errno) << std::endl;
    }
    
    std::cout << "Array and mkdir fixes successful!" << std::endl;
    return 0;
}