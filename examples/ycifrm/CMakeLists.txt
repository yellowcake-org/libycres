cmake_minimum_required(VERSION 3.25)
project(ycifrm LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCIFRML_SOURCES source/*)

add_executable(ycifrm ${YCIFRML_SOURCES})
target_include_directories(ycifrm PUBLIC include)
target_include_directories(ycifrm PRIVATE ../dependencies/argtable/src)