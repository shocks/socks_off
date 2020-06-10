cmake_minimum_required(VERSION 3.0.0)
project(socks_epoll)

#if(COMPILER_SUPPORTS_CXX11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#elseif(COMPILER_SUPPORTS_CXX0X)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#else()
#message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()
#add_compile_options(-std=c++11)
##add_definitions(-std=c++11)
#set(CMAKE_CXX_STANDARD 11)

aux_source_directory(${PROJECT_SOURCE_DIR} SOURCE_DIR)

add_executable(socks_epoll
${SOURCE_DIR}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

target_include_directories(socks_epoll PUBLIC ${PROJECT_SOURCE_DIR})
find_package(Threads)
target_link_libraries(socks_epoll ${CMAKE_THREAD_LIBS_INIT})

include(CPack)
