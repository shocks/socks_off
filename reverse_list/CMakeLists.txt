cmake_minimum_required(VERSION 3.0.0)
project(reverse_list)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_DIR)

add_executable(reverse_list
${SOURCE_DIR}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
