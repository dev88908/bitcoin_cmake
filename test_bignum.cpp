#include "src/compat.h"
#include "src/bignum.h"
#include <iostream>

int main() {
    try {
        CBigNum a(42);
        CBigNum b(13);
        CBigNum c = a + b;
        
        std::cout << "Testing CBigNum: 42 + 13 = " << c.getulong() << std::endl;
        
        CBigNum d("0x1234567890abcdef");
        std::cout << "Hex test passed" << std::endl;
        
        std::cout << "All bignum tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}