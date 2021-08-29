cmake_minimum_required(VERSION 3.15)

project(database LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

add_subdirectory(api)

add_executable(database main.cpp)
target_include_directories(database PUBLIC api/include)
target_link_libraries(database PUBLIC api)