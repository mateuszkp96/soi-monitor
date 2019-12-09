cmake_minimum_required(VERSION 3.15)
project(monitor)

set(CMAKE_CXX_STANDARD 14)

add_executable(monitor main.cpp monitor.h BufferMonitor.h Message.h)
target_link_libraries(monitor pthread)