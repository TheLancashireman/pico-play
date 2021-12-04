cmake_minimum_required(VERSION 3.13)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(blinker_project C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(blinker
	blinker.c
	blink.c
)

#pico_enable_stdio_usb(blinker 1)
#pico_enable_stdio_uart(blinker 1)

pico_add_extra_outputs(blinker)

target_link_libraries(blinker pico_stdlib)
