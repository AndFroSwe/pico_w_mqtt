add_executable(testmain testmain.cpp)
target_link_libraries(testmain pico_stdlib)
pico_add_extra_outputs(testmain)

add_executable(testmqtt testmqtt.cpp)
target_link_libraries(testmqtt pico_stdlib
                      pico_cyw43_arch_lwip_threadsafe_background pico_lwip_mqtt)
target_include_directories(testmqtt PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(testmqtt)
