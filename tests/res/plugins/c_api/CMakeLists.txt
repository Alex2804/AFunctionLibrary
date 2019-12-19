cmake_minimum_required(VERSION 3.1.0)

add_library(c_api_plugin SHARED src/main.cpp)
target_include_directories(c_api_plugin PRIVATE ../../../../SDK)
set_target_properties(c_api_plugin PROPERTIES PREFIX "")
