cmake_minimum_required(VERSION 3.8)

project(GPF)

add_library(GPF STATIC src/Plugin.cpp src/PluginsManager.cpp)

set_property(TARGET GPF PROPERTY CXX_STANDARD 11)

target_include_directories(GPF PUBLIC include)