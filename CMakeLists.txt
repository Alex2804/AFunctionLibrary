cmake_minimum_required(VERSION 3.1.0)
project(AFunctionLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(libs/APluginLibrary)
add_subdirectory(libs/pugixml)

set(HEADERS
        include/AFunctionLibrary/function.h
        include/AFunctionLibrary/syntaxtree.h
        include/AFunctionLibrary/token.h
        include/AFunctionLibrary/utility.h
        src/private/resourceparser.h
        src/private/tokenextras.h)
set(SOURCES
        src/function.cpp
        include/AFunctionLibrary/implementation/syntaxtree.tpp
        include/AFunctionLibrary/implementation/token.tpp
        src/utility.cpp include/AFunctionLibrary/implementation/utility.tpp
        src/private/resourceparser.cpp)

set(SDK_HEADERS
        SDK/AFunctionLibrary/implementation/cstringtoken.h)
set(SDK_SOURCES)

set(PUBLIC_INCLUDE_DIRECTORIES
        include
        SDK
        ${CMAKE_CURRENT_BINARY_DIR}/exports)
set(PRIVATE_INCLUDE_DIRECTORIES
        libs/tinydir)

add_library(AFunctionLibrary STATIC ${HEADERS} ${SOURCES} ${SDK_HEADERS} ${SDK_SOURCES})
include(GenerateExportHeader)
generate_export_header(AFunctionLibrary
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/AFunctionLibrary/afunctionlibrary_export.h)
target_include_directories(AFunctionLibrary
        PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES}
        PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
target_link_libraries(AFunctionLibrary APluginLibrary pugixml)

if(${AFunctionLibraryTest})
    add_subdirectory(tests)
endif()