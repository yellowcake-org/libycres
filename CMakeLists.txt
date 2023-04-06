cmake_minimum_required(VERSION 3.25)
project(libycres C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

add_library(
        ycres
        source/formats/dat/dat.c
)
target_include_directories(
        ycres PUBLIC
        include
)
