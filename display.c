/* 
   Displayfunksjoner

   LCM1602C

   Displayet er koblet til følgende pinner:

   Display-pinne    Signal      Atmega
   4                RS          PD7
   6                E           PB0
   7 - 11           DB0 - DB4   PD0 - PD4
   12, 13           DB5, DB6    PB6, PB7
   14               DB7         PD5
*/

#include "display.h"
#include "fixed_point.h"
#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>

// Bitmasks for pins on PORTD
#define DB_PORTD_m 0b00111111
#define DB0_4_PORTD_m 0b00011111
#define DB7_PORTD_m 0b00100000

// Bitmasks for pins on PORTB
#define DB_PORTB_m 0b11000000
#define RS_PORTD_m 0b10000000
#define E_PORTB_m 1

// Instructions
#define CLEAR_DISPLAY 1
#define RETURN_HOME 0b10
#define ENTRY_MODE 0b100
#define DISPLAY_CONTROL 0b1000
#define DISPLAY_SHIFT 0b10000
#define FUNCTION_SET 0b100000
#define SET_CGRAM_ADDRESS 0b1000000
#define SET_DDRAM_ADDRESS 0b10000000

// Write modes
#define MODE_INSTRUCTION 0
#define MODE_DATA 1

// Scroll directions
#define RIGHT 0
#define LEFT 1

uint8_t current_screen = 1;

#define nop() asm volatile("nop")

inline void control(uint8_t D, uint8_t C, uint8_t B);
inline void entrymode(uint8_t increment, uint8_t shift);
inline void functionset(uint8_t DL, uint8_t N, uint8_t F);
inline void shift(uint8_t SC, uint8_t RL);
inline void cgram_address(uint8_t address);
inline void ddram_address(uint8_t address);

void write(uint8_t, uint8_t);

/* low level functions */

void db_set(uint8_t db_value) 
{
  PORTD &= ~DB_PORTD_m;
  PORTD |= (db_value & DB0_4_PORTD_m) | ((db_value >> 2) & DB7_PORTD_m);
  PORTB &= ~DB_PORTB_m;
  PORTB |= (db_value << 1) & DB_PORTB_m;
}

void rs_set(uint8_t rs_value)
{
  PORTD &= ~RS_PORTD_m;
  PORTD |= rs_value << 7;
}

void e_set(uint8_t e_value)
{
  PORTB &= ~E_PORTB_m;
  PORTB |= e_value;
}

void write(uint8_t mode, uint8_t data)
/*
  mode: instruction or data (0: instruction, 1: data)
  data: value to write
*/
{
  rs_set(mode);
  db_set(data); 
  e_set(1);
  nop();
  e_set(0); 
  _delay_ms(2);
}

/* Display instructions */

inline void entrymode(uint8_t increment, uint8_t shift)
// increment - 0: dekrement cursor, 1: inkrementer cursor
// shift - 0: ingen shift, 1: shift
{
  write(MODE_INSTRUCTION, ENTRY_MODE | (increment << 1) | shift);
}

inline void control(uint8_t D, uint8_t C, uint8_t B)
// D: display on/off
// C: cursor on/off
// B: blink cursor on/off 
{
  write(MODE_INSTRUCTION, DISPLAY_CONTROL | (D << 2) | (C << 1) | B); 
}

inline void functionset(uint8_t DL, uint8_t N, uint8_t F)
// DL: data length (0: 4-bit, 1: 8-bit)
// N: number of display lines (0: 1-line, 1: 2-line)
// F: character font (0: 5x8, 1: 5x10) (except when N = 1; then it is 5x8 anyway)
{
  write(MODE_INSTRUCTION, FUNCTION_SET | (DL << 4) | (N << 3) | (F << 2));
}

inline void shift(uint8_t SC, uint8_t RL) 
/*
  SC: shift mode (0: cursor move, 1: display shift)
  RL: direction (0: right, 1: left)
*/
{
  write(MODE_INSTRUCTION, DISPLAY_SHIFT | (SC << 3) | (RL << 2));
}

inline void cgram_address(uint8_t address)
// address: 6 bit CGRAM address
{
  write(MODE_INSTRUCTION, SET_CGRAM_ADDRESS | (address & 0x3F));
}

inline void ddram_address(uint8_t address)
// address: 7 bit DDRAM address
{
  write(MODE_INSTRUCTION, SET_DDRAM_ADDRESS | (address & 0x7F));
}


/* External functions */

void display_setup() 
{
  DDRD |= 0b10111111;
  DDRB |= 0b11000001;

  _delay_ms(50);
  functionset(1, 0, 0);
  _delay_ms(5);
  functionset(1, 0, 0);
  _delay_us(150);
  functionset(1, 0, 0);
  
  functionset(1, 1, 1);
  control(1, 0, 0);
  display_clear();
  entrymode(1, 0);
}

void display_clear()
{
  write(MODE_INSTRUCTION, CLEAR_DISPLAY);
}

void display_returnhome()
{
  write(MODE_INSTRUCTION, RETURN_HOME);
}


void display_setcursor(uint8_t screen, uint8_t line, uint8_t pos) 
/*
  screen: 1, 2
  line: 1, 2
  pos: 1 - 40
*/
{
  uint8_t address = 0;
  if (line == 2) 
    address = 0x40;

  if (screen == 2)
    address += 16;

  address += pos - 1;

  ddram_address(address);
}

void display_scroll(uint8_t direction)
{
  shift(1, direction);
}

void display_print(char* string) 
{
  for (uint8_t i = 0; string[i] != 0; i++) 
    write(MODE_DATA, string[i]);
}

void scroll_window(uint8_t direction)
{
  for (uint8_t i = 0; i < 16; i++) {
    display_scroll(direction);
    _delay_ms(50);
  }
}

void display_move_window(uint8_t screen)
{
  if (current_screen == screen)
    return;

  if (screen == 1) 
    scroll_window(LEFT);

  else
    scroll_window(RIGHT);

  current_screen = screen;
}

void display_print_number(uint16_t number)
{
  uint8_t digits[6] = {0};

  uint8_t i = 5;
  while (number > 0) {
    uint8_t digit = number % 10;
    digits[i--] = digit;   
    number -= digit;
    number /= 10;
  }

  for (uint8_t j = i+1; j < 6; j++) {
    write(MODE_DATA, 48 + digits[j]);
  }

}

void display_print_fixed(fixed_t number) 
{
  uint16_t scale = 1;
  for (uint8_t i = 0; i < number.scale; i++) 
    scale *= 10;

  if (number.value < 0) {
    write(MODE_DATA, '-');
    number.value = -number.value;
  }

  uint8_t first_digit = number.value / scale;
  uint16_t rest_digits = number.value - first_digit * scale;

  write(MODE_DATA, 48 + first_digit);
  if (rest_digits > 0) {
    write(MODE_DATA, '.');
    display_print_number(rest_digits);
  }
}
