cmake_minimum_required(VERSION 3.10)
project(ServerProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(server
    src/main.cpp
    src/server.cpp
    src/mergesort.cpp
    src/threadpool.cpp
)
target_include_directories(server PRIVATE include)