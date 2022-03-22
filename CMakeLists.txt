# The Romi Project
#
# @ File   :  CMakeLists.txt
# @ Author :  Tensiya(T2SU)
# @ Email  :  tsi@romi-project.net
#

cmake_minimum_required(VERSION 3.19)
project(RomiServerAlpha VERSION 1.0)

if(MSVC)
  string (REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
  string (REPLACE "/Zi" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
  add_compile_options(/W4 /WX /fsanitize=address /Z7)
else()
  add_compile_options(-Wall -Werror -Wextra -g)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(srcs)

add_subdirectory(plog)

target_include_directories(${PROJECT_NAME}
	PUBLIC plog/include
)
target_link_libraries(${PROJECT_NAME}
  PUBLIC plog
)
