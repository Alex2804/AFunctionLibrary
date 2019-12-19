cmake_minimum_required(VERSION 3.1.0)

add_library(cpp_api_plugin SHARED src/main.cpp)
target_include_directories(cpp_api_plugin PRIVATE ../../../../SDK)
set_target_properties(cpp_api_plugin PROPERTIES PREFIX "")
