cmake_minimum_required(VERSION 3.1.0)
project(AFunctionLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(libs/APluginLibrary)

set(HEADERS
        include/AFunctionLibrary/function.h
        include/AFunctionLibrary/syntaxtree.h
        include/AFunctionLibrary/token.h
        include/AFunctionLibrary/utility.h)
set(SOURCES
        src/function.cpp
        include/AFunctionLibrary/implementation/syntaxtree.tpp
        include/AFunctionLibrary/implementation/token.tpp
        src/utility.cpp include/AFunctionLibrary/implementation/utility.tpp)

add_library(AFunctionLibrary STATIC ${HEADERS} ${SOURCES})
include(GenerateExportHeader)
generate_export_header(AFunctionLibrary
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/AFunctionLibrary/afunctionlibrary_export.h)
target_include_directories(AFunctionLibrary PUBLIC include "${CMAKE_CURRENT_BINARY_DIR}/exports")
target_link_libraries(AFunctionLibrary APluginLibrary)

if(${AFunctionLibraryTest})
    add_subdirectory(tests)
endif()