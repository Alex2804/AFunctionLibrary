cmake_minimum_required(VERSION 3.1.0)
project(ACUtilsTest VERSION 0.1)

add_subdirectory(tests/libs/check)

add_executable(ACUtilsTest tests/src/testrunner.c tests/src/test_adynarray.c tests/src/test_astring.c src/adynarray.c src/astring.c)
set_target_properties(ACUtilsTest PROPERTIES C_STANDARD 90 C_EXTENSIONS off)
#target_compile_definitions(ACUtilsTest PUBLIC ACUTILS_ONE_SOURCE)
target_include_directories(ACUtilsTest PUBLIC include)
target_link_libraries(ACUtilsTest check)
