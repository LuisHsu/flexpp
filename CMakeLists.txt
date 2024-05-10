cmake_minimum_required(VERSION 3.27)
project(flex)

set(CMAKE_PROJECT_VERSION_MAJOR 2)
set(CMAKE_PROJECT_VERSION_MINOR 6)
set(CMAKE_PROJECT_VERSION_PATCH 4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_subdirectory(src)