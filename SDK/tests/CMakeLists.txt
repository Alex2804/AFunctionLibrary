cmake_minimum_required(VERSION 2.8.12)

enable_language(CXX)
set(CMAKE_CXX_STANDARD 11)
enable_language(C)

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(libs/googletest
        ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
        EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

add_subdirectory(libs/check)

set(SOURCES
        ../AFunctionLibrary/libs/APluginSDK/src/cpp/pluginapi.cpp
        src/test_utilities.cpp
        src/test_node.cpp
        src/test_syntaxtree.cpp
        src/test_token.cpp
        src/test_cstring.cpp
        src/test_cstringtoken.cpp

        src/testrunner.c
        ../AFunctionLibrarySDK/libs/ACUtils/src/astring.c
        ../AFunctionLibrarySDK/private/src/atoken.c
        ../AFunctionLibrarySDK/private/src/atokengroup.c
        ../AFunctionLibrarySDK/private/src/astringaliases.c
        ../AFunctionLibrarySDK/private/src/atokennode.c
        src/test_atoken.c
        src/test_atokengroup.c
        src/test_astringaliases.c
        src/test_atokennode.c
        )

add_executable(AFunctionLibrarySDKTest ${SOURCES})
target_link_libraries(AFunctionLibrarySDKTest gtest_main check)
target_compile_definitions(AFunctionLibrarySDKTest PUBLIC APLUGINSDK_EXCLUDE_IMPLEMENTATION)
add_test(NAME AFunctionLibrarySDKTest COMMAND AFunctionLibrarySDKTest)
