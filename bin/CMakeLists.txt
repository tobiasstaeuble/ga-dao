cmake_minimum_required(VERSION 2.8)

project(ga-dao)
set(VERSION 0.1.0)

set(CMAKE_C_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_COMPILER "/usr/bin/mpicxx")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -I ../lib/eigen/")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=gnu++11 -I ../lib/eigen/")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")     
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE}") 

set(INCLUDE_DIRS "/usr/local/include")
set(MOGAL_LIBRARY "/usr/local/lib/libmogal.so")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}../lib)

include_directories("${INCLUDE_DIRS}" ../include)
file(GLOB SOURCES "../src/*.cpp")

add_executable(ga-dao ${SOURCES})


target_link_libraries(ga-dao "${MOGAL_LIBRARY}")







