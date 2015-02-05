#include "input.h"
#include <avr/io.h>

#define F_CPU 8000000L
#include <avr/delay.h>
#include "display.h"

#define DEBOUNCE_TIME 15

static uint8_t input;
static uint8_t last_input;

static inline uint8_t read_input();
static inline uint8_t get_button_state(uint8_t input, button_e button);

/* External functions */
void input_setup() 
{
  PORTC |= 0b00001111;
}

uint8_t input_on(button_e button)
{
  return !get_button_state(input, button);
}

uint8_t input_pressed(button_e button) 
{
  uint8_t state = get_button_state(input, button);
  uint8_t last_state = get_button_state(last_input, button);
  return (state == 0 && last_state == 1);
}

uint8_t input_depressed(button_e button) 
{
  uint8_t state = get_button_state(input, button);
  uint8_t last_state = get_button_state(last_input, button);
  return (state == 1 && last_state == 0);
}

void input_update()
{
  // Save last input. This is needed for the press and depress functions
  last_input = input;

  // Debounce by reading, waiting DEBOUNCE_TIME ms and reading again
  uint8_t first = read_input();
  _delay_ms(DEBOUNCE_TIME);
  uint8_t next = read_input();

  // OR the two read values to get the resulting value (a press is thus only
  // detected when both first and next is 0
  input = first | next;
}

/* Internal functions */

static inline uint8_t read_input()
{
  return PINC & 0b00001111;
}

static inline uint8_t get_button_state(uint8_t input, button_e button)
{
  return (input >> button) & 1;
}






    
