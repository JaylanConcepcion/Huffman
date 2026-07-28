cmake_minimum_required(VERSION 3.23)
project(huffman)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 14)

add_executable(huffman HuffmanDriver.cpp Huffman.h Huffman.cpp Node.h Storage/Storage.cpp Storage/Storage.h)
add_executable(StorageDriver Storage/StorageDriver.cpp Storage/Storage.cpp Storage/Storage.h)
