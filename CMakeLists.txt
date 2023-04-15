cmake_minimum_required(VERSION 3.25)
project(libycres LANGUAGES C VERSION 0.2.0)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -pedantic -Werror)

FILE(GLOB_RECURSE YCRES_SOURCES source/ source/**)

add_library(ycres ${YCRES_SOURCES})
target_include_directories(ycres PUBLIC include)
target_include_directories(ycres PRIVATE private)

add_subdirectory(examples/dependencies/argtable)

add_subdirectory(examples/ycundat)
add_subdirectory(examples/ycipal)

add_dependencies(ycundat argtable3)
add_dependencies(ycipal argtable3)

target_link_libraries(ycundat argtable3)
target_link_libraries(ycundat ycres)

target_link_libraries(ycipal argtable3)
target_link_libraries(ycipal ycres)
