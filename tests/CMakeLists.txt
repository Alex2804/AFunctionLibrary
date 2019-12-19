cmake_minimum_required(VERSION 2.8.12)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(COPY res/extensions DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)

set(SOURCES
        src/node_test.cpp
        src/syntaxtree_test.cpp
        src/token_test.cpp
        src/utility_test.cpp
        src/resourceparser_test.cpp
        src/resourcemanager_test.cpp
        src/functionformatter_test.cpp
        src/tokenmanager_test.cpp
        src/tokenextras_test.cpp)

add_executable(AFunctionLibraryTest ${SOURCES})
target_link_libraries(AFunctionLibraryTest AFunctionLibrary gtest_main)
add_test(NAME AFunctionLibraryTest COMMAND AFunctionLibraryTest)

add_subdirectory(res/plugins/c_api)
add_subdirectory(res/plugins/cpp_api)
add_dependencies(AFunctionLibraryTest c_api_plugin)
add_dependencies(AFunctionLibraryTest cpp_api_plugin)