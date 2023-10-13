cmake_minimum_required(VERSION 3.25)
project(ycimsg LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCIMSG_SOURCES source/*)

add_executable(ycimsg ${YCIMSG_SOURCES})
target_include_directories(ycimsg PUBLIC include)
target_include_directories(ycimsg PRIVATE ../dependencies/argtable/src)

if (NOT TARGET argtable3)
    add_subdirectory(../dependencies/argtable ../dependencies/argtable)
endif()

target_link_libraries(ycimsg argtable3)
target_link_libraries(ycimsg ycres)