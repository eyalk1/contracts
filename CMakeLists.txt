cmake_minimum_required(VERSION 3.14)
project(TEST)
set(CMAKE_CXX_COMPILER=clang++-12)
find_package(fmt)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# add_subdirectory(fmt)

add_subdirectory(tests)

