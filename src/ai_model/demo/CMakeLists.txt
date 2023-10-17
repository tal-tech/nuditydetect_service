cmake_minimum_required(VERSION 3.10)
project(det_handwrite_maskrcnn)

file(GLOB LINK_LIBRARIES ../lib/*.so)
include_directories(../include)
add_executable(demo demo.cpp)
target_link_libraries(demo ${LINK_LIBRARIES})