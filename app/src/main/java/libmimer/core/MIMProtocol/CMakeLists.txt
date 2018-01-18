# Make 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (mimp)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
include(../mimer.cmake)

#定义ELLOG位置
get_property(ELLOG_BUILED_MODULE GLOBAL PROPERTY "ellog_build")
find_library(ELLOG ellog ${CMAKE_MODULE_PATH}/lib)
if(NOT ELLOG AND NOT ELLOG_BUILED_MODULE)
	set(ELLOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../)
	add_subdirectory(${ELLOG_DIR} ${PROJECT_BINARY_DIR}/ellog.out)
	message(STATUS "ellog will build at ${PROJECT_NAME}")
else()
	set(ELLOG_LINK_DIR ${CMAKE_MODULE_PATH}/lib)
	message(STATUS "ellog is found at ${CMAKE_MODULE_PATH}/lib")
endif(NOT ELLOG AND NOT ELLOG_BUILED_MODULE)

include (CTest)
# 指定include 路径
include_directories(./ ../ellog/ ../utils/stream)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
aux_source_directory(../utils/stream/Stream.cpp DIR_SRCS)
link_directories(${ELLOG_LINK_DIR})
# 生成动态链接库
add_library(mimp SHARED ${DIR_SRCS})
add_library(mimp_static STATIC ${DIR_SRCS})
target_link_libraries(mimp ellog)
set_target_properties(mimp_static PROPERTIES OUTPUT_NAME "mimp")
set_target_properties(mimp PROPERTIES VERSION 0.1 SOVERSION 1)
