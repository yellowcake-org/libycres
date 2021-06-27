cmake_minimum_required(VERSION 3.9)
project(libycres VERSION 0.1.0)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu89 -Wall -Wextra")

add_library(ycres 
    sources/formats/dat/dat.c
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
