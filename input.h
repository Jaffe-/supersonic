#pragma once
#include <avr/io.h>
typedef enum {BUTTON_UP, BUTTON_DOWN, BUTTON_RIGHT, BUTTON_LEFT} button_e;

void input_setup();
uint8_t input_pressed(button_e button);
uint8_t input_depressed(button_e button);
uint8_t input_on(button_e button);
void input_update();
