cmake_minimum_required(VERSION 3.25)
project(libycres LANGUAGES C VERSION 0.8.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCRES_SOURCES source/ source/**)

add_library(ycres ${YCRES_SOURCES})
target_include_directories(ycres PUBLIC include)
target_include_directories(ycres PRIVATE private)

add_subdirectory(examples/ycifrm)
add_subdirectory(examples/ycilst)
add_subdirectory(examples/ycimap)
add_subdirectory(examples/ycimsg)
add_subdirectory(examples/ycipal)
add_subdirectory(examples/ycipro)
add_subdirectory(examples/ycundat)
