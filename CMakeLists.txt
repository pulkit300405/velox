cmake_minimum_required(VERSION 3.20)
project(Velox)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(velox src/main.cpp)
target_include_directories(velox PRIVATE include)

add_executable(benchmark bench/benchmark.cpp)
target_include_directories(benchmark PRIVATE include)