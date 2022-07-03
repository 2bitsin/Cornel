cmake_minimum_required(VERSION 3.18)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(calculator)
add_executable(calculator main.cpp)
target_link_libraries(calculator PRIVATE common)
