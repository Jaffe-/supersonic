#include <avr/io.h>

#define F_CPU 8000000L

#include <util/delay.h>
#include <math.h>
#include "pulse.h"
#include "display.h"
#include "input.h"
#include "animation.h"

int main()
{  
  //while(1);
  display_setup(); 
  input_setup();
  pulse_setup();
  screen_setup();

  menu_handler();


}
