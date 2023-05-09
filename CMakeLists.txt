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
add_subdirectory(examples/ycifrm)
add_subdirectory(examples/ycipro)
add_subdirectory(examples/ycimap)
add_subdirectory(examples/ycilst)
add_subdirectory(examples/ycimsg)

add_dependencies(ycundat argtable3)
add_dependencies(ycipal argtable3)
add_dependencies(ycifrm argtable3)
add_dependencies(ycipro argtable3)
add_dependencies(ycimap argtable3)
add_dependencies(ycilst argtable3)
add_dependencies(ycimsg argtable3)

target_link_libraries(ycundat argtable3)
target_link_libraries(ycundat ycres)

target_link_libraries(ycipal argtable3)
target_link_libraries(ycipal ycres)

target_link_libraries(ycifrm argtable3)
target_link_libraries(ycifrm ycres)

target_link_libraries(ycipro argtable3)
target_link_libraries(ycipro ycres)

target_link_libraries(ycimap argtable3)
target_link_libraries(ycimap ycres)

target_link_libraries(ycilst argtable3)
target_link_libraries(ycilst ycres)

target_link_libraries(ycimsg argtable3)
target_link_libraries(ycimsg ycres)
