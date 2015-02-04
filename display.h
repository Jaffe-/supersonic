#pragma once
#include "fixed_point.h"
#include <avr/io.h>

void display_setup();
void display_clear();
void display_returnhome();
void display_setcursor(uint8_t screen, uint8_t line, uint8_t pos); 
void display_scroll(uint8_t direction);
void display_print(char* string);
void display_move_window(uint8_t screen);
void display_print_number(uint16_t number);
void display_print_fixed(fixed_t number);
void display_write_character(uint8_t index, uint8_t pixels[8]);
void display_print_char(char c);
