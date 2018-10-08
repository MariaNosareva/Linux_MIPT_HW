cmake_minimum_required(VERSION 3.8)
project(mini_fs)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES main.c inode.h inode.c superblock.h)
add_executable(mini_fs ${SOURCE_FILES})