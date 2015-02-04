#include "input.h"
#include <avr/io.h>

#define F_CPU 8000000L
#include <avr/delay.h>
#include "display.h"

#define DEBOUNCE_TIME 10

uint8_t input;
uint8_t last_input;

void get_input();
inline uint8_t read_input();

/* External functions */
void input_setup() 
{
  PORTC |= 0b00001111;
}

uint8_t input_on(button_e button)
{
  return !(input >> button);
}

uint8_t input_pressed(button_e button) 
{
  uint8_t state = input >> button;
  uint8_t last_state = last_input >> button;
  return (state == 0 && last_state == 1);
}

uint8_t input_depressed(button_e button) 
{
  uint8_t state = input >> button;
  uint8_t last_state = last_input >> button;
  return (state == 1 && last_state == 0);
}

void input_update()
{
  last_input = input;
  uint8_t first = read_input();
  uint8_t change = input ^ first;
  _delay_ms(DEBOUNCE_TIME);
  uint8_t next = read_input();
  input = change ^ next;
}



/* Internal functions */

inline uint8_t read_input()
{
  uint8_t inp = PINC & 0b00001111;
  //display_binary(inp);
  return inp;
}






    
