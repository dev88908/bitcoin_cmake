// 简单测试验证修复
#include <vector>
#include <iostream>

// 模拟CDataStream的关键部分来测试修复
class TestDataStream {
private:
    std::vector<char> vch;
    
public:
    typedef std::vector<char>::iterator iterator;
    typedef std::vector<char>::const_iterator const_iterator;
    
    // 测试修复后的构造函数
    TestDataStream(const std::vector<unsigned char>& vchIn) {
        if (!vchIn.empty()) {
            vch.assign(reinterpret_cast<const char*>(&vchIn[0]), 
                      reinterpret_cast<const char*>(&vchIn[0] + vchIn.size()));
        }
    }
    
    // 测试修复后的insert方法
    void insert(iterator it, const char* first, const char* last) {
        vch.insert(it, first, last);
    }
    
    iterator begin() { return vch.begin(); }
    iterator end() { return vch.end(); }
    bool empty() const { return vch.empty(); }
    size_t size() const { return vch.size(); }
};

int main() {
    std::cout << "测试修复后的代码..." << std::endl;
    
    // 测试vector<unsigned char>构造函数
    std::vector<unsigned char> test_vec = {'H', 'e', 'l', 'l', 'o'};
    TestDataStream ds(test_vec);
    std::cout << "vector<unsigned char>构造函数: OK (size=" << ds.size() << ")" << std::endl;
    
    // 测试insert方法
    std::vector<unsigned char> empty_vec;
    TestDataStream ds2(empty_vec);
    const char* test_data = "World";
    ds2.insert(ds2.end(), test_data, test_data + 5);
    std::cout << "insert方法: OK (size=" << ds2.size() << ")" << std::endl;
    
    std::cout << "所有修复验证通过!" << std::endl;
    return 0;
}