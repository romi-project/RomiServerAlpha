cmake_minimum_required(VERSION 3.19)
project(romisvr VERSION 1.0)

if(MSVC)
  string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
  string (REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  add_compile_options(/W4 /WX /fsanitize=address /Z7)
else()
  add_compile_options(-Wall -Werror -Wextra -fsanitize=address -g)
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(srcs)