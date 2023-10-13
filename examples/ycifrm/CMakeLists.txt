cmake_minimum_required(VERSION 3.25)
project(ycifrm LANGUAGES C VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCIFRM_SOURCES source/*)

add_executable(ycifrm ${YCIFRM_SOURCES})
target_include_directories(ycifrm PUBLIC include)
target_include_directories(ycifrm PRIVATE ../dependencies/argtable/src)

if (NOT TARGET argtable3)
add_subdirectory(../dependencies/argtable ../dependencies/argtable)
endif()

target_link_libraries(ycifrm argtable3)
target_link_libraries(ycifrm ycres)