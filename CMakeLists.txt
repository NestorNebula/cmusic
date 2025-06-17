cmake_minimum_required(VERSION 3.15...4.00)

project(cmusic
        DESCRIPTION "Managing your favorite music"
        VERSION 1.00
        LANGUAGES C)

file(GLOB src_files src/*.c)
add_executable(cmusic ${src_files})
target_include_directories(cmusic PRIVATE include lib)

add_library(cJSON SHARED lib/cjson/cJSON.c)
target_include_directories(cJSON PRIVATE lib/cjson)
target_link_libraries(cmusic cJSON curl)
