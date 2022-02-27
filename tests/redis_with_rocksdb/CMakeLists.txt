cmake_minimum_required(VERSION 3.8)

# Add target sources
file(GLOB_RECURSE test_files *.cpp LIST_DIRECTORIES true)
add_executable(${PROJECT_UT_BINARY} ${test_files})

# Add dependencies
target_include_directories(${PROJECT_UT_BINARY} PUBLIC ${INCLUDE_DIRS})
target_link_libraries(${PROJECT_UT_BINARY}
    PUBLIC
        gtest
        gmock
        gmock_main
        ${PROJECT_LIBRARY}
)
target_compile_features(${PROJECT_UT_BINARY} PRIVATE cxx_std_11)
add_test(
    NAME ${PROJECT_UT_BINARY}
    COMMAND ${PROJECT_UT_BINARY}
)
