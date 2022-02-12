cmake_minimum_required(VERSION 3.10)

add_executable(romisvr
    main.cpp
    romi.cpp
)

target_include_directories(romisvr
PUBLIC
    ${CMAKE_BINARY_DIR}/includes
)
