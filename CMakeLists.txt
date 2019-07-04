# minizip Conan package
# Dmitriy Vetutnev, Odant 2019


cmake_minimum_required(VERSION 3.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

enable_testing()
add_subdirectory(src)

