cmake_minimum_required(VERSION 3.25)
project(ycipal LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCIPAL_SOURCES source/*)

add_executable(ycipal ${YCIPAL_SOURCES})
target_include_directories(ycipal PUBLIC include)
target_include_directories(ycipal PRIVATE ../dependencies/argtable/src)