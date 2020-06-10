cmake_minimum_required(VERSION 3.0.0)
project(ini_file)

aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_DIR)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(ini_file 
${SOURCE_DIR}
)

target_include_directories(ini_file PUBLIC "/usr/include/mysql")
target_include_directories(ini_file PUBLIC "/usr/local/boost/include/boost")
#include<boost/include/boost/>
target_link_libraries(ini_file hiredis)
target_link_libraries(ini_file mysqlclient)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
