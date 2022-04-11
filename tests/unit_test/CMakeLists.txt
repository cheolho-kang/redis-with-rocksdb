cmake_minimum_required(VERSION 3.8)

# Add target sources
file(GLOB_RECURSE test_files *.cpp *.c LIST_DIRECTORIES true)
add_executable(${PROJECT_UT_BINARY} ${test_files})

# Add dependencies
set_target_properties(${PROJECT_UT_BINARY} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_UT_BINARY} ${EXTERNAL_INCS})
target_link_libraries(${PROJECT_UT_BINARY}
    PUBLIC
        gtest
        gmock
        gmock_main
        pthread
        ${EXTERNAL_LIBS}
        ${PROJECT_BINARY}lib
)
target_compile_options(${PROJECT_UT_BINARY} PRIVATE -g -O0)
target_compile_features(${PROJECT_UT_BINARY} PRIVATE cxx_std_11)
target_compile_definitions(${PROJECT_UT_BINARY} PRIVATE -DROCKSDB)
add_test(
    NAME ${PROJECT_UT_BINARY}
    COMMAND ${PROJECT_UT_BINARY}
)
