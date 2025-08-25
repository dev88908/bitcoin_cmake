# Bitcoin v0.1.3 构建指南

这是Satoshi Nakamoto原始Bitcoin代码的现代化CMake构建系统。

## 系统要求

### 必需的依赖库

1. **wxWidgets** (GUI框架)
   - Ubuntu/Debian: `sudo apt-get install libwxgtk3.0-gtk3-dev`
   - macOS: `brew install wxwidgets`
   - Windows: 从 https://www.wxwidgets.org/ 下载

2. **OpenSSL** (加密库)
   - Ubuntu/Debian: `sudo apt-get install libssl-dev`
   - macOS: `brew install openssl`
   - Windows: 从 https://slproweb.com/products/Win32OpenSSL.html 下载

3. **Berkeley DB** (数据库)
   - Ubuntu/Debian: `sudo apt-get install libdb++-dev`
   - macOS: `brew install berkeley-db`
   - Windows: 从 https://www.oracle.com/database/berkeley-db/ 下载

4. **Boost** (C++库)
   - Ubuntu/Debian: `sudo apt-get install libboost-all-dev`
   - macOS: `brew install boost`
   - Windows: 从 https://www.boost.org/ 下载

### 构建工具

- CMake 3.16+
- C++编译器 (GCC, Clang, 或 MSVC)
- Make 或 Ninja

## 快速构建

### Linux/macOS

```bash
# 调试版本
./build.sh debug

# 发布版本
./build.sh release

# 清理重新构建
./build.sh debug clean
```

### Windows (MinGW)

```cmd
REM 调试版本
build.bat debug

REM 发布版本
build.bat release

REM 清理重新构建
build.bat debug clean
```

## 手动构建

如果你想手动控制构建过程：

```bash
# 创建构建目录
mkdir build && cd build

# 配置项目
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_MODULE_PATH=../cmake

# 编译
make -j$(nproc)  # Linux/macOS
# 或者
mingw32-make     # Windows MinGW
```

## 自定义构建选项

你可以通过CMake变量来自定义构建：

```bash
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local \
  -DwxWidgets_ROOT_DIR=/path/to/wxwidgets \
  -DOPENSSL_ROOT_DIR=/path/to/openssl \
  -DBOOST_ROOT=/path/to/boost
```

## 跨平台兼容性

这个版本的Bitcoin代码已经修改为支持跨平台编译：

### 支持的平台
- **Windows** (MinGW, MSVC)
- **macOS** (Clang)
- **Linux** (GCC, Clang)

### 兼容性特性
- 统一的网络API (Winsock2 vs BSD sockets)
- 跨平台线程支持 (Windows CRITICAL_SECTION vs pthread_mutex)
- 统一的文件I/O接口
- 平台特定的系统调用封装

### 兼容性修复

这个版本修复了原始代码中的现代C++兼容性问题：

- **迭代器转换问题**: 修复了`const char*`到`const_iterator`的非法转换
- **向量构造问题**: 修复了`vector<unsigned char>`构造函数中的指针算术问题
- **MSVC兼容性**: 移除了过时的MSVC 6.0特定代码

### 兼容性测试

你可以运行兼容性测试来验证平台支持：

```bash
# 使用Makefile运行所有测试
make -f Makefile.unix test

# 或者单独编译和运行测试
g++ -o test_compat test_compatibility.cpp -I./src
./test_compat

g++ -o test_serialize test_serialize.cpp -I./src
./test_serialize
```

## 故障排除

### 找不到依赖库

如果CMake找不到某个依赖库，你可以手动指定路径：

```bash
# 指定wxWidgets路径
cmake .. -DwxWidgets_ROOT_DIR=/path/to/wxwidgets

# 指定OpenSSL路径
cmake .. -DOPENSSL_ROOT_DIR=/path/to/openssl

# 指定Berkeley DB路径
cmake .. -DBDB_ROOT=/path/to/berkeleydb
```

### Windows特定问题

1. 确保所有依赖库都是用相同的编译器编译的
2. 如果使用Visual Studio，确保使用正确的架构 (x86/x64)
3. 可能需要设置环境变量 `PATH` 来包含DLL文件

### macOS特定问题

如果使用Homebrew安装的依赖库，可能需要指定路径：

```bash
cmake .. \
  -DOPENSSL_ROOT_DIR=/usr/local/opt/openssl \
  -DBOOST_ROOT=/usr/local/opt/boost
```

## 原始构建系统

如果你想使用原始的Makefile构建系统，可以在 `src/` 目录下运行：

```bash
cd src
make
```

注意：原始构建系统只支持Windows/MinGW环境。

## 许可证

这个项目遵循MIT/X11许可证，详见 `license.txt` 文件。