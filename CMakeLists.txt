# 指定编译器
set (CMAKE_C_COMPILER "D:/compliers/mingw64_posix_seh/bin/gcc.exe")
set (CMAKE_CXX_COMPILER "D:/compliers/mingw64_posix_seh/bin/g++.exe")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 设置编译器选项
# set(CMAKE_CXX_FLAGS   "-lmingw32 -lSDL2main -lSDL2")  
# set(CMAKE_C_FLAGS   "-lmingw32 -lSDL2main -lSDL2")  

# cmake 最低版本
cmake_minimum_required(VERSION 3.27)
# 项目名称
project(SmallStep)

# 使用 .exe 运行时静态链接
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# 定义路径
message(${CMAKE_CURRENT_SOURCE_DIR})

set(RUN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/run)

# 可执行文件输出位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RUN_DIR}/bin)
# 动态库输出位置
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${RUN_DIR}/bin)
# 静态库输出位置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${RUN_DIR}/lib)

list(APPEND CMAKE_PREFIX_PATH "D:/3rd_lib")

# 指明第三方头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/3rd)

# 链接库文件
link_directories(${RUN_DIR}/lib
                 ${RUN_DIR}/bin)

add_subdirectory(Game)

add_subdirectory(DataStructure&Algorithm)