cmake_minimum_required(VERSION 3.15...4.00)

project(cmusic-tests)

file(GLOB test_files *.c)
add_executable(cmusic-tests ${test_files})
target_include_directories(cmusic-tests PRIVATE ../include ../lib)

file(GLOB src_files ../src/*.c)
list(FILTER src_files EXCLUDE REGEX "main.c")
add_library(src ${src_files})
target_include_directories(src PRIVATE ../include ../lib)

add_library(cJSON SHARED ../lib/cjson/cJSON.c)
target_include_directories(cJSON PRIVATE ../lib/cjson)

find_package(PkgConfig REQUIRED)
pkg_check_modules(criterion REQUIRED IMPORTED_TARGET criterion)

enable_testing()

target_link_libraries(cmusic-tests src curl cJSON PkgConfig::criterion)
