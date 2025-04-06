add_executable(testmain testmain.cpp)
target_link_libraries(testmain pico_stdlib)

pico_add_extra_outputs(testmain)
