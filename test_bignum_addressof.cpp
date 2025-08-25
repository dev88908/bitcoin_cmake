#include "src/compat.h"
#include <openssl/bn.h>
#include <iostream>
#include <stdexcept>

class bignum_error : public std::runtime_error
{
public:
    explicit bignum_error(const std::string& str) : std::runtime_error(str) {}
};

class CAutoBN_CTX
{
protected:
    BN_CTX* pctx;
    BN_CTX* operator=(BN_CTX* pnew) { return pctx = pnew; }

public:
    CAutoBN_CTX()
    {
        pctx = BN_CTX_new();
        if (pctx == NULL)
            throw bignum_error("CAutoBN_CTX : BN_CTX_new() returned NULL");
    }

    ~CAutoBN_CTX()
    {
        if (pctx != NULL)
            BN_CTX_free(pctx);
    }

    operator BN_CTX*() { return pctx; }
    BN_CTX& operator*() { return *pctx; }
    BN_CTX** operator&() { return &pctx; }
    bool operator!() { return (pctx == NULL); }
};

class CBigNum
{
private:
    BIGNUM* bn;

public:
    CBigNum()
    {
        bn = BN_new();
        if (!bn) throw bignum_error("CBigNum::CBigNum() : BN_new failed");
    }

    CBigNum(unsigned long n)
    {
        bn = BN_new();
        if (!bn) throw bignum_error("CBigNum : BN_new failed");
        if (!BN_set_word(bn, n))
            throw bignum_error("CBigNum conversion from unsigned long : BN_set_word failed");
    }

    ~CBigNum()
    {
        BN_free(bn);
    }

    // Conversion operator to BIGNUM* for OpenSSL functions
    operator BIGNUM*() { return bn; }
    operator const BIGNUM*() const { return bn; }
    
    // Address-of operator to get BIGNUM* for OpenSSL functions that need &object
    BIGNUM* operator&() { return bn; }
    const BIGNUM* operator&() const { return bn; }

    unsigned long getulong() const
    {
        return BN_get_word(bn);
    }
};

int main() {
    try {
        // Test the address-of operator (like in base58.h)
        CBigNum dv, rem, bn(100), bn58(58);
        CAutoBN_CTX pctx;
        
        // This should work now with our address-of operator override
        if (BN_div(&dv, &rem, &bn, &bn58, pctx)) {
            std::cout << "BN_div with address-of operator works!" << std::endl;
            std::cout << "100 / 58 = " << dv.getulong() << " remainder " << rem.getulong() << std::endl;
        } else {
            std::cout << "BN_div failed" << std::endl;
            return 1;
        }
        
        // Test BN_mul as well
        CBigNum result;
        if (BN_mul(&result, &bn, &bn58, pctx)) {
            std::cout << "BN_mul with address-of operator works!" << std::endl;
            std::cout << "100 * 58 = " << result.getulong() << std::endl;
        } else {
            std::cout << "BN_mul failed" << std::endl;
            return 1;
        }
        
        std::cout << "All address-of operator tests passed!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}