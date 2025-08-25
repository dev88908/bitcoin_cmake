cmake_minimum_required(VERSION 3.16)
project(bitcoin VERSION 0.1.3 LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 添加自定义模块路径
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# 平台特定设置
if(WIN32)
    add_definitions(-DWIN32 -D__WXMSW__ -D_WINDOWS -DNOPCH)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mthreads -w -Wno-invalid-offsetof -Wformat")
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_definitions(-D__WXDEBUG__)
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
    endif()
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wno-invalid-offsetof -Wformat")

    set(BDB_ROOT /opt/homebrew/Cellar/berkeley-db/18.1.40_2)
else() # Linux
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w -Wno-invalid-offsetof -Wformat")
endif()

# 查找依赖库
# 查找 wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS richtext html core base)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
else()
    message(FATAL_ERROR "wxWidgets not found! Please install wxWidgets development libraries.")
endif()

# 查找 OpenSSL
find_package(OpenSSL REQUIRED)
if(NOT OPENSSL_FOUND)
    message(FATAL_ERROR "OpenSSL not found! Please install OpenSSL development libraries.")
endif()

# 查找 Boost
find_package(Boost)
if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found! Please install Boost development libraries.")
endif()

# 查找 Berkeley DB
include_directories(/opt/homebrew/opt/berkeley-db@4/include)

# 告诉 CMake 去哪里找库文件
link_directories(/opt/homebrew/opt/berkeley-db@4/lib)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${Boost_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIR}
)

# 源文件列表
set(BITCOIN_SOURCES
    src/main.cpp
    src/util.cpp
    src/script.cpp
    src/db.cpp
    src/net.cpp
    src/market.cpp
    src/sha.cpp
    src/irc.cpp
    src/stubs.cpp
)

# 头文件列表
set(BITCOIN_HEADERS
    src/headers.h
    src/compat.h
    src/util.h
    src/main.h
    src/serialize.h
    src/uint256.h
    src/key.h
    src/bignum.h
    src/script.h
    src/db.h
    src/base58.h
    src/net.h
    src/irc.h
    src/market.h
    src/uibase.h
    src/ui.h
    src/sha.h
)

# 创建可执行文件
add_executable(bitcoin ${BITCOIN_SOURCES} ${BITCOIN_HEADERS})

# 链接库
target_link_libraries(bitcoin
    ${wxWidgets_LIBRARIES}
    ${OPENSSL_LIBRARIES}
    ${Boost_LIBRARIES}
)

# 在 target_link_libraries 里加上 Berkeley DB
target_link_libraries(bitcoin db_cxx-4.8)

# 平台特定的链接库
if(WIN32)
    target_link_libraries(bitcoin
        kernel32 user32 gdi32 comdlg32 winspool winmm shell32 
        comctl32 ole32 oleaut32 uuid rpcrt4 advapi32 ws2_32
    )
    
    # 设置Windows子系统
    set_target_properties(bitcoin PROPERTIES
        WIN32_EXECUTABLE TRUE
        LINK_FLAGS "-mwindows -Wl,--subsystem,windows"
    )
elseif(APPLE)
    # macOS特定的链接库
    target_link_libraries(bitcoin
        pthread
        "-framework CoreFoundation"
        "-framework Security"
    )
else()
    # Linux特定的链接库
    target_link_libraries(bitcoin
        pthread
        dl
    )
endif()

# 资源文件处理 (Windows)
if(WIN32)
    # 如果有资源文件，可以添加
    # target_sources(bitcoin PRIVATE src/ui.rc)
endif()

# 安装规则
install(TARGETS bitcoin
    RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
)

# Set default install prefix to user's home directory if not specified
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Install prefix" FORCE)
endif()

# 打印配置信息
message(STATUS "Bitcoin build configuration:")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  wxWidgets found: ${wxWidgets_FOUND}")
message(STATUS "  OpenSSL found: ${OPENSSL_FOUND}")
message(STATUS "  Boost found: ${Boost_FOUND}")
message(STATUS "  Berkeley DB: ${BDB_LIBRARY}")
