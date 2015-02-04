#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>
#include "pulse.h"
#include "display.h"
#include "fixed_point.h"
#include "input.h"

int main()
{
  display_setup(); 
  input_setup();
  pulse_setup();

  display_setcursor(1, 2, 1);
  display_print("Fuck you!");
  display_setcursor(2, 1, 1);

  fixed_t a = {.value = -19, .scale = 1};
  fixed_t b = {.value = 2990, .scale = 3};

  //display_print_number(-fixed_sub(a,b).value);
  display_print_fixed(fixed_div(a,b));
  while(1) {
    input_update();
    if (input_pressed(BUTTON_RIGHT))
      display_move_window(2);
    if (input_pressed(BUTTON_LEFT))
      display_move_window(1);
    if (input_pressed(BUTTON_UP))
      pulse(65000);
  }
}
