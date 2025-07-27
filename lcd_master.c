/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

 #include <stdio.h>
 #include <string.h>
 #include "pico/stdlib.h"
 #include "hardware/i2c.h"
 
 #define I2C_ADDR       0x27         // I2C slave address for the LCD module

 int master_i2c_init()
 {
     uint8_t b = 0x00; // Initialize a byte to send
 
     i2c_init(i2c1, 100000); // Initialize I2C0 with 100kHz
     gpio_init(2); // Initialize SDA pin
     gpio_init(3); // Initialize SCL pin
 
     gpio_set_function( 2, GPIO_FUNC_I2C); // Set SDA pin function
     gpio_set_function( 3, GPIO_FUNC_I2C); // Set SCL pin function
     gpio_pull_up(2); // Enable pull-up on SDA
     gpio_pull_up(3); // Enable pull-up on SCL
 
     return 0;
 }
 
 void port_write(uint8_t val, bool is_command, bool en)
 {
     uint8_t data = val << 4;
     data |= 0x08; // Set bit high to enable the LCD
 
     // This function sends data to the PCF8574 I/O expander
     // is_command: true for command, false for data
     if (!is_command) {
         data |= 0x01; // Set RS bit for data
     }
 
     if(en) {
         data |= 0x04; // Set EN bit high to latch the data
     }
     
     i2c_write_blocking(i2c1, I2C_ADDR, &data, 1, false); // Send data to slave at address 0x27
 }
 
 void lcd_send_nibble(uint8_t nibble, bool is_command)
 {
     sleep_us(200); // Small delay to ensure the LCD processes the command
     port_write(nibble, is_command, true); // Set EN bit high and send
     port_write(nibble, is_command, false); // Set EN bit low to latch the data
 }
 
 void lcd_send_byte(uint8_t byte, bool is_command)
 {
     // Send high nibble
     lcd_send_nibble(byte >> 4, is_command);
     // Send low nibble
     lcd_send_nibble( byte &0xF , is_command);
 }
 
 void lcd_send_command(uint8_t command)
 {
     lcd_send_byte(command, true); // Send command
 }
 
 void lcd_print_char(char c)
 {
     lcd_send_byte(c, false); // Send character
 }
 
 void lcd_print_string(const char *str)
 {
     while (*str) {
         lcd_print_char(*str++); // Send each character in the string
     }
 }
 
 void lcd_init() 
 {  
     
     lcd_send_nibble(3, true);
     sleep_ms(5);       
     lcd_send_nibble(3, true);
     sleep_ms(1);       
     lcd_send_nibble(3, true);
     sleep_ms(1);
 
     lcd_send_nibble(2, true);
     lcd_send_command(0x28); // Function set: 4-bit mode, 2 lines, 5x8 dotS
     lcd_send_command(0x08); // Function set: 4-bit mode, 2 lines, 5x8 dots
     lcd_send_command(0x01); // Function set: 4-bit mode, 2 lines, 5x8 dots
 
     lcd_send_command(6);
     lcd_send_command(0x0E); // Display on, cursor on, blink on
  }
 
  void core1_entry()
  {
    char lcd_message[] = "Hello, LCD!";
    uint8_t lcd_message_length = strlen(lcd_message);
    uint8_t lcd_message_index = 0;
     bool button_pressed = true;
     bool button = true;
     gpio_init(13);
     gpio_set_dir(13, GPIO_IN);
     gpio_pull_up(13); // Enable pull-up on GPIO 13
 
     master_i2c_init();
     lcd_init();
     button_pressed = button = gpio_get(13);
     while (true) {
         // This core can be used for other tasks or to handle I2C sniffer logic
         button = gpio_get(13); // Read GPIO 13 state
         if(!button && button_pressed) {
             lcd_print_char(lcd_message[lcd_message_index]); // Clear display if GPIO 13 is high
             lcd_message_index++;
                if (lcd_message_index >= lcd_message_length) {
                    lcd_message_index = 0; // Reset index if end of message is reached
                }
         }
         button_pressed = button; // Update button state
         sleep_ms(10); // Debounce delay
     }
  }
 