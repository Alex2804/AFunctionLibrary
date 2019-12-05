cmake_minimum_required(VERSION 3.1.0)
project(AFunctionLibraryDefaultPlugin VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS ../../src/private/implementation/tokenmanager.tpp)
set(SOURCES
        src/createtokensdefault.cpp)

set(PUBLIC_INCLUDE_DIRECTORIES
        ../../SDK)
set(PRIVATE_INCLUDE_DIRECTORIES)

add_library(AFunctionLibraryDefaultPlugin SHARED ${HEADERS} ${SOURCES})
target_include_directories(AFunctionLibraryDefaultPlugin
        PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES}
        PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
