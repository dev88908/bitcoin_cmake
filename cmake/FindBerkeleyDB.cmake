# FindBerkeleyDB.cmake - 查找Berkeley DB库
# 
# 这个模块定义了以下变量:
#  BerkeleyDB_FOUND - 如果找到Berkeley DB则为TRUE
#  BerkeleyDB_INCLUDE_DIRS - Berkeley DB的包含目录
#  BerkeleyDB_LIBRARIES - 需要链接的Berkeley DB库
#  BerkeleyDB_VERSION - Berkeley DB的版本

find_path(BerkeleyDB_INCLUDE_DIR
    NAMES db_cxx.h
    PATHS
        /usr/include
        /usr/local/include
        /opt/local/include
        /usr/include/db4
        /usr/include/db5
        /usr/include/db6
        /usr/local/include/db4
        /usr/local/include/db5
        /usr/local/include/db6
    PATH_SUFFIXES
        db4 db5 db6
)

find_library(BerkeleyDB_LIBRARY
    NAMES db_cxx db_cxx-6 db_cxx-5 db_cxx-4.8 db_cxx-4.7 db_cxx-4.6
    PATHS
        /usr/lib
        /usr/local/lib
        /opt/local/lib
    PATH_SUFFIXES
        db4 db5 db6
)

# 尝试确定版本
if(BerkeleyDB_INCLUDE_DIR AND EXISTS "${BerkeleyDB_INCLUDE_DIR}/db_cxx.h")
    file(STRINGS "${BerkeleyDB_INCLUDE_DIR}/db_cxx.h" BerkeleyDB_VERSION_MAJOR_LINE
         REGEX "^#define[ \t]+DB_VERSION_MAJOR[ \t]+[0-9]+")
    file(STRINGS "${BerkeleyDB_INCLUDE_DIR}/db_cxx.h" BerkeleyDB_VERSION_MINOR_LINE
         REGEX "^#define[ \t]+DB_VERSION_MINOR[ \t]+[0-9]+")
    
    string(REGEX REPLACE "^#define[ \t]+DB_VERSION_MAJOR[ \t]+([0-9]+)" "\\1"
           BerkeleyDB_VERSION_MAJOR "${BerkeleyDB_VERSION_MAJOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+DB_VERSION_MINOR[ \t]+([0-9]+)" "\\1"
           BerkeleyDB_VERSION_MINOR "${BerkeleyDB_VERSION_MINOR_LINE}")
    
    set(BerkeleyDB_VERSION "${BerkeleyDB_VERSION_MAJOR}.${BerkeleyDB_VERSION_MINOR}")
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BerkeleyDB
    REQUIRED_VARS BerkeleyDB_LIBRARY BerkeleyDB_INCLUDE_DIR
    VERSION_VAR BerkeleyDB_VERSION
)

if(BerkeleyDB_FOUND)
    set(BerkeleyDB_LIBRARIES ${BerkeleyDB_LIBRARY})
    set(BerkeleyDB_INCLUDE_DIRS ${BerkeleyDB_INCLUDE_DIR})
    
    if(NOT TARGET BerkeleyDB::BerkeleyDB)
        add_library(BerkeleyDB::BerkeleyDB UNKNOWN IMPORTED)
        set_target_properties(BerkeleyDB::BerkeleyDB PROPERTIES
            IMPORTED_LOCATION "${BerkeleyDB_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${BerkeleyDB_INCLUDE_DIR}"
        )
    endif()
endif()

mark_as_advanced(BerkeleyDB_INCLUDE_DIR BerkeleyDB_LIBRARY)