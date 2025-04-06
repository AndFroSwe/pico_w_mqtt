cmake_minimum_required(VERSION 3.22)

include(lib/pico-sdk/pico_sdk_init.cmake)

project(testpico)

pico_sdk_init()

add_subdirectory(src)
