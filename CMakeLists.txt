cmake_minimum_required(VERSION 3.15)
project(utils)

set(CMAKE_CXX_STANDARD 20)

add_executable(utils main.cpp datastructures/Queue.h datastructures/Deque.h parallel/Guard.h parallel/ThreadPoolExecutor.h datastructures/FibonacciHeap.h)

target_link_libraries(utils pthread)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")


find_package(Boost 1.57.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(BoostTest main.cpp datastructures/Heap.h datastructures/Vector.h)
target_link_libraries(BoostTest ${Boost_LIBRARIES})