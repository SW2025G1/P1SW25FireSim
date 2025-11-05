# This file downloads mtest to the project.
# To use it, just include the file in the top level CMakeLists.txt file, e.g.:
#   include(cmake/mtest.cmake)

# Setting this policy to NEW avoids a warning in newer versions of CMake.
cmake_policy(SET CMP0135 NEW)

# Installation of a testing framework - here the simple mtest.
include(FetchContent)
FetchContent_Declare(mtest
    URL https://github.com/MortenSchou/mtest/archive/refs/tags/v0.2.0.zip
    URL_HASH SHA256=bec1e90fb00a5bc155de12ed5ed39ea5d1a1b6fcfb6c80cce5ad3e38e360248c
)
FetchContent_MakeAvailable(mtest)
