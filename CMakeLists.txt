cmake_minimum_required(VERSION 3.31)

project(Batch2Bash C)

set(PROGRAM bat2sh)

file(GLOB SRC_FILES src/*.c)

add_executable(${PROGRAM} ${SRC_FILES})

target_include_directories(${PROGRAM} PRIVATE include)
