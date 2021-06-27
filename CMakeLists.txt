cmake_minimum_required(VERSION 3.9)
project(libycres VERSION 0.1.0)

add_library(ycres 
    sources/dat/dat.c
)

target_include_directories(ycres PUBLIC 
    include
)

add_executable(undat 
    examples/undat/sources/main.c
)

target_include_directories(undat PRIVATE 
    examples/undat/include
)

target_link_libraries(undat LINK_PUBLIC 
    ycres
)