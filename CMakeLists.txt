cmake_minimum_required (VERSION 3.13)
project(csc-opt)
set(CMAKE_CXX_STANDARD 17)
# add_definitions(-O0 -g3 -ggdb)
add_definitions(-Wall -g)

# 定义变量
set(PROJ_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(SOURCE_DIR ${PROJ_DIR}/src)
set(BUILD_DIR ${PROJ_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${BUILD_DIR})

# 添加头文件目录
include_directories(${SOURCE_DIR})
include_directories(${SOURCE_DIR}/pass)
include_directories(${SOURCE_DIR}/utils)
include_directories(${PROJ_DIR}/Lyra/include)
include_directories(${PROJ_DIR}/Lyra/include/lyra)
# 添加所有源文件
file(GLOB_RECURSE SRC_CPP ${SOURCE_DIR}/*.cpp)
# 添加目标文件
add_executable(${PROJECT_NAME} ${SRC_CPP})
