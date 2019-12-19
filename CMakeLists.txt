cmake_minimum_required(VERSION 3.1.0)
project(AFunctionLibrary VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(libs/APluginLibrary)
add_subdirectory(libs/pugixml)

set(HEADERS
        include/AFunctionLibrary/function.h src/private/functionprivate.h
        src/private/resourceparser.h
        src/private/resourcemanager.h
        src/private/tokenmanager.h
        src/private/tokenextras.h
        src/private/functionformatter.h
        src/private/syntaxtreehandler.h)
set(SOURCES
        src/function.cpp src/private/src/functionprivate.cpp
        src/private/src/resourceparser.cpp
        src/private/src/resourcemanager.cpp
        src/private/src/tokenmanager.cpp
        src/private/src/functionformatter.cpp
        src/private/src/syntaxtreehandler.cpp)

set(SDK_HEADERS
        SDK/AFunctionLibrary/implementation
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/syntaxtree.h
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/token.h
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/utility.h
        SDK/AFunctionLibrary/implementation/definitions.h
        SDK/AFunctionLibrary/implementation/macros.h
        SDK/AFunctionLibrary/implementation/cstringtoken.h)
set(SDK_SOURCES
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/implementation/syntaxtree.tpp
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/implementation/token.tpp
        SDK/AFunctionLibrary/implementation/include/AFunctionLibrary/implementation/utility.ipp)

set(PUBLIC_INCLUDE_DIRECTORIES
        include
        SDK
        SDK/AFunctionLibrary/implementation/include
        ${CMAKE_CURRENT_BINARY_DIR}/exports)
set(PRIVATE_INCLUDE_DIRECTORIES
        libs/tinydir)

file(COPY res/extensions DESTINATION ${CMAKE_BINARY_DIR})

add_library(AFunctionLibrary STATIC ${HEADERS} ${SOURCES} ${SDK_HEADERS} ${SDK_SOURCES})
include(GenerateExportHeader)
generate_export_header(AFunctionLibrary
        EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/exports/AFunctionLibrary/afunctionlibrary_export.h)
target_include_directories(AFunctionLibrary
        PUBLIC ${PUBLIC_INCLUDE_DIRECTORIES}
        PRIVATE ${PRIVATE_INCLUDE_DIRECTORIES})
target_link_libraries(AFunctionLibrary APluginLibrary pugixml)
target_compile_definitions(AFunctionLibrary PUBLIC AFUNCTIONLIBRARY_BUILD_LIBRARY)

add_subdirectory(res/plugins)
add_dependencies(AFunctionLibrary AFunctionLibraryDefaultPlugin)

set(AFunctionLibraryTest TRUE)
if(${AFunctionLibraryTest})
    add_subdirectory(tests)
endif()
