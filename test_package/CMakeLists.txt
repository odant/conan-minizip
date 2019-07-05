# Tests for minizip Conan package
# Dmitriy Vetutnev, Odant 2019


project(PackageTest C)
cmake_minimum_required(VERSION 3.0)


include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

if(MSVC)
    add_compile_definitions("_CRT_SECURE_NO_WARNINGS")
endif()

enable_testing()


# Conan imported targets

add_executable(test_pack_CONAN_PKG test_pack.c)
target_link_libraries(test_pack_CONAN_PKG CONAN_PKG::minizip)
set_target_properties(test_pack_CONAN_PKG
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_pack_CONAN_PKG
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_pack_CONAN_PKG
)

add_executable(test_list_CONAN_PKG test_list.c)
target_link_libraries(test_list_CONAN_PKG CONAN_PKG::minizip)
set_target_properties(test_list_CONAN_PKG
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_list_CONAN_PKG
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_list_CONAN_PKG
)
set_tests_properties(test_list_CONAN_PKG PROPERTIES DEPENDS test_pack_CONAN_PKG)

add_executable(test_unpack_CONAN_PKG test_unpack.c)
target_link_libraries(test_unpack_CONAN_PKG CONAN_PKG::minizip)
set_target_properties(test_unpack_CONAN_PKG
    PROPERTIES
    INSTALL_RPATH "$ORIGIN"
    BUILD_WITH_INSTALL_RPATH True
)
add_test(
    NAME test_unpack_CONAN_PKG
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    COMMAND test_unpack_CONAN_PKG
)
set_tests_properties(test_unpack_CONAN_PKG PROPERTIES DEPENDS test_pack_CONAN_PKG)
