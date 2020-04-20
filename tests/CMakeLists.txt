cmake_minimum_required(VERSION 2.8.12)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

file(COPY res/extensions DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/res)

set(SOURCES
        src/test_resourceparser.cpp
        src/test_resourcemanager.cpp
        src/test_tokenmanager.cpp
        src/test_functionformatter.cpp
        src/test_functionoptimizer.cpp
        )

add_executable(AFunctionLibraryTest ${SOURCES})
target_link_libraries(AFunctionLibraryTest AFunctionLibrary gtest_main)
add_test(NAME AFunctionLibraryTest COMMAND AFunctionLibraryTest)

add_subdirectory(res/plugins/c_api)
add_subdirectory(res/plugins/cpp_api)
add_dependencies(AFunctionLibraryTest c_api_plugin)
add_dependencies(AFunctionLibraryTest cpp_api_plugin)
