set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})  # Place programs in root folder when compiled.

add_library(visualizer.c visualizer.c)
target_include_directories(visualizer.c PUBLIC . ${CMAKE_SOURCE_DIR}/src)