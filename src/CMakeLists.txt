add_executable(mqttmain mqttmain.cpp)
target_link_libraries(mqttmain pico_stdlib
                      pico_cyw43_arch_lwip_threadsafe_background pico_lwip_mqtt)
target_include_directories(mqttmain PUBLIC ${CMAKE_CURRENT_LIST_DIR})
pico_add_extra_outputs(mqttmain)
