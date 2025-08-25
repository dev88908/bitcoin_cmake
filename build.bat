@echo off
REM Bitcoin Windows 构建脚本
REM 用法: build.bat [debug|release] [clean]

set BUILD_TYPE=Debug
set CLEAN_BUILD=false

REM 解析命令行参数
:parse_args
if "%1"=="debug" (
    set BUILD_TYPE=Debug
    shift
    goto parse_args
)
if "%1"=="release" (
    set BUILD_TYPE=Release
    shift
    goto parse_args
)
if "%1"=="clean" (
    set CLEAN_BUILD=true
    shift
    goto parse_args
)
if not "%1"=="" (
    echo 用法: %0 [debug^|release] [clean]
    exit /b 1
)

echo 构建配置: %BUILD_TYPE%

REM 创建构建目录
set BUILD_DIR=build
if "%CLEAN_BUILD%"=="true" (
    echo 清理构建目录...
    if exist "%BUILD_DIR%" rmdir /s /q "%BUILD_DIR%"
)
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

cd "%BUILD_DIR%"

REM 运行CMake配置
echo 配置项目...
cmake .. -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_MODULE_PATH=../cmake -G "MinGW Makefiles"

if errorlevel 1 (
    echo CMake配置失败!
    exit /b 1
)

REM 编译项目
echo 编译项目...
mingw32-make

if errorlevel 1 (
    echo 编译失败!
    exit /b 1
) else (
    echo 编译成功! 可执行文件位于: %BUILD_DIR%\bitcoin.exe
)