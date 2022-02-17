cmake_minimum_required(VERSION 3.19)

add_executable(romisvr
    main.cpp
    romi.cpp
)

target_include_directories(romisvr
PUBLIC
    ${CMAKE_SOURCE_DIR}/includes
)