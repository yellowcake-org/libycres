cmake_minimum_required(VERSION 3.25)
project(ycipro LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCIPRO_SOURCES source/*)

add_executable(ycipro ${YCIPRO_SOURCES})
target_include_directories(ycipro PUBLIC include)
target_include_directories(ycipro PRIVATE ../dependencies/argtable/src)