cmake_minimum_required(VERSION 3.15...4.00)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cmusic-tests
        LANGUAGES C CXX)

file(GLOB test_files *.cpp)
add_executable(cmusic-tests ${test_files})
target_include_directories(cmusic ../include)

file(GLOB src_files ../src/*.c)
add_library(src src_files)
target_include_directories(src ../include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

target_link_libraries(cmusic GTest::gtest src)
