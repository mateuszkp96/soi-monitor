cmake_minimum_required(VERSION 3.15)
project(monitor)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES
        BufferMonitor.h
        main.cpp
        Message.h
        monitor.h
        Timer.h
        Utils.h
        )

add_executable(monitor ${SOURCE_FILES})
target_link_libraries(monitor pthread)