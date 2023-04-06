cmake_minimum_required(VERSION 3.25)
project(ycundat C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCUNDAT_SOURCES source/*)

add_executable(ycundat ${YCUNDAT_SOURCES})
target_include_directories(ycundat PUBLIC include)
target_include_directories(ycundat PRIVATE dependencies/argtable/src)

add_subdirectory(dependencies/argtable)
add_dependencies(ycundat argtable3)
target_link_libraries(ycundat argtable3)