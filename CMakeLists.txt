cmake_minimum_required(VERSION 3.15)

project(cargs
    VERSION 1.0.0
    LANGUAGES C
)

add_library(cargs STATIC
    src/cargs.c
)

target_include_directories(cargs
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(cargs PUBLIC c_std_11)
