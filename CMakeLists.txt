# Set project feature
cmake_minimum_required(VERSION 3.8)
set(project_NAME redis-with-rocksdb)
project(${project_NAME}
        VERSION 0.1
        DESCRIPTION "Key-Value Stroe NoSQL based on redis that use rocksdb as backend engines"
)
set(PROJECT_BINARY ${project_NAME})
set(PROJECT_LIBRARY ${project_NAME}lib)
set(PROJECT_UT_BINARY unit_test)
set(CMAKE_VERBOSE_MAKEFILE false)

# External Dependences
include(ExternalProject)

set(default_build_type "Release")
if (NOT CMAKE_BUILD_DIRECTORY)
    set(CMAKE_BUILD_DIRECTORY ${CMAKE_BINARY_DIR})
endif()
# GLIBC < 2.17 should explict specify the real time library when use clock_*
find_library(REALTIME_LIB rt)
if (REALTIME_LIB)
    list(APPEND EXTERNAL_LIBS PRIVATE rt)
endif()

include(cmake/jemalloc.cmake)
include(cmake/lua.cmake)
include(cmake/hiredis.cmake)
include(cmake/hdr_histogram.cmake)
include(cmake/linenoise.cmake)

list(APPEND EXTERNAL_LIBS PRIVATE ${jemalloc_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${jemalloc_INCLUDE_DIRS})

list(APPEND EXTERNAL_LIBS PRIVATE ${lua_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${lua_INCLUDE_DIRS})

list(APPEND EXTERNAL_LIBS PRIVATE ${hiredis_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${hiredis_INCLUDE_DIRS})

list(APPEND EXTERNAL_LIBS PRIVATE ${hdr_histogram_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${hdr_histogram_INCLUDE_DIRS})

list(APPEND EXTERNAL_LIBS PRIVATE ${linenoise_LIBRARIES})
list(APPEND EXTERNAL_INCS PRIVATE ${linenoise_INCLUDE_DIRS})


# End dependencies

# CXX settings
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
endif()

# Main target
add_subdirectory(src)

# Unit-test target
add_subdirectory(tests/redis_with_rocksdb)