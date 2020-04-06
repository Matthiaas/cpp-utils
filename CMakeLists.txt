cmake_minimum_required(VERSION 3.15)
project(utils)

set(CMAKE_CXX_STANDARD 20)

add_executable(utils main.cpp parallel/ParallelQueue.h datastructures/Queue.cpp datastructures/Queue.h datastructures/Deque.cpp datastructures/Deque.h parallel/Guard.h)