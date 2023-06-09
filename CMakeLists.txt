cmake_minimum_required(VERSION 3.9)

add_subdirectory(dependencies/argtable)

project(libycres VERSION 0.3.2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c89 -pedantic -pedantic-errors -W -Wall -Wstrict-prototypes -Wunreachable-code -Wwrite-strings -Wpointer-arith -Wbad-function-cast -Wcast-align -Wcast-qual -Werror=pedantic")

add_library(ycres 
    sources/formats/dat/dat.c
    sources/formats/dat/lzss.c
    sources/formats/dat/load.c
    sources/formats/dat/flat.c
)

target_include_directories(ycres PUBLIC 
    include
)

target_include_directories(ycres PRIVATE
    private
)

add_executable(undat 
    examples/undat/sources/main.c
    examples/undat/sources/print/print.c
    examples/undat/sources/extract/extract.c
    examples/undat/sources/iterate/iterate.c
    examples/undat/sources/platform/platform.c
    examples/undat/sources/filesystem/filesystem.c
    
    examples/undat/sources/private/print/print.c
    examples/undat/sources/private/extract/extract.c
)

add_dependencies(undat
    argtable3
    ycres
)

target_include_directories(undat PRIVATE
    dependencies/argtable/src
)

target_include_directories(undat PRIVATE
    examples/undat/include
)

target_link_libraries(undat LINK_PUBLIC
    argtable3
    ycres
)
