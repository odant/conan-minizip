# minizip Conan package
# Dmitriy Vetutnev, Odant, 2019


find_path(MINIZIP_INCLUDE_DIR
    NAMES mz.h
    PATHS ${CONAN_INCLUDE_DIRS_MINIZIP}
    NO_DEFAULT_PATH
)

find_library(MINIZIP_LIBRARY
    NAMES minizip
    PATHS ${CONAN_LIB_DIRS_MINIZIP}
    NO_DEFAULT_PATH
)


if(MINIZIP_INCLUDE_DIR AND EXISTS ${MINIZIP_INCLUDE_DIR}/mz.h)
    file(STRINGS ${MINIZIP_INCLUDE_DIR}/mz.h DEFINE_MZ_VERSION REGEX "^#define MZ_VERSION[ \t]+.*$")
    string(REGEX REPLACE "^#define MZ_VERSION[ \t]+\\(\"([0-9]+)\\.[0-9]+\\.[0-9]+\"\\)" "\\1" MINIZIP_VERSION_MAJOR ${DEFINE_MZ_VERSION})
    string(REGEX REPLACE "^#define MZ_VERSION[ \t]+\\(\"[0-9]+\\.([0-9]+)\\.[0-9]+\"\\)" "\\1" MINIZIP_VERSION_MINOR ${DEFINE_MZ_VERSION})
    string(REGEX REPLACE "^#define MZ_VERSION[ \t]+\\(\"[0-9]+\\.[0-9]+\\.([0-9]+)\"\\)" "\\1" MINIZIP_VERSION_PATCH ${DEFINE_MZ_VERSION})
    set(MINIZIP_VERSION_STRING "${MINIZIP_VERSION_MAJOR}.${MINIZIP_VERSION_MINOR}.${MINIZIP_VERSION_PATCH}")
    unset(DEFINE_MZ_VERSION)
endif()


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MINIZIP
    REQUIRED_VARS MINIZIP_LIBRARY MINIZIP_INCLUDE_DIR
    VERSION_VAR MINIZIP_VERSION_STRING
)


if(MINIZIP_FOUND)
    set(MINIZIP_INCLUDE_DIRS ${MINIZIP_INCLUDE_DIR})
    set(MINIZIP_LIBRARIES ${MINIZIP_LIBRARY})
    mark_as_advanced(MINIZIP_INCLUDE_DIR MINIZIP_LIBRARY)
    set(MINIZIP_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_MINIZIP}) # Add defines from package_info

    include(CMakeFindDependencyMacro)
    find_dependency(ZLIB)

    if(NOT TARGET MINIZIP::minizip)
        add_library(MINIZIP::minizip UNKNOWN IMPORTED)
        set_target_properties(MINIZIP::minizip PROPERTIES
            INTERFACE_INCLUDE_DIRECTORIES ${MINIZIP_INCLUDE_DIR}
            IMPORTED_LOCATION ${MINIZIP_LIBRARY}
            INTERFACE_LINK_LIBRARIES ZLIB::ZLIB
        )
    endif()
endif()
