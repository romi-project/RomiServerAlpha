cmake_minimum_required(VERSION 3.19)

add_executable(romisvr
 romi.cpp
 "win.cpp")

 add_subdirectory(error)

target_include_directories(romisvr
PUBLIC
    ${CMAKE_SOURCE_DIR}/includes
)