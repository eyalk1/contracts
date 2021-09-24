cmake_minimum_required(VERSION 3.14)
project(TEST)
find_package(fmt)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 20)

# add_subdirectory(fmt)

add_subdirectory(tests)

