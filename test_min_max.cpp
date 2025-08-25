#include <algorithm>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vRecv = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    // Test the fixed min call
    for (int i = 0; i < std::min(vRecv.size(), static_cast<size_t>(25)); i++) {
        std::cout << vRecv[i] << " ";
    }
    std::cout << std::endl;
    
    // Test other type combinations
    unsigned int nSize = 100;
    unsigned int i = 50;
    unsigned int blk = std::min(nSize - i, static_cast<unsigned int>(1 + 4999999 / sizeof(int)));
    std::cout << "blk = " << blk << std::endl;
    
    std::cout << "min/max type fixes successful!" << std::endl;
    return 0;
}