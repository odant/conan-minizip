# Tests for minizip Conan package
# Dmitriy Vetutnev, Odant 2019


project(PackageTest C)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

if(MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
endif()

enable_testing()

find_package(MINIZIP REQUIRED)

add_executable(test_pack test_pack.c)
target_link_libraries(test_pack MINIZIP::minizip)
set_target_properties(test_pack
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_pack
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_pack
)

add_executable(test_list test_list.c)
target_link_libraries(test_list MINIZIP::minizip)
set_target_properties(test_list
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_list
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_list
)
set_tests_properties(test_list PROPERTIES DEPENDS test_pack)

add_executable(test_unpack test_unpack.c)
target_link_libraries(test_unpack MINIZIP::minizip)
set_target_properties(test_unpack
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_unpack
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_unpack
)
set_tests_properties(test_unpack PROPERTIES DEPENDS test_pack)
