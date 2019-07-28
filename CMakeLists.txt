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

add_executable(test safe_stack_listing_3_5.cpp ex_two_threads.cpp ex_create_4_threads.cpp ex_stop_thread.cpp ex_pi.cpp thread_safe_list.cpp)
add_executable(two_threads_ex ex_two_threads.cpp)
add_executable(create_4_threads_ex ex_create_4_threads.cpp)
add_executable(ex_stop_thread ex_stop_thread.cpp)
add_executable(ex_pi ex_pi.cpp)
add_executable(thread_safe_list thread_safe_list.cpp)
target_link_libraries (thread_safe_list ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (ex_pi ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (ex_stop_thread ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (create_4_threads_ex ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries (two_threads_ex ${CMAKE_THREAD_LIBS_INIT})

#find_package(GTest REQUIRED)
#include_directories(${GTEST_INCLUDE_DIRS})