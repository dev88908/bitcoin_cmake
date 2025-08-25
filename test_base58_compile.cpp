// Test compilation of base58.h with our bignum fixes
#include "src/compat.h"

// Minimal definitions needed for base58.h
#define VERSION 1
typedef long long int64;
typedef unsigned long long uint64;

// Forward declaration for uint256 (we don't need the full implementation)
class uint256 {
public:
    uint256() {}
    uint256(unsigned int n) {}
};

#include "src/bignum.h"

// Now include base58.h to test if it compiles
#include "src/base58.h"

int main() {
    std::cout << "base58.h compiled successfully with bignum fixes!" << std::endl;
    return 0;
}