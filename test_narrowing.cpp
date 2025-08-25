#include <iostream>

// Test the narrowing fix
static const unsigned char pchMessageStart[4] = { 0xf9, 0xbe, 0xb4, 0xd9 };

int main() {
    std::cout << "Testing narrowing fix..." << std::endl;
    std::cout << "pchMessageStart values: ";
    for (int i = 0; i < 4; i++) {
        std::cout << "0x" << std::hex << (int)pchMessageStart[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Narrowing fix successful!" << std::endl;
    return 0;
}