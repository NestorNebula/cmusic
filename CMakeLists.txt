cmake_minimum_required(VERSION 3.15...4.00)

project(cmusic
        DESCRIPTION "Managing your favorite music"
        VERSION 1.00
        LANGUAGES C)

file(GLOB src_files src/*.c)
add_executable(cmusic ${src_files})
target_include_directories(cmusic include)
