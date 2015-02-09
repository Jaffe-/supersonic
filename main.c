#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>
#include "pulse.h"
#include "display.h"
#include "input.h"
#include "animation.h"

int main()
{
  display_setup(); 
  input_setup();
  pulse_setup();

  uint8_t bupp_frames[][8] = {
    {0b01110,
     0b10001,
     0b00000,
     0b00000,
     0b01110,
     0b10001,
     0b00000,
     0b00100},
    
    {0b10001,
     0b00000,
     0b00000,
     0b01110,
     0b10001,
     0b00000,
     0b00100,
     0b01010},

    {0b00000,
     0b00000,
     0b01110,
     0b10001,
     0b00000,
     0b01110,
     0b10001,
     0b00000}
  };

  uint16_t logo_frames[][16] = {
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000100000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000
    },
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000100000,
      0b0001010000,
      0b0000100000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000
    },
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0001100000,
      0b0010010000,
      0b0100001000,
      0b0010010000,
      0b0001100000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000
    },
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0011100000,
      0b0100010000,
      0b0100001000,
      0b1000001000,
      0b1000001000,
      0b0100010000,
      0b0011100000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000
    },
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0111110000,
      0b1000001000,
      0b0000000100,
      0b0000000100,
      0b0000000100,
      0b0000000100,
      0b1000000100,
      0b0110011000,
      0b0001100000,
      0b0000000000,
      0b0000000000,
      0b0000000000
    },
    {
      0b0000000000,
      0b0000000000,
      0b0000000000,
      0b0111110000,
      0b1000001000,
      0b0000000100,
      0b0000000010,
      0b0000000010,
      0b0000000010,
      0b0000000010,
      0b0000000010,
      0b1000000100,
      0b0100001000,
      0b0011110000,
      0b0000000000,
      0b0000000000
    }
  };

  
  animation_t logo = {.width = 2, .height = 2,
		      .frames = logo_frames,
		      .num_frames = 6,
		      .first_character = 1,
		      .count = 2};
  /*
  animation_t bupp = {.width = 1, .height = 1,
		      .frames = bupp_frames,
		      .num_frames = 3,
		      .first_character = 0,
		      .count = 8};
*/    
  display_setcursor(1, 2, 1);
  display_print_string("Fuck you! ");

  uint16_t timer_value = 65535;  
  float distance = (timer_value * 340.0) / (2 * 1000000.0);

  animation_init(&logo);
  animation_display(&logo, 1, 1, 14);
  
  display_setcursor(1, 1, 1);
  display_print_float(distance * 1000);
  display_setcursor(2, 1, 1);
  display_print_string("Fuck you too!");
  while(1) {
    animation_animate(&logo);
    
    input_update();

    if (input_pressed(BUTTON_RIGHT))
      display_move_window(2);
    if (input_pressed(BUTTON_LEFT))
      display_move_window(1);
    if (input_pressed(BUTTON_UP))
      pulse(65000);
      
  }
}
