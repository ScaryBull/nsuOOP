cmake_minimum_required(VERSION 3.14)
project(flat_hash_map_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(test_flat_hash_map
  test_flat_hash_map.cpp
)

target_include_directories(test_flat_hash_map PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(test_flat_hash_map PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/arch:SSE2>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-msse2>
)

target_link_libraries(test_flat_hash_map PRIVATE gtest_main)

include(GoogleTest)
gtest_discover_tests(test_flat_hash_map)
