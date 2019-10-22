cmake_minimum_required(VERSION 3.1.0)
project(AFunctionLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
        include/AFunctionLibrary/function.h)
set(SOURCES
        src/function.cpp)

add_library(AFunctionLibrary STATIC ${HEADERS} ${SOURCES})
target_include_directories(AFunctionLibrary PUBLIC include)

set(AFunctionLibraryTest TRUE)
if(${AFunctionLibraryTest})
    add_subdirectory(tests)
endif()