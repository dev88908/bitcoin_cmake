#!/bin/bash

# Bitcoin 构建脚本
# 用法: ./build.sh [debug|release] [clean]

BUILD_TYPE="Debug"
CLEAN_BUILD=false

# 解析命令行参数
for arg in "$@"; do
    case $arg in
        debug)
            BUILD_TYPE="Debug"
            ;;
        release)
            BUILD_TYPE="Release"
            ;;
        clean)
            CLEAN_BUILD=true
            ;;
        *)
            echo "用法: $0 [debug|release] [clean]"
            exit 1
            ;;
    esac
done

echo "构建配置: $BUILD_TYPE"

# 创建构建目录
BUILD_DIR="build"
if [ "$CLEAN_BUILD" = true ] || [ ! -d "$BUILD_DIR" ]; then
    echo "清理构建目录..."
    rm -rf "$BUILD_DIR"
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# 运行CMake配置
echo "配置项目..."
cmake .. -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
         -DCMAKE_MODULE_PATH="../cmake"

if [ $? -ne 0 ]; then
    echo "CMake配置失败!"
    exit 1
fi

# 编译项目
echo "编译项目..."
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "编译成功! 可执行文件位于: $BUILD_DIR/bitcoin"
else
    echo "编译失败!"
    exit 1
fi