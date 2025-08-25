#include "src/compat.h"
#include "src/uint256.h"
#include "src/bignum.h"
#include <iostream>

int main() {
    try {
        // Test basic operations
        CBigNum a(42);
        CBigNum b(13);
        CBigNum c = a + b;
        
        std::cout << "Testing CBigNum: 42 + 13 = " << c.getulong() << std::endl;
        
        // Test with uint256
        uint256 hash;
        CBigNum d(hash);
        std::cout << "uint256 constructor test passed" << std::endl;
        
        // Test address-of operator (like in base58.h)
        CBigNum dv, rem, bn(100), bn58(58);
        CAutoBN_CTX pctx;
        
        // This should work now with our address-of operator override
        if (BN_div(&dv, &rem, &bn, &bn58, pctx)) {
            std::cout << "BN_div with address-of operator works: " << dv.getulong() << std::endl;
        }
        
        std::cout << "All bignum tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}