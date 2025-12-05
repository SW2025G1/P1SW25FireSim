cmake_minimum_required(VERSION 3.14)
project(Sandbox C)
set(CMAKE_C_STANDARD 11)
add_compile_options(-Wall -Wpedantic -Wextra)

# Include dependencies
include(cmake/mtest.cmake)

add_subdirectory(src)
enable_testing()
add_subdirectory(test)

