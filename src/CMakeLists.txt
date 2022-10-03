cmake_minimum_required(VERSION 3.15)
project(matrix)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_INSTALL_PREFIX install)

set(SOURCES MatrixException.cpp)

add_executable(matrix ${SOURCES} main.cpp)

target_include_directories(matrix PRIVATE ../include)

if(SHARED_LIB)
    add_library(Matrix SHARED ${SOURCES})
    install(DIRECTORY ../include DESTINATION ../include)
    install(TARGETS matrix DESTINATION lib)
endif()

install(TARGETS matrix DESTINATION bin)