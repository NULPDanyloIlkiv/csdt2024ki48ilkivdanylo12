cmake_minimum_required(VERSION 3.21)

include(pico_sdk_import.cmake)

project(pico_project)



pico_sdk_init()

add_executable(main
	main.c server.c game.c logic.c bot.c data.c update.c serial_port.c
)



target_link_libraries(main
    pico_stdio pico_stdlib pico_malloc hardware_uart m
)



pico_enable_stdio_usb(main 1)

pico_enable_stdio_uart(main 0)

pico_add_extra_outputs(main)



include_directories(".")
