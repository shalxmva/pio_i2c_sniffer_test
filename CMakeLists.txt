# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.1.1)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.1.1)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(sniffer C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pio_i2c_sniffer_test main.c lcd_master.c)

pico_set_program_name(pio_i2c_sniffer_test "pio_i2c_sniffer")
pico_set_program_version(pio_i2c_sniffer_test "0.1")

# Modify the below lines to enable/disable output over UART/USB
pico_enable_stdio_uart(pio_i2c_sniffer_test 0)
pico_enable_stdio_usb(pio_i2c_sniffer_test 1)

# Add the standard library to the build
target_link_libraries(pio_i2c_sniffer_test 
        pico_stdlib)

# Add the standard include files to the build
target_include_directories(pio_i2c_sniffer_test PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)

#uncomment next line to run a lcd master that will send a character each time PIN13 is set to low
# Must add jumpers from LCD I2C pins to SNIFFER I2C pins
#SNIFFER SDA = 0
#SNIFFER SCL = 1
#LCD SCL = 3
#LCD SDA = 2
#BUTTON  = 13
target_compile_definitions(pio_i2c_sniffer_test PRIVATE -DRUN_LCD_MASTER)

add_subdirectory(pio_i2c_sniffer) # 1 ADDS thelib folder to cmake

# Add any user requested libraries
target_link_libraries(pio_i2c_sniffer_test 
        pico_multicore
        hardware_i2c
        pio_i2c_sniffer               # 2 Link the I2C sniffer library
        )

pico_add_extra_outputs(pio_i2c_sniffer_test)

