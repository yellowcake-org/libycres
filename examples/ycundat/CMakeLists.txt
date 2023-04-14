cmake_minimum_required(VERSION 3.25)
project(ycundat LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCUNDAT_SOURCES source/*)

add_executable(ycundat ${YCUNDAT_SOURCES})
target_include_directories(ycundat PUBLIC include)
target_include_directories(ycundat PRIVATE ../dependencies/argtable/src)