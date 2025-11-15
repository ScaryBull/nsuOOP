cmake_minimum_required(VERSION 3.14)
project(lab1 VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(googletest)

set(SOURCES src/PriorityQueue.cpp)
set(HEADERS include/PriorityQueue.h)
set(TEST_SOURCES test/testPriorityQueue.cpp)

add_executable(lab1_tests
        ${SOURCES}
        ${TEST_SOURCES}
        ${HEADERS}
)

target_include_directories(lab1_tests PRIVATE include)
target_link_libraries(lab1_tests PRIVATE gtest gtest_main)