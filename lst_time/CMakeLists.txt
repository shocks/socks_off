cmake_minimum_required(VERSION 3.0.0)
project(lst_time)

aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_DIR)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(lst_time 
${SOURCE_DIR}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
