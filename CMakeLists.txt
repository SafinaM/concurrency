cmake_minimum_required(VERSION 3.6)
project(concurrency)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Threads)
if(Threads)
	message("Threads found!")
endif()

#set(CMAKE_CXX_FLAGS "-lncurses")
#find_package(Curses REQUIRED)
#include_directories(${CURSES_INCLUDE_DIR})

add_executable(test safe_stack_listing_3_5.cpp)
#target_link_libraries (mutex_example ${CMAKE_THREAD_LIBS_INIT})

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})