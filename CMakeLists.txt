cmake_minimum_required(VERSION 3.10)
project(romisvr VERSION 1.0)

add_compile_options(-Wall -Werror -Wextra)
set(CMAKE_CXX_FLAGS "-fsanitize=address -g")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(srcs)
